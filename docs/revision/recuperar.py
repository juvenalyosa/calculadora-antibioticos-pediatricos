"""Recupera el texto completo de acceso abierto de los documentos incluidos.

Uso: python3 docs/revision/recuperar.py <carpeta_destino>

Para cada documento incluido en docs/revision/cribado.csv busca, en este orden:
  1. PubMed Central (XML de texto completo, si tiene PMCID);
  2. la copia de acceso abierto que registra OpenAlex (PDF), por DOI.
Solo descarga lo que las propias bases declaran de acceso abierto. Escribe
docs/revision/recuperacion.csv con el resultado de cada documento. Los textos
quedan en <carpeta_destino> (fuera del control de versiones: tienen derechos
de autor).
"""
import csv
import json
import sys
import time
import urllib.parse
import urllib.request
from pathlib import Path

AGENTE = "literature-retrieval/1.0"
CARPETA = Path(__file__).resolve().parent


def obtener(url, binario=False, intentos=3):
    ultimo = None
    for intento in range(intentos):
        try:
            peticion = urllib.request.Request(url, headers={"User-Agent": AGENTE})
            respuesta = urllib.request.urlopen(peticion, timeout=60)
            datos = respuesta.read()
            return (datos, respuesta.headers.get("Content-Type", "")) if binario else datos.decode("utf-8", "replace")
        except Exception as error:
            ultimo = error
            time.sleep(2 + 2 * intento)
    raise ultimo


def main():
    destino = Path(sys.argv[1])
    destino.mkdir(parents=True, exist_ok=True)
    incluidos = [f for f in csv.DictReader(open(CARPETA / "cribado.csv", encoding="utf-8"))
                 if f["decision"] == "incluir"]
    salida = []
    for f in incluidos:
        nombre = f["clave"].replace(":", "_").replace("/", "_")[:120]
        estado, archivo, url = "no_disponible", "", ""
        # 1. PubMed Central
        pmcid = f["pmcid"]
        if not pmcid and f["pmid"]:
            try:
                conv = json.loads(obtener("https://www.ncbi.nlm.nih.gov/pmc/utils/idconv/v1.0/?format=json&ids=" + f["pmid"]))
                pmcid = conv.get("records", [{}])[0].get("pmcid", "")
            except Exception:
                pass
        if pmcid:
            try:
                xml = obtener("https://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?db=pmc&retmode=xml&id=" + pmcid.replace("PMC", ""))
                if "<body" in xml:
                    archivo = f"{nombre}.xml"
                    (destino / archivo).write_text(xml, encoding="utf-8")
                    estado, url = "pmc", f"https://pmc.ncbi.nlm.nih.gov/articles/{pmcid}/"
            except Exception:
                pass
            time.sleep(0.4)
        # 2. OpenAlex: copia de acceso abierto
        if estado == "no_disponible" and f["doi"]:
            try:
                w = json.loads(obtener("https://api.openalex.org/works/doi:" + urllib.parse.quote(f["doi"])))
                oa = w.get("open_access", {})
                candidatos = [loc.get("pdf_url") for loc in [w.get("best_oa_location") or {}] + (w.get("locations") or [])
                              if loc and loc.get("is_oa") and loc.get("pdf_url")]
                for enlace in dict.fromkeys(candidatos):
                    try:
                        datos, tipo = obtener(enlace, binario=True, intentos=2)
                    except Exception:
                        continue
                    if datos[:4] == b"%PDF":
                        archivo = f"{nombre}.pdf"
                        (destino / archivo).write_bytes(datos)
                        estado, url = "acceso_abierto", enlace
                        break
                if estado == "no_disponible":
                    estado = "oa_sin_pdf" if oa.get("is_oa") else "cerrado"
                    url = oa.get("oa_url") or ""
            except Exception:
                estado = "doi_no_resuelto"
            time.sleep(0.3)
        salida.append({"clave": f["clave"], "grupo": f["grupo"], "ambito": f["ambito"], "anio": f["anio"],
                       "titulo": f["titulo"][:200], "estado": estado, "archivo": archivo, "url": url})
        print(f"{estado:16s} {f['clave'][:60]}", flush=True)
    with open(CARPETA / "recuperacion.csv", "w", newline="", encoding="utf-8") as a:
        w = csv.DictWriter(a, fieldnames=list(salida[0]), lineterminator="\n")
        w.writeheader()
        w.writerows(salida)


if __name__ == "__main__":
    main()
