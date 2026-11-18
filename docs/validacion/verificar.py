"""Verificación exhaustiva de la calculadora con la población modelada.

Uso:
  python3 docs/validacion/verificar.py <poblacion.csv> <resultados.csv> [resultados_repetidos.csv]

Lee las hojas de datos (data/fuente), la población (poblacion.py) y la salida
del motor (`calc-dosis lote`). Sin usar código del motor, comprueba:

  A. Cálculo: cada dosis, intervalo y cantidad de presentación coincide con
     un recálculo independiente a partir de las reglas escritas en
     docs/guia_datos.md.
  B. Aplicabilidad: cada opción aparece calculada o «no aplica» según los
     límites de edad y peso del esquema.
  C. Reglas de seguridad tomadas de la literatura:
     C1 la dosis por kilogramo entregada no supera el máximo de la guía;
     C2 la dosis no supera los topes por toma ni por día;
     C3 con función renal reducida la dosis no aumenta ni el intervalo se acorta;
     C4 donde la ficha técnica indica «evitar», el medicamento no se calcula;
     C5 todo esquema por peso sin tope muestra la advertencia SIN_TOPE_MAXIMO;
     C6 las presentaciones marcadas adecuadas desvían ≤ 10 % y no superan el
        tope por toma;
     C7 a igual edad y función renal, la dosis no disminuye al aumentar el peso.
  D. Determinismo: dos ejecuciones dan el mismo archivo.
  E. Concordancia con tablas publicadas de dosis por peso (AIEPI 2012 y OMS
     AWaRe 2022).

Escribe docs/validacion/informe.json y un resumen en pantalla. Termina con
código 1 si alguna comprobación de A a D falla.
"""
import csv
import hashlib
import json
import math
import sys
from collections import Counter, defaultdict
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent.parent
FUENTE = REPO / "data" / "fuente"
TOL = 1e-6          # tolerancia relativa para comparar números
DESVIO_MAX = 0.10   # desviación máxima aceptable por redondeo (limits::kMaxRoundingDeviation)
UMBRAL_SIN_DATO = 90.0


def leer(ruta, **kw):
    with open(ruta, encoding="utf-8", newline="") as archivo:
        return list(csv.DictReader(archivo, **kw))


def num(texto):
    texto = (texto or "").strip().replace(",", ".")
    return float(texto) if texto else None


def igual(a, b):
    return abs(a - b) <= TOL * max(1.0, abs(a), abs(b))


# --- Hojas de datos ----------------------------------------------------------
esquemas = leer(FUENTE / "esquemas.csv")
enfermedades = {e["id"]: e for e in leer(FUENTE / "enfermedades.csv")}
medicamentos = {m["id"]: m for m in leer(FUENTE / "medicamentos.csv")}
presentaciones = defaultdict(list)
for p in leer(FUENTE / "presentaciones.csv"):
    presentaciones[(p["medicamento_id"], p["via"])].append(p)
bandas = defaultdict(list)
for b in leer(FUENTE / "ajuste_renal.csv"):
    bandas[b["medicamento_id"]].append(b)
topes_med = {(t["medicamento_id"], t["via"]): t for t in leer(FUENTE / "topes_medicamento.csv")}
por_clave = {(e["enfermedad_id"], e["linea"], e["opcion"], e["medicamento_id"]): e for e in esquemas}


def topes_de(esquema):
    """(tope por toma, tope diario): los del esquema o, si faltan, los del medicamento."""
    t = topes_med.get((esquema["medicamento_id"], esquema["via"]), {})
    por_toma = num(esquema["max_por_dosis"]) if esquema["max_por_dosis"] else num(t.get("max_por_dosis", ""))
    por_dia = num(esquema["max_por_dia"]) if esquema["max_por_dia"] else num(t.get("max_por_dia", ""))
    return por_toma, por_dia
opciones = defaultdict(list)
for e in esquemas:
    opciones[(e["enfermedad_id"], e["linea"], e["opcion"])].append(e)


# --- Recálculo independiente -------------------------------------------------
def aplica(esquema, meses, kg):
    lim = [(num(esquema["edad_min_meses"]), meses, True), (num(esquema["edad_max_meses"]), meses, False),
           (num(esquema["peso_min_kg"]), kg, True), (num(esquema["peso_max_kg"]), kg, False)]
    for limite, valor, es_minimo in lim:
        if limite is None:
            continue
        if es_minimo and valor < limite:
            return False
        if not es_minimo and valor >= limite:
            return False
    return True


def dosis_base(esquema, kg):
    usual, intervalo = num(esquema["dosis_usual"]), num(esquema["intervalo_horas"])
    if esquema["base"] == "mg_kg_dia":
        dosis = usual * kg * intervalo / 24
    elif esquema["base"] == "mg_kg_dosis":
        dosis = usual * kg
    else:
        dosis = usual
    tope_toma, tope_dia = topes_de(esquema)
    if tope_toma is not None:
        dosis = min(dosis, tope_toma)
    if tope_dia is not None and intervalo:
        dosis = min(dosis, tope_dia * intervalo / 24)
    return dosis, intervalo


def banda_de(medicamento, tfg):
    for b in bandas.get(medicamento, []):
        if num(b["tfg_min"]) <= tfg < num(b["tfg_max"]):
            return b
    return None


def esperado(esquema, kg, tfg):
    """(estado, dosis, intervalo) que deberían salir del motor."""
    if esquema["base"] == "sin_calculo":
        return "sin_calculo", None, None
    dosis, intervalo = dosis_base(esquema, kg)
    medicamento = medicamentos[esquema["medicamento_id"]]
    if tfg is not None and medicamento["ajuste_renal_tipo"] == "por_bandas":
        b = banda_de(esquema["medicamento_id"], tfg)
        if b is not None:
            if b["evitar"] == "si":
                return "no_recomendado", None, None
            if num(b["dosis_por_kg"]) is not None:
                dosis = num(b["dosis_por_kg"]) * kg
            if num(b["factor_dosis"]) is not None:
                dosis *= num(b["factor_dosis"])
            if num(b["max_por_dosis"]) is not None:
                dosis = min(dosis, num(b["max_por_dosis"]))
            if num(b["intervalo_minimo_horas"]) is not None and intervalo is not None:
                intervalo = max(intervalo, num(b["intervalo_minimo_horas"]))
    return "calculado", dosis, intervalo


def cantidad(presentacion, dosis, tope_toma):
    por_unidad = num(presentacion["cantidad"])
    if presentacion["tipo"] == "liquido":
        por_unidad /= num(presentacion["volumen_ml"])
        paso = num(presentacion["paso_redondeo"]) or 0.5
    else:
        paso = num(presentacion["paso_redondeo"]) or (0.5 if presentacion["divisible"] == "si" else 1.0)
    exacta = dosis / por_unidad
    redondeada = math.floor(exacta / paso + 0.5) * paso
    if tope_toma is not None and redondeada * por_unidad > tope_toma * (1 + 1e-9):
        redondeada = math.floor(exacta / paso + 1e-9) * paso
    desvio = (redondeada * por_unidad - dosis) / dosis
    return redondeada, desvio, redondeada > 0 and abs(desvio) <= DESVIO_MAX + 1e-9, por_unidad


# --- Lectura de población y resultados ----------------------------------------
def main():
    ruta_pob, ruta_res = Path(sys.argv[1]), Path(sys.argv[2])
    repetida = Path(sys.argv[3]) if len(sys.argv) > 3 else None
    pacientes = {p["caso"]: p for p in leer(ruta_pob)}
    resultados = leer(ruta_res)

    fallos = defaultdict(list)
    conteo = Counter()

    def falla(codigo, detalle):
        fallos[codigo].append(detalle)

    vistos = defaultdict(set)   # (caso, enfermedad) -> {(linea, opcion)}
    dosis_por = {}              # para C3 y C7

    for r in resultados:
        caso, enfermedad = r["caso"], r["enfermedad_id"]
        p = pacientes[caso]
        meses, kg = int(p["edad_meses"]), float(p["peso_kg"])
        tfg = num(p["tfg"])
        conteo["filas"] += 1
        if r["estado"] in ("sin_antibiotico", "bloqueado"):
            conteo[r["estado"]] += 1
            if r["estado"] == "sin_antibiotico" and enfermedades[enfermedad]["requiere_antibiotico"] != "no":
                falla("B", f"{caso} {enfermedad}: sin antibiótico pero la hoja lo indica")
            if r["estado"] == "bloqueado":
                falla("B", f"{caso} {enfermedad}: bloqueado")
            continue
        linea, opcion = r["linea"], r["opcion"]
        vistos[(caso, enfermedad)].add((linea, opcion))
        componentes = opciones[(enfermedad, linea, opcion)]

        if r["estado"] == "no_aplica":
            conteo["no_aplica"] += 1
            if all(aplica(c, meses, kg) for c in componentes):
                falla("B", f"{caso} {enfermedad} {linea}/{opcion}: «no aplica» pero cumple los límites")
            continue

        esquema = por_clave[(enfermedad, linea, opcion, r["medicamento_id"])]
        if not all(aplica(c, meses, kg) for c in componentes):
            falla("B", f"{caso} {enfermedad} {linea}/{opcion}: calculado fuera de sus límites de edad o peso")
        estado, dosis, intervalo = esperado(esquema, kg, tfg)
        conteo[r["estado"]] += 1
        if r["estado"] != estado:
            falla("A", f"{caso} {enfermedad} {linea}/{opcion} {r['medicamento_id']}: estado {r['estado']} ≠ {estado}")
            continue
        avisos = set(r["avisos"].split("|")) - {""}

        if estado == "no_recomendado":
            conteo["C4_evitar"] += 1
            continue
        if estado == "sin_calculo":
            continue

        # A. dosis e intervalo
        dosis_motor = float(r["dosis_por_toma"])
        intervalo_motor = num(r["intervalo_horas"])
        conteo["A_dosis_comprobadas"] += 1
        if not igual(dosis_motor, dosis):
            falla("A", f"{caso} {enfermedad} {linea}/{opcion} {r['medicamento_id']}: dosis {dosis_motor} ≠ {dosis:.6g}")
        if (intervalo_motor or None) != (intervalo or None):
            falla("A", f"{caso} {enfermedad} {linea}/{opcion}: intervalo {intervalo_motor} ≠ {intervalo}")

        # A. presentaciones
        tope_toma, tope_dia_ef = topes_de(esquema)
        b = banda_de(esquema["medicamento_id"], tfg) if tfg is not None else None
        if b is not None and num(b["max_por_dosis"]) is not None:
            tope_toma = min(tope_toma, num(b["max_por_dosis"])) if tope_toma else num(b["max_por_dosis"])
        motor_pres = {}
        for item in filter(None, r["presentaciones"].split("|")):
            pid, cant, desvio, adecuada = item.split("=")
            motor_pres[pid] = (float(cant), float(desvio), adecuada == "si")
        esperadas = presentaciones.get((esquema["medicamento_id"], esquema["via"]), [])
        if set(motor_pres) != {x["id"] for x in esperadas}:
            falla("A", f"{caso} {enfermedad} {linea}/{opcion}: presentaciones {sorted(motor_pres)}")
        for pres in esperadas:
            if pres["id"] not in motor_pres:
                continue
            cant, desvio, adecuada, por_unidad = cantidad(pres, dosis, tope_toma)
            conteo["A_presentaciones_comprobadas"] += 1
            m_cant, m_desvio, m_adecuada = motor_pres[pres["id"]]
            if not igual(m_cant + 1, cant + 1) or m_adecuada != adecuada:
                falla("A", f"{caso} {pres['id']}: cantidad {m_cant} ({m_adecuada}) ≠ {cant} ({adecuada})")
            # C6
            if m_adecuada:
                if abs(m_desvio) > DESVIO_MAX + 1e-9:
                    falla("C6", f"{caso} {pres['id']}: desviación {m_desvio:.3f}")
                if tope_toma is not None and m_cant * por_unidad > tope_toma * (1 + 1e-9):
                    falla("C6", f"{caso} {pres['id']}: entrega {m_cant * por_unidad} > tope {tope_toma}")

        # C1. dosis por kilogramo entregada ≤ máximo de la guía
        if esquema["base"] in ("mg_kg_dia", "mg_kg_dosis"):
            maximo = num(esquema["dosis_max"])
            por_kg = (dosis_motor * 24 / intervalo_motor / kg if esquema["base"] == "mg_kg_dia"
                      else dosis_motor / kg)
            if b is not None and num(b["dosis_por_kg"]) is not None:
                maximo = max(maximo, num(b["dosis_por_kg"]) * (24 / intervalo_motor if esquema["base"] == "mg_kg_dia" else 1))
            conteo["C1_comprobadas"] += 1
            if por_kg > maximo * (1 + 1e-9):
                falla("C1", f"{caso} {enfermedad} {linea}/{opcion}: {por_kg:.3f}/kg > {maximo}")
        # C2. topes
        if tope_toma is not None and dosis_motor > tope_toma * (1 + 1e-9):
            falla("C2", f"{caso} {enfermedad} {linea}/{opcion}: {dosis_motor} > tope por toma")
        if tope_dia_ef is not None and intervalo_motor and \
                dosis_motor * 24 / intervalo_motor > tope_dia_ef * (1 + 1e-9):
            falla("C2", f"{caso} {enfermedad} {linea}/{opcion}: diaria > tope por día")
        # C5. sin ningún tope → advertencia
        if esquema["base"] in ("mg_kg_dia", "mg_kg_dosis") and tope_toma is None and tope_dia_ef is None:
            conteo["C5_sin_tope"] += 1
            if "SIN_TOPE_MAXIMO" not in avisos:
                falla("C5", f"{caso} {enfermedad} {linea}/{opcion}: falta SIN_TOPE_MAXIMO")
        # Advertencia de ajuste renal faltante cuando corresponde
        if tfg is not None and tfg < UMBRAL_SIN_DATO and \
                medicamentos[esquema["medicamento_id"]]["ajuste_renal_tipo"] == "sin_dato":
            conteo["sin_dato_renal_esperado"] += 1
            if "SIN_DATO_AJUSTE_RENAL" not in avisos:
                falla("C4", f"{caso} {r['medicamento_id']}: falta SIN_DATO_AJUSTE_RENAL")

        clave_sin_tfg = (p["edad_meses"], p["sexo"], p["z"],
                         enfermedad, linea, opcion, r["medicamento_id"])
        dosis_por[(clave_sin_tfg, p["tfg"])] = (dosis_motor, intervalo_motor)

    # B. toda opción del esquema aparece para cada caso y enfermedad con antibiótico
    for (caso, enfermedad), vistas in vistos.items():
        for (enf, linea, opcion) in opciones:
            if enf == enfermedad and (linea, opcion) not in vistas:
                falla("B", f"{caso} {enfermedad}: falta la opción {linea}/{opcion}")

    # C3. función renal: la dosis no aumenta y el intervalo no se acorta
    for (clave, tfg), (dosis, intervalo) in dosis_por.items():
        if tfg == "":
            continue
        base = dosis_por.get((clave, ""))
        if base is None:
            continue
        conteo["C3_comparaciones"] += 1
        if dosis > base[0] * (1 + 1e-9):
            falla("C3", f"{clave} tfg {tfg}: dosis {dosis} > {base[0]}")
        if intervalo and base[1] and intervalo < base[1]:
            falla("C3", f"{clave} tfg {tfg}: intervalo {intervalo} < {base[1]}")

    # C7. monotonía en el peso (mismas edad, sexo, función renal, esquema)
    grupos = defaultdict(list)
    for (clave, tfg), (dosis, intervalo) in dosis_por.items():
        edad, sexo, z = clave[0], clave[1], clave[2]
        peso = next(float(x["peso_kg"]) for x in [pacientes_por(edad, sexo, z, tfg, pacientes)])
        grupos[(edad, sexo, tfg) + clave[3:]].append((peso, dosis))
    for clave, serie in grupos.items():
        serie.sort()
        conteo["C7_series"] += 1
        for (p1, d1), (p2, d2) in zip(serie, serie[1:]):
            if d2 < d1 * (1 - 1e-9):
                falla("C7", f"{clave}: {p1} kg → {d1}, {p2} kg → {d2}")

    # D. determinismo
    determinismo = None
    if repetida is not None:
        h1 = hashlib.sha256(ruta_res.read_bytes()).hexdigest()
        h2 = hashlib.sha256(repetida.read_bytes()).hexdigest()
        determinismo = h1 == h2
        if not determinismo:
            falla("D", "las dos ejecuciones difieren")

    concordancia = concordancia_publicada(resultados, pacientes)
    descriptivo = describir(resultados, pacientes)

    informe = {
        "escenarios": len(pacientes), "filas": len(resultados),
        "conteos": dict(conteo),
        "fallos": {k: {"n": len(v), "ejemplos": v[:10]} for k, v in sorted(fallos.items())},
        "determinismo": determinismo,
        "concordancia_publicada": concordancia,
        "descriptivo": descriptivo,
    }
    (Path(__file__).parent / "informe.json").write_text(
        json.dumps(informe, ensure_ascii=False, indent=1) + "\n", encoding="utf-8")

    print(f"Escenarios: {len(pacientes)} · filas de resultado: {len(resultados)}")
    for clave, valor in sorted(conteo.items()):
        print(f"  {clave}: {valor}")
    print("Fallos:", {k: len(v) for k, v in fallos.items()} or "ninguno")
    for k, v in sorted(fallos.items()):
        for ejemplo in v[:5]:
            print(f"  [{k}] {ejemplo}")
    print("Determinismo:", determinismo)
    sys.exit(1 if fallos else 0)


_indice = {}


def pacientes_por(edad, sexo, z, tfg, pacientes):
    if not _indice:
        for p in pacientes.values():
            _indice[(p["edad_meses"], p["sexo"], p["z"], p["tfg"])] = p
    return _indice[(edad, sexo, z, tfg)]


# --- E. Concordancia con tablas publicadas ------------------------------------
# AIEPI 2012 (Libro clínico, p. 492): amoxicilina 90 mg/kg/día cada 12 horas,
# mL por dosis según el peso, para tres concentraciones.
AIEPI_AMOX = [((4, 6), {"amox_susp_250": 5, "amox_susp_500": 2.5, "amox_susp_750": 1.5}),
              ((7, 9), {"amox_susp_250": 7, "amox_susp_500": 3.5, "amox_susp_750": 2.5}),
              ((10, 12), {"amox_susp_250": 10, "amox_susp_500": 5, "amox_susp_750": 3.5}),
              ((13, 15), {"amox_susp_250": 12.5, "amox_susp_500": 6.5, "amox_susp_750": 4.5}),
              ((16, 18), {"amox_susp_250": 14.5, "amox_susp_500": 7.5, "amox_susp_750": 5}),
              ((19, 21), {"amox_susp_250": 18, "amox_susp_500": 9, "amox_susp_750": 6})]
# OMS AWaRe 2022 (tabla 50.1): amoxicilina 80-90 mg/kg/día, mg por dosis cada 12 horas.
AWARE_AMOX = [((3, 6), 250), ((6, 10), 375), ((10, 15), 500), ((15, 20), 750), ((20, 999), 1000)]


def concordancia_publicada(resultados, pacientes):
    """Compara la amoxicilina de la otitis (90 mg/kg/día cada 12 h, ACORL 2024)
    con las dosis por peso publicadas, en todos los pacientes modelados con
    función renal no evaluada."""
    filas = [r for r in resultados if r["enfermedad_id"] == "oma" and r["linea"] == "primera"
             and r["opcion"] == "1" and r["estado"] == "calculado" and pacientes[r["caso"]]["tfg"] == ""]
    aiepi, aware = [], []
    for r in filas:
        kg = float(pacientes[r["caso"]]["peso_kg"])
        dosis = float(r["dosis_por_toma"])
        cantidades = {i.split("=")[0]: float(i.split("=")[1]) for i in r["presentaciones"].split("|")}
        for (desde, hasta), ml in AIEPI_AMOX:
            # Las bandas de AIEPI son de kilos enteros (4 a 6 kg incluye 6,9 kg).
            if desde <= kg < hasta + 1:
                for pid, ml_aiepi in ml.items():
                    aiepi.append({"peso_kg": kg, "presentacion": pid, "ml_aiepi": ml_aiepi,
                                  "ml_calculadora": cantidades.get(pid),
                                  "diferencia_ml": round(cantidades.get(pid) - ml_aiepi, 2),
                                  "mg_kg_dia_aiepi": round(ml_aiepi * num(pid.split("_")[-1]) / 5 * 2 / kg, 1)})
        for (desde, hasta), mg in AWARE_AMOX:
            if desde <= kg < hasta:
                aware.append({"peso_kg": kg, "mg_dosis_aware": mg, "mg_dosis_calculadora": round(dosis, 1),
                              "mg_kg_dia_aware": round(mg * 2 / kg, 1)})
    def resumen(lista, campo_a, campo_b):
        if not lista:
            return {}
        rel = [abs(x[campo_a] - x[campo_b]) / x[campo_b] for x in lista if x[campo_b]]
        return {"comparaciones": len(lista),
                "dentro_de_10_por_ciento": sum(1 for v in rel if v <= 0.10),
                "dentro_de_20_por_ciento": sum(1 for v in rel if v <= 0.20),
                "diferencia_relativa_maxima": round(max(rel), 3)}
    mgkg_aiepi = [x["mg_kg_dia_aiepi"] for x in aiepi]
    mgkg_aware = [x["mg_kg_dia_aware"] for x in aware]
    return {
        "aiepi_2012_amoxicilina": {**resumen(aiepi, "ml_calculadora", "ml_aiepi"),
                                   "mg_kg_dia_implicita_min": min(mgkg_aiepi, default=None),
                                   "mg_kg_dia_implicita_max": max(mgkg_aiepi, default=None),
                                   "detalle": aiepi},
        "oms_aware_2022_amoxicilina": {**resumen(aware, "mg_dosis_calculadora", "mg_dosis_aware"),
                                       "mg_kg_dia_implicita_min": min(mgkg_aware, default=None),
                                       "mg_kg_dia_implicita_max": max(mgkg_aware, default=None),
                                       "detalle": aware},
    }


def describir(resultados, pacientes):
    """Datos descriptivos para el informe: presentaciones inadecuadas y dosis
    sin tope en los pacientes más pesados."""
    sin_pres = Counter()
    for r in resultados:
        if r["estado"] == "calculado" and "SIN_PRESENTACION_ADECUADA" in r["avisos"]:
            sin_pres[(r["medicamento_id"], r["via"])] += 1
    pesados = []
    for r in resultados:
        p = pacientes[r["caso"]]
        if r["estado"] == "calculado" and "SIN_TOPE_MAXIMO" in r["avisos"] and p["tfg"] == "" \
                and p["edad_meses"] == "215" and p["sexo"] == "m" and p["z"] == "3" \
                and r["dosis_diaria"]:
            pesados.append({"enfermedad": r["enfermedad_id"], "opcion": f"{r['linea']}/{r['opcion']}",
                            "medicamento": r["medicamento_id"], "peso_kg": float(p["peso_kg"]),
                            "dosis_diaria_mg": round(float(r["dosis_diaria"]))})
    return {"sin_presentacion_adecuada": {f"{m} ({v})": n for (m, v), n in sin_pres.most_common()},
            "sin_tope_en_adolescente_de_mayor_peso": pesados}


if __name__ == "__main__":
    main()
