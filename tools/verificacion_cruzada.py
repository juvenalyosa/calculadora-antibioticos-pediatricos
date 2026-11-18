"""Calcula la matriz alfa con una implementación independiente del motor en C++.

Uso: python3 tools/verificacion_cruzada.py [repo]

Sigue las reglas escritas en docs/guia_datos.md (no el código del motor), así
que sirve como verificación cruzada. NO reemplaza el cálculo a mano del equipo
clínico: por eso `calculado_por` dice «desarrollo».
"""
import csv
import math
import sys
from pathlib import Path

REPO = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(__file__).resolve().parent.parent
FUENTE = REPO / "data" / "fuente"
AUTOR = "desarrollo (verificación cruzada; falta cálculo clínico)"

PACIENTES = [("lactante", 12, 10, 75), ("adolescente", 156, 50, 162)]
# Escenarios renales: uno por tramo habitual de las bandas.
TFG_RENALES = [40, 20, 5]


def leer(nombre):
    with open(FUENTE / f"{nombre}.csv", encoding="utf-8") as archivo:
        return list(csv.DictReader(archivo))


def num(texto):
    texto = texto.strip().replace(",", ".")
    return float(texto) if texto else None


def texto(valor):
    """Número sin ceros sobrantes y con punto decimal."""
    if valor is None:
        return ""
    redondeado = round(valor, 4)
    return str(int(redondeado)) if redondeado == int(redondeado) else repr(redondeado)


TOPES = {}


def aplica(fila, meses, kg):
    edad_min, edad_max = num(fila["edad_min_meses"]), num(fila["edad_max_meses"])
    peso_min, peso_max = num(fila["peso_min_kg"]), num(fila["peso_max_kg"])
    return not ((edad_min is not None and meses < edad_min)
                or (edad_max is not None and meses >= edad_max)
                or (peso_min is not None and kg < peso_min)
                or (peso_max is not None and kg >= peso_max))


def dosis_por_toma(fila, kg):
    usual, intervalo = num(fila["dosis_usual"]), num(fila["intervalo_horas"])
    base = fila["base"]
    if base == "mg_kg_dia":
        dosis = usual * kg * intervalo / 24
    elif base == "mg_kg_dosis":
        dosis = usual * kg
    else:  # dosis_fija
        dosis = usual
    # Topes del esquema o, si faltan, del medicamento por esa vía.
    tope_med = TOPES.get((fila["medicamento_id"], fila["via"]), {})
    tope_toma = num(fila["max_por_dosis"]) if fila["max_por_dosis"] else num(tope_med.get("max_por_dosis", ""))
    tope_dia = num(fila["max_por_dia"]) if fila["max_por_dia"] else num(tope_med.get("max_por_dia", ""))
    if tope_toma is not None:
        dosis = min(dosis, tope_toma)
    if tope_dia is not None and intervalo:
        dosis = min(dosis, tope_dia * intervalo / 24)
    return dosis, intervalo


def banda_de(bandas, medicamento, tfg):
    for banda in bandas:
        if banda["medicamento_id"] == medicamento and num(banda["tfg_min"]) <= tfg < num(banda["tfg_max"]):
            return banda
    return None


def ajuste_renal(banda, kg, dosis, intervalo):
    """Dosis de reemplazo, luego fracción, luego tope de la banda; intervalo = el mayor."""
    reemplazo, factor = num(banda["dosis_por_kg"]), num(banda["factor_dosis"])
    tope, minimo = num(banda["max_por_dosis"]), num(banda["intervalo_minimo_horas"])
    if reemplazo is not None:
        dosis = reemplazo * kg
    if factor is not None:
        dosis *= factor
    if tope is not None:
        dosis = min(dosis, tope)
    if minimo is not None and intervalo is not None:
        intervalo = max(intervalo, minimo)
    return dosis, intervalo


def cantidad(presentacion, dosis, tope_toma):
    por_unidad = num(presentacion["cantidad"])
    if presentacion["tipo"] == "liquido":
        por_unidad /= num(presentacion["volumen_ml"])
        paso = num(presentacion["paso_redondeo"]) or 0.5
    else:
        paso = num(presentacion["paso_redondeo"]) or (0.5 if presentacion["divisible"] == "si" else 1)
    exacta = dosis / por_unidad
    redondeada = math.floor(exacta / paso + 0.5) * paso
    if tope_toma is not None and redondeada * por_unidad > tope_toma * (1 + 1e-9):
        redondeada = math.floor(exacta / paso + 1e-9) * paso
    desviacion = abs(redondeada * por_unidad - dosis) / dosis
    return redondeada, desviacion


def tope_por_toma(fila, banda):
    """Tope por toma que limita el redondeo: el del esquema o el del medicamento,
    y el de la banda renal si es menor."""
    tope_med = TOPES.get((fila["medicamento_id"], fila["via"]), {})
    tope = num(fila["max_por_dosis"]) if fila["max_por_dosis"] else num(tope_med.get("max_por_dosis", ""))
    if banda is not None and num(banda["max_por_dosis"]) is not None:
        tope = min(tope, num(banda["max_por_dosis"])) if tope is not None else num(banda["max_por_dosis"])
    return tope


def elegir_presentacion(presentaciones, fila, dosis, preferida, tope):
    """La presentación adecuada (desviación ≤ 10 %) del tipo preferido, si existe."""
    candidatas = []
    for p in presentaciones:
        if p["medicamento_id"] == fila["medicamento_id"] and p["via"] == fila["via"]:
            valor, desviacion = cantidad(p, dosis, tope)
            if valor > 0 and desviacion <= 0.10 + 1e-9:
                candidatas.append((p["tipo"] != preferida, desviacion, p["id"], valor))
    if not candidatas:
        return "", None
    _, _, identificador, valor = min(candidatas)
    return identificador, valor


def main():
    esquemas, presentaciones, bandas = leer("esquemas"), leer("presentaciones"), leer("ajuste_renal")
    TOPES.update({(t["medicamento_id"], t["via"]): t for t in leer("topes_medicamento")})
    abreviatura = {"primera": "1", "segunda": "2", "alergia": "al"}
    filas = []
    for nombre, meses, kg, talla in PACIENTES:
        preferida = "liquido" if nombre == "lactante" else "solido"
        for fila in esquemas:
            if fila["base"] == "sin_calculo" or not aplica(fila, meses, kg):
                continue
            # Un escenario renal por cada banda del medicamento que no sea «evitar».
            escenarios = [None] + [tfg for tfg in TFG_RENALES
                                   if (banda := banda_de(bandas, fila["medicamento_id"], tfg))
                                   and banda["evitar"] != "si"]
            for tfg in escenarios:
                dosis, intervalo = dosis_por_toma(fila, kg)
                banda = banda_de(bandas, fila["medicamento_id"], tfg) if tfg is not None else None
                if banda is not None:
                    dosis, intervalo = ajuste_renal(banda, kg, dosis, intervalo)
                presentacion, valor = elegir_presentacion(presentaciones, fila, dosis, preferida,
                                                          tope_por_toma(fila, banda))
                caso = "-".join([fila["enfermedad_id"], nombre,
                                 abreviatura[fila["linea"]] + fila["opcion"],
                                 "normal" if tfg is None else f"tfg{tfg}"])
                filas.append([caso, meses, kg, talla, "" if tfg is None else tfg,
                              fila["enfermedad_id"], fila["linea"], fila["opcion"],
                              fila["medicamento_id"], texto(dosis), texto(intervalo),
                              presentacion, texto(valor), AUTOR])

    encabezado = ["caso", "edad_meses", "peso_kg", "talla_cm", "tfg", "enfermedad_id", "linea",
                  "opcion", "medicamento_id", "dosis_por_toma", "intervalo_horas",
                  "presentacion_id", "cantidad", "calculado_por"]
    destino = REPO / "tests" / "data" / "matriz_alfa.csv"
    with open(destino, "w", newline="", encoding="utf-8") as archivo:
        salida = csv.writer(archivo, lineterminator="\n")
        salida.writerow(encabezado)
        salida.writerows(filas)
    assert not any("," in str(campo) for fila in filas for campo in fila), "coma dentro de un campo"
    print(f"{len(filas)} filas en {destino}")


if __name__ == "__main__":
    main()
