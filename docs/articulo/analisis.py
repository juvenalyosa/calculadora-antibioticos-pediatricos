"""Análisis cuantitativos del manuscrito. Uso: python3 docs/articulo/analisis.py

Lee las hojas de data/fuente y la matriz de verificación y escribe
docs/articulo/resultados.json. No usa datos de pacientes.
"""
import csv
import json
import math
import re
from collections import Counter
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent.parent
FUENTE = REPO / "data" / "fuente"


def leer(ruta):
    with open(ruta, encoding="utf-8") as archivo:
        return list(csv.DictReader(archivo))


esquemas = leer(FUENTE / "esquemas.csv")
enfermedades = leer(FUENTE / "enfermedades.csv")
medicamentos = leer(FUENTE / "medicamentos.csv")
fuentes = leer(FUENTE / "fuentes.csv")
presentaciones = leer(FUENTE / "presentaciones.csv")
bandas = leer(FUENTE / "ajuste_renal.csv")
matriz = leer(REPO / "tests" / "data" / "matriz_alfa.csv")

LATAM = {"sochinf_faringoamigdalitis_2019", "sadi_neumonia_complicada_2024", "sap_piel_2014", "sochinf_itu_2022"}
COLOMBIA = {"acorl_oma_2024", "acorl_faringoamigdalitis_2024", "acorl_rinosinusitis_2024",
            "acorl_otitis_externa_2024", "minsalud_neumonia_2014", "minsalud_desparasitacion_2013",
            "aiepi_2012", "acin_piel_2023", "consenso_colombiano_itu_2023"}
CORPUS = {"consenso_italiano_oma_2025", "sip_oma_2019", "aafp_oma_2019",
          "consenso_italiano_faringitis_2024", "finlandia_vri_2023", "china_micoplasma_2023",
          "francia_gastroenteritis_2017", "codepeh_ome_2019"}


def origen(fuente_id):
    if fuente_id in COLOMBIA:
        return "colombiana"
    if fuente_id in LATAM:
        return "latinoamericana"
    if fuente_id == "oms_aware_2022":
        return "OMS"
    return "corpus" if fuente_id in CORPUS else "internacional"


r = {}
r["base"] = {
    "enfermedades": len(enfermedades),
    "con_antibiotico": sum(e["requiere_antibiotico"] == "si" for e in enfermedades),
    "sin_antibiotico": sum(e["requiere_antibiotico"] == "no" for e in enfermedades),
    "grupos": dict(Counter(e["grupo"] for e in enfermedades)),
    "esquemas": len(esquemas),
    "medicamentos": len(medicamentos),
    "fuentes": len(fuentes),
    "presentaciones": len(presentaciones),
    "bandas_renales": len(bandas),
    "ajuste_renal_tipo": dict(Counter(m["ajuste_renal_tipo"] for m in medicamentos)),
    "estado_esquemas": dict(Counter(e["estado"] for e in esquemas)),
}
r["esquemas_por_origen_de_la_fuente"] = dict(Counter(origen(e["fuente_id"]) for e in esquemas))
r["fuentes_citadas_por_esquemas"] = len({e["fuente_id"] for e in esquemas})
r["fuentes_del_corpus_usadas"] = sorted({f for f in CORPUS if any(e["fuente_id"] == f for e in esquemas)}
                                        | {e["fuente_id"] for e in enfermedades if e["fuente_id"] in CORPUS})
r["enfermedades_con_fuente_principal"] = dict(Counter(origen(e["fuente_id"]) for e in enfermedades))
r["esquemas_por_linea"] = dict(Counter(e["linea"] for e in esquemas))
r["esquemas_por_via"] = dict(Counter(e["via"] for e in esquemas))
r["esquemas_por_base"] = dict(Counter(e["base"] for e in esquemas))

por_peso = [e for e in esquemas if e["base"] in ("mg_kg_dia", "mg_kg_dosis")]
con_tope = [e for e in por_peso if e["max_por_dosis"] or e["max_por_dia"]]
PRESTADO = re.compile(r"[Tt]ope[^.]*(tomado|se aplica|ficha|adulto|banda de peso|PIDS|AAP|FDA)")
prestado = [e for e in con_tope if PRESTADO.search(e["nota"])]
r["topes"] = {
    "esquemas_por_peso": len(por_peso),
    "con_tope": len(con_tope),
    "sin_tope": len(por_peso) - len(con_tope),
    "tope_de_otra_fuente_segun_la_nota": len(prestado),
    "tope_de_la_propia_fuente": len(con_tope) - len(prestado),
}
topes_med = leer(FUENTE / "topes_medicamento.csv")
tope_med = {(t["medicamento_id"], t["via"]): t for t in topes_med}
con_tope_med = [e for e in por_peso if not (e["max_por_dosis"] or e["max_por_dia"]) and (e["medicamento_id"], e["via"]) in tope_med]
r["topes"]["tope_por_medicamento_disponible_para_esquemas_sin_tope"] = len(con_tope_med)
r["topes"]["sin_ningun_tope"] = len(por_peso) - len(con_tope) - len(con_tope_med)
r["topes"]["topes_por_medicamento"] = len(topes_med)
r["topes"]["medicamentos_con_tope"] = len({t["medicamento_id"] for t in topes_med})
r["esquemas_con_rango"] = sum(1 for e in por_peso if e["dosis_min"] != e["dosis_max"])
r["esquemas_con_limite_de_edad"] = sum(1 for e in esquemas if e["edad_min_meses"] or e["edad_max_meses"])

r["matriz"] = {
    "filas": len(matriz),
    "funcion_renal_normal": sum(1 for f in matriz if not f["tfg"]),
    "con_ajuste_renal": sum(1 for f in matriz if f["tfg"]),
    "con_presentacion_comprobada": sum(1 for f in matriz if f["presentacion_id"]),
    "por_paciente": dict(Counter("lactante (12 meses, 10 kg)" if f["edad_meses"] == "12"
                                 else "adolescente (156 meses, 50 kg)" for f in matriz)),
    "enfermedades_cubiertas": len({f["enfermedad_id"] for f in matriz}),
}


# --- Comprobación aritmética de medidas de efecto publicadas en el corpus ---
def diferencia_de_riesgos(a, n1, b, n2):
    p1, p2 = a / n1, b / n2
    ee = math.sqrt(p1 * (1 - p1) / n1 + p2 * (1 - p2) / n2)
    d = p1 - p2
    return {"p1": round(100 * p1, 1), "p2": round(100 * p2, 1), "dr": round(100 * d, 1),
            "ic95_wald": [round(100 * (d - 1.96 * ee), 1), round(100 * (d + 1.96 * ee), 1)]}


def riesgo_relativo(a, n1, b, n2):
    rr = (a / n1) / (b / n2)
    ee = math.sqrt(1 / a - 1 / n1 + 1 / b - 1 / n2)
    return {"rr": round(rr, 2), "ic95": [round(math.exp(math.log(rr) - 1.96 * ee), 2),
                                         round(math.exp(math.log(rr) + 1.96 * ee), 2)]}


r["recalculo"] = {
    "skoog_2019_curacion_clinica_por_protocolo_5d_vs_10d": diferencia_de_riesgos(181, 202, 182, 195),
    "skoog_2019_erradicacion_5d_vs_10d": diferencia_de_riesgos(156, 194, 165, 182),
    "lin_2021_cefadroxilo_vs_flucloxacilina": riesgo_relativo(17, 21, 18, 20),
    "lin_2021_cefdinir_vs_cefalexina": riesgo_relativo(25, 32, 32, 42),
    "lin_2021_cefditoren_vs_cefaclor": riesgo_relativo(24, 46, 21, 47),
    "kuitunen_2023_retratamiento_corto_vs_largo": riesgo_relativo(53, 641, 50, 647),
}

# --- Concordancia entre guías: dosis diaria recomendada de amoxicilina oral (mg/kg/día) ---
# (mínimo, máximo) tal como lo escribe cada documento; ver la tabla 3 del manuscrito.
CONCORDANCIA = {
    "otitis media aguda": {
        "ACORL 2024 (Colombia)": (90, 90), "AIEPI 2012 (Colombia)": (90, 90),
        "Consenso italiano 2025": (80, 90), "SIP 2019 (Italia)": (80, 90), "AAP 2013 (EE. UU.)": (80, 90),
        "AAFP 2019 (EE. UU.)": (80, 90), "OMS AWaRe 2022": (80, 90),
        "Sakulchit y Goldman 2017 (Canadá)": (45, 90), "Rothman 2018 (Israel)": (60, 80),
    },
    "neumonía adquirida en la comunidad, ambulatoria": {
        "Guía 42 del Ministerio 2014 (Colombia)": (90, 90), "AIEPI 2012 (Colombia)": (90, 90),
        "PIDS/IDSA 2011 (EE. UU.)": (90, 90), "OMS 2014": (80, 80),
        "Guía finlandesa 2023, dosis habitual": (40, 45), "Guía finlandesa 2023, dosis alta": (80, 90),
    },
    "faringoamigdalitis estreptocócica": {
        "ACORL 2024 (Colombia)": (50, 50), "IDSA 2012 (EE. UU.)": (50, 50),
        "Consenso italiano 2024": (50, 50), "OMS AWaRe 2022": (80, 90),
    },
}
r["concordancia_amoxicilina"] = {}
for indicacion, guias in CONCORDANCIA.items():
    minimo = min(v[0] for v in guias.values())
    maximo = max(v[1] for v in guias.values())
    r["concordancia_amoxicilina"][indicacion] = {
        "documentos": len(guias), "minimo": minimo, "maximo": maximo,
        "razon_max_min": round(maximo / minimo, 2),
        "documentos_que_incluyen_90": sum(1 for v in guias.values() if v[0] <= 90 <= v[1]),
        "detalle": {k: list(v) for k, v in guias.items()},
    }

(Path(__file__).parent / "resultados.json").write_text(
    json.dumps(r, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
print(json.dumps({k: v for k, v in r.items() if k != "concordancia_amoxicilina"}, ensure_ascii=False, indent=1))
print({k: {x: v[x] for x in ("documentos", "minimo", "maximo", "razon_max_min")} for k, v in r["concordancia_amoxicilina"].items()})
