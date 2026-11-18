"""Variabilidad entre guías de la dosis diaria recomendada.

Uso: python3 docs/articulo/concordancia.py

Lee los esquemas extraídos en la revisión (docs/revision/esquemas_G*.csv) y,
para cada par enfermedad–medicamento por vía oral recomendado por dos o más
documentos distintos, calcula el intervalo de la dosis diaria en mg/kg/día
(los esquemas en mg/kg/dosis se convierten con su intervalo). La penicilina V se
excluye porque unas guías la dosifican en mg y otras en UI. Escribe
docs/articulo/concordancia.csv y concordancia.json.
"""
import csv
import json
from collections import defaultdict
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent.parent


def num(texto):
    texto = (texto or "").strip().replace(",", ".")
    try:
        return float(texto) if texto else None
    except ValueError:
        return None


grupos = defaultdict(list)
descartadas = 0
for g in ("G1", "G2", "G3", "G4"):
    with open(REPO / "docs" / "revision" / f"esquemas_{g}.csv", encoding="utf-8") as archivo:
        for e in csv.DictReader(archivo):
            if e["via"] != "oral" or e["base"] not in ("mg_kg_dia", "mg_kg_dosis"):
                continue
            # La penicilina V se dosifica en mg en unas guías y en UI en otras: no se compara.
            if e["medicamento_id"] in ("penicilina_v", "penicilina_g"):
                continue
            valores = [v for v in (num(e["dosis_min"]), num(e["dosis_usual"]), num(e["dosis_max"])) if v]
            if not valores:
                descartadas += 1
                continue
            minimo, maximo = min(valores), max(valores)
            if e["base"] == "mg_kg_dosis":
                intervalo = num(e["intervalo_horas"])
                if not intervalo:
                    descartadas += 1
                    continue
                minimo, maximo = minimo * 24 / intervalo, maximo * 24 / intervalo
            grupos[(e["enfermedad_id"], e["medicamento_id"])].append((e["clave"], minimo, maximo))

filas = []
for (enfermedad, medicamento), valores in sorted(grupos.items()):
    documentos = {c for c, _, _ in valores}
    if len(documentos) < 2:
        continue
    minimo = min(v[1] for v in valores)
    maximo = max(v[2] for v in valores)
    filas.append({"enfermedad": enfermedad, "medicamento": medicamento, "documentos": len(documentos),
                  "esquemas": len(valores), "mg_kg_dia_min": round(minimo, 1), "mg_kg_dia_max": round(maximo, 1),
                  "razon_max_min": round(maximo / minimo, 2) if minimo else None})
filas.sort(key=lambda f: (-(f["razon_max_min"] or 0), -f["documentos"]))
with open(REPO / "docs" / "articulo" / "concordancia.csv", "w", newline="", encoding="utf-8") as archivo:
    w = csv.DictWriter(archivo, fieldnames=list(filas[0]), lineterminator="\n")
    w.writeheader()
    w.writerows(filas)
razones = [f["razon_max_min"] for f in filas if f["razon_max_min"]]
resumen = {"pares_con_dos_o_mas_documentos": len(filas), "filas_sin_dosis_o_intervalo": descartadas,
           "pares_con_razon_1": sum(1 for r in razones if r == 1.0),
           "pares_con_razon_mayor_o_igual_2": sum(1 for r in razones if r >= 2.0),
           "mediana_razon": sorted(razones)[len(razones) // 2] if razones else None,
           "pares": filas}
(REPO / "docs" / "articulo" / "concordancia.json").write_text(
    json.dumps(resumen, ensure_ascii=False, indent=1) + "\n", encoding="utf-8")
print({k: v for k, v in resumen.items() if k != "pares"})
for f in filas: print(f)
