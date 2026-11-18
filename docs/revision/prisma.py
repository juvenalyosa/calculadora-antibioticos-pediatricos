"""Cuenta el flujo PRISMA de la revisión y escribe docs/revision/flujo_prisma.md.

Uso: python3 docs/revision/prisma.py
"""
import csv
import json
from collections import Counter
from pathlib import Path

AQUI = Path(__file__).resolve().parent


def leer(nombre):
    with open(AQUI / nombre, encoding="utf-8") as archivo:
        return list(csv.DictReader(archivo))


bitacora = json.loads((AQUI / "bitacora_busqueda.json").read_text(encoding="utf-8"))
por_base = Counter()
for e in bitacora["estrategias"]:
    por_base[e["base"]] += e["recuperados"]
identificados = sum(por_base.values())
cribado = leer("cribado.csv")
unicos = len(cribado)
excl_tr = Counter(f["motivo"] for f in cribado if f["decision"] == "excluir")
incluidos_tr = [f for f in cribado if f["decision"] == "incluir"]

texto = []
for g in ("G1", "G2", "G3", "G4"):
    texto += leer(f"texto_completo_{g}.csv")
no_recuperados = [t for t in texto if t["recuperado"] != "si"]
evaluados = [t for t in texto if t["recuperado"] == "si"]
excl_tc = Counter(t["motivo"] for t in evaluados if t["decision"] == "excluir")
incluidos = [t for t in evaluados if t["decision"] == "incluir"]
esquemas = []
for g in ("G1", "G2", "G3", "G4"):
    esquemas += leer(f"esquemas_{g}.csv")
comparacion = Counter(e["comparacion"] for e in esquemas)
maximos = sum(len(leer(f"maximos_{g}.csv")) for g in ("G1", "G2", "G3", "G4"))
ambito = Counter(f["ambito"].split(" ")[0] for f in incluidos_tr)
ambito_final = Counter(t.get("ambito", "").split(" ")[0] for t in incluidos)

MOTIVOS = {"E1": "no es guía", "E2": "sin contenido de tratamiento", "E3": "población fuera del alcance",
           "E4": "infección fuera del alcance", "E5": "versión reemplazada", "E6": "no recuperable como documento",
           "T1": "sin dosis pediátricas utilizables", "T2": "no es guía al leerla",
           "T3": "duplicado o resumen de otra incluida", "T4": "población fuera del alcance"}

lineas = [
    "# Flujo de la revisión (PRISMA-ScR)", "",
    f"Búsqueda del {bitacora['fecha']}, publicaciones de {bitacora['desde'][:4]} a {bitacora['hasta'][:4]}. "
    "Cifras calculadas por `prisma.py` a partir de los archivos de esta carpeta.", "",
    "```",
    "IDENTIFICACIÓN",
    f"  Registros identificados en bases de datos: {identificados}",
] + [f"    {base}: {n}" for base, n in por_base.most_common()] + [
    f"  Duplicados eliminados: {identificados - unicos}",
    "",
    "CRIBADO",
    f"  Registros cribados por título y resumen: {unicos}",
    f"  Registros excluidos: {sum(excl_tr.values())}",
] + [f"    {m} {MOTIVOS[m]}: {n}" for m, n in sorted(excl_tr.items())] + [
    "",
    f"  Documentos buscados a texto completo: {len(incluidos_tr)}",
    f"  Documentos no recuperados (acceso cerrado o sin copia): {len(no_recuperados)}",
    "",
    f"  Documentos evaluados a texto completo: {len(evaluados)}",
    f"  Documentos excluidos: {sum(excl_tc.values())}",
] + [f"    {m} {MOTIVOS.get(m, m)}: {n}" for m, n in sorted(excl_tc.items())] + [
    "",
    "INCLUSIÓN",
    f"  Documentos incluidos en la síntesis: {len(incluidos)}",
    f"  Esquemas extraídos: {len(esquemas)} "
    f"(iguales a la base: {comparacion['igual']}; distintos: {comparacion['distinta']}; nuevos: {comparacion['nueva']})",
    f"  Dosis máximas extraídas: {maximos}",
    "```", "",
    "Ámbito de los documentos que pasaron el cribado por título y resumen: "
    + ", ".join(f"{k} {v}" for k, v in ambito.most_common()) + ".", "",
    "Además de lo anterior, la síntesis usó documentos obtenidos fuera de esta "
    "búsqueda: el corpus inicial de 31 artículos reunido por el equipo clínico, "
    "las guías colombianas encontradas en las revisiones dirigidas previas "
    "(Ministerio de Salud, AIEPI, ACORL, ACIN, consenso de infección urinaria), "
    "las fichas técnicas para el ajuste renal y el libro AWaRe de la OMS.", "",
]
(AQUI / "flujo_prisma.md").write_text("\n".join(lineas), encoding="utf-8")
print("\n".join(lineas))
