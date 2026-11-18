"""Genera la población pediátrica modelada para la prueba exhaustiva.

Uso: python3 docs/validacion/poblacion.py <salida.csv>

Cada paciente virtual sale de los patrones de crecimiento de la OMS (carpeta
oms/), con el método LMS:  valor = M × (1 + L × S × z)^(1/L).

  - Hasta los 60 meses: peso para la edad en el puntaje z pedido y talla
    mediana para la edad.
  - Desde los 61 meses la OMS no publica peso para la edad más allá de los
    10 años: el peso se obtiene del índice de masa corporal para la edad en el
    puntaje z pedido y de la talla mediana (peso = IMC × talla²).

No son pacientes reales.
"""
import csv
import sys
from pathlib import Path

OMS = Path(__file__).resolve().parent / "oms"

EDADES_MESES = [1, 2, 3, 4, 6, 9, 12, 15, 18, 24, 30, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144,
                156, 168, 180, 192, 204, 215]
PUNTAJES_Z = [-3, -2, 0, 2, 3]          # del bajo peso grave a la obesidad
SEXOS = ["m", "f"]
# Función renal: no evaluada, o TFG digitada en cada tramo habitual de ajuste.
ESCENARIOS_TFG = ["", "75", "45", "20", "5"]
DIAS_POR_MES = 30.4375


def lms(L, M, S, z):
    return M * (1 + L * S * z) ** (1 / L) if abs(L) > 1e-9 else M * 2.718281828459045 ** (S * z)


def cargar_0_a_5(nombre):
    """{(sexo, día): (L, M, S)} de los archivos del paquete anthro."""
    tabla = {}
    with open(OMS / nombre, encoding="utf-8") as archivo:
        for fila in csv.DictReader(archivo, delimiter="\t"):
            tabla[("m" if fila["sex"] == "1" else "f", int(fila["age"]))] = (
                float(fila["l"]), float(fila["m"]), float(fila["s"]))
    return tabla


def cargar_5_a_19():
    tabla = {}
    with open(OMS / "oms_2007_5_a_19.csv", encoding="utf-8") as archivo:
        for fila in csv.DictReader(archivo):
            tabla[(fila["indicador"], fila["sexo"], int(fila["mes"]))] = (
                float(fila["L"]), float(fila["M"]), float(fila["S"]))
    return tabla


def poblacion():
    peso, talla, mayores = cargar_0_a_5("weianthro.txt"), cargar_0_a_5("lenanthro.txt"), cargar_5_a_19()
    for meses in EDADES_MESES:
        for sexo in SEXOS:
            if meses <= 60:
                dia = round(meses * DIAS_POR_MES)
                talla_cm = lms(*talla[(sexo, dia)], 0)
            else:
                talla_cm = lms(*mayores[("talla_edad", sexo, meses)], 0)
            for z in PUNTAJES_Z:
                if meses <= 60:
                    peso_kg = lms(*peso[(sexo, dia)], z)
                else:
                    peso_kg = lms(*mayores[("imc_edad", sexo, meses)], z) * (talla_cm / 100) ** 2
                yield {"edad_meses": meses, "sexo": sexo, "z": z,
                       "peso_kg": round(peso_kg, 2), "talla_cm": round(talla_cm, 1)}


def main():
    salida = Path(sys.argv[1])
    filas = 0
    with open(salida, "w", newline="", encoding="utf-8") as archivo:
        w = csv.writer(archivo, lineterminator="\n")
        w.writerow(["caso", "edad_meses", "peso_kg", "talla_cm", "sexo", "tfg", "enfermedad_id", "z"])
        for p in poblacion():
            for tfg in ESCENARIOS_TFG:
                caso = f"{p['edad_meses']}m-{p['sexo']}-z{p['z']:+d}-tfg{tfg or 'ne'}"
                w.writerow([caso, p["edad_meses"], p["peso_kg"], p["talla_cm"], p["sexo"], tfg, "", p["z"]])
                filas += 1
    print(f"{filas} escenarios en {salida}")


if __name__ == "__main__":
    main()
