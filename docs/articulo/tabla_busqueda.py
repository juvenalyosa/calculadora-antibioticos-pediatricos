"""Genera las tablas de la búsqueda para el material suplementario.

Uso: python3 docs/articulo/tabla_busqueda.py

Lee docs/revision/bitacora_busqueda.json y escribe en docs/articulo/latex/datos/:
  busqueda_resumen.tex     resultados por grupo de infecciones y estrategia
  busqueda_ecuaciones.tex  las 44 ecuaciones, tal como se ejecutaron
"""
import json
from collections import defaultdict
from pathlib import Path

RAIZ = Path(__file__).resolve().parent.parent
ESTRATEGIAS = ["pubmed_guias", "pubmed_region", "openalex", "doaj"]
NOMBRES = {"pubmed_guias": "PubMed, guías", "pubmed_region": "PubMed, región",
           "openalex": "OpenAlex", "doaj": "DOAJ"}
TEMAS = {"otitis": "Otitis", "faringitis": "Faringoamigdalitis", "sinusitis": "Rinosinusitis",
         "neumonia": "Neumonía", "respiratorias_virales": "Bronquiolitis, laringitis, bronquitis y tos ferina",
         "urinaria": "Infección urinaria", "diarrea": "Diarrea aguda y disentería",
         "piel": "Piel, tejidos blandos y mordeduras", "parasitos": "Parásitos intestinales",
         "boca_ojos": "Infección odontogénica y conjuntivitis",
         "fiebre_grave": "Fiebre sin foco, sepsis y meningitis"}
ESPECIALES = {"\\": r"\textbackslash{}", "&": r"\&", "%": r"\%", "#": r"\#",
              "_": r"\_", "$": r"\$", "{": r"\{", "}": r"\}", "~": r"\textasciitilde{}",
              "^": r"\textasciicircum{}"}


def escapar(texto):
    return "".join(ESPECIALES.get(c, c) for c in str(texto))


def main():
    bitacora = json.loads((RAIZ / "revision" / "bitacora_busqueda.json").read_text(encoding="utf-8"))
    consultas = bitacora["estrategias"]
    por_tema = defaultdict(dict)
    for c in consultas:
        por_tema[c["tema"]][c["estrategia"]] = c["recuperados"]
    temas = list(dict.fromkeys(c["tema"] for c in consultas))

    filas = []
    totales = defaultdict(int)
    for tema in temas:
        valores = [por_tema[tema].get(e, 0) for e in ESTRATEGIAS]
        for e, v in zip(ESTRATEGIAS, valores):
            totales[e] += v
        filas.append(" & ".join([escapar(TEMAS.get(tema, tema))] + [str(v) for v in valores]
                                + [str(sum(valores))]) + r" \\")
    filas.append(r"\midrule")
    total = sum(totales.values())
    filas.append(" & ".join(["Total"] + [str(totales[e]) for e in ESTRATEGIAS] + [str(total)]) + r" \\")

    datos = RAIZ / "articulo" / "latex" / "datos"
    (datos / "busqueda_resumen.tex").write_text(
        "% Generado por docs/articulo/tabla_busqueda.py; no editar a mano.\n" + "\n".join(filas) + "\n",
        encoding="utf-8")

    ecuaciones = []
    for c in consultas:
        ecuaciones.append(
            escapar(TEMAS.get(c["tema"], c["tema"])) + " & " + NOMBRES.get(c["estrategia"], escapar(c["estrategia"]))
            + " & " + str(c["recuperados"])
            + r" & {\ttfamily\scriptsize\raggedright " + escapar(c["ecuacion"]) + r"} \\")
    (datos / "busqueda_ecuaciones.tex").write_text(
        "% Generado por docs/articulo/tabla_busqueda.py; no editar a mano.\n"
        + "\n\\addlinespace\n".join(ecuaciones) + "\n", encoding="utf-8")
    print(f"{len(consultas)} consultas, {total} registros; fecha {bitacora['fecha']}")


if __name__ == "__main__":
    main()
