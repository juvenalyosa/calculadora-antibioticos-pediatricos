"""Búsqueda bibliográfica reproducible del protocolo (docs/revision/protocolo.md).

Uso:
  python3 docs/revision/buscar.py contar            # solo cuenta resultados por estrategia
  python3 docs/revision/buscar.py ejecutar <carpeta> # descarga los registros a <carpeta>

Consulta PubMed (E-utilities), OpenAlex y DOAJ con las ecuaciones definidas
aquí. No usa claves ni datos personales. Guarda un registro por estrategia y
un archivo `registros.csv` sin duplicados.
"""
import csv
import json
import re
import sys
import time
import urllib.parse
import urllib.request
from datetime import date
from pathlib import Path

DESDE, HASTA = "2010/01/01", "2026/11/18"
AGENTE = "literature-search/1.0"

# --- Bloques de la ecuación de PubMed ---------------------------------------
NINOS = ('(child*[tiab] OR pediatric*[tiab] OR paediatric*[tiab] OR infant*[tiab] OR '
         'adolescen*[tiab] OR "Child"[Mesh] OR "Infant"[Mesh] OR "Adolescent"[Mesh])')
GUIA = ('(Guideline[pt] OR "Practice Guideline"[pt] OR "Consensus Development Conference"[pt] OR '
        'guideline*[ti] OR consensus[ti] OR recommendation*[ti] OR "position statement"[ti] OR '
        'guia[ti] OR consenso[ti] OR recomendaciones[ti])')
TRATAMIENTO = ('("Anti-Bacterial Agents"[Mesh] OR antibiotic*[tiab] OR antimicrobial*[tiab] OR '
               'amoxicillin[tiab] OR treatment[ti] OR management[ti])')
REGION = ('(Colombia[tiab] OR Colombia[ad] OR Colombian[tiab] OR "Latin America"[tiab] OR '
          '"Latin American"[tiab] OR "Colombia"[Mesh] OR "Latin America"[Mesh])')
FILTROS = f'("{DESDE}"[dp] : "{HASTA}"[dp]) AND (english[la] OR spanish[la])'

# Tema: (clave, nombre, términos en PubMed, términos en español para OpenAlex y DOAJ)
TEMAS = [
    ("otitis", "Otitis media aguda, con efusión, externa y mastoiditis",
     '("otitis media"[tiab] OR "Otitis Media"[Mesh] OR "otitis externa"[tiab] OR mastoiditis[tiab])',
     "otitis"),
    ("faringitis", "Faringoamigdalitis",
     '(pharyngitis[tiab] OR tonsillitis[tiab] OR pharyngotonsillitis[tiab] OR tonsillopharyngitis[tiab] '
     'OR "sore throat"[tiab] OR "Pharyngitis"[Mesh] OR "scarlet fever"[tiab])',
     "faringoamigdalitis OR faringitis OR amigdalitis"),
    ("sinusitis", "Rinosinusitis",
     '(sinusitis[tiab] OR rhinosinusitis[tiab] OR "Sinusitis"[Mesh])', "rinosinusitis OR sinusitis"),
    ("neumonia", "Neumonía adquirida en la comunidad",
     '("community-acquired pneumonia"[tiab] OR "community acquired pneumonia"[tiab] OR '
     '("Pneumonia"[Mesh] AND "Community-Acquired Infections"[Mesh]) OR "Mycoplasma pneumoniae"[tiab])',
     "neumonía adquirida en la comunidad"),
    ("respiratorias_virales", "Bronquiolitis, laringitis, bronquitis y tos ferina",
     '(bronchiolitis[tiab] OR croup[tiab] OR laryngotracheitis[tiab] OR "acute bronchitis"[tiab] OR '
     'pertussis[tiab] OR "whooping cough"[tiab])', "bronquiolitis OR tos ferina OR laringotraqueítis"),
    ("urinaria", "Infección urinaria",
     '("urinary tract infection"[tiab] OR "urinary tract infections"[tiab] OR pyelonephritis[tiab] OR '
     'cystitis[tiab] OR "Urinary Tract Infections"[Mesh])', "infección urinaria OR infección de vías urinarias"),
    ("diarrea", "Diarrea aguda y disentería",
     '(gastroenteritis[tiab] OR "acute diarrhea"[tiab] OR "acute diarrhoea"[tiab] OR dysentery[tiab] OR '
     'shigell*[tiab] OR "Clostridioides difficile"[tiab] OR "Clostridium difficile"[tiab] OR '
     '"Gastroenteritis"[Mesh])', "enfermedad diarreica aguda OR diarrea aguda OR gastroenteritis"),
    ("piel", "Infecciones de piel y tejidos blandos, y mordeduras",
     '("skin and soft tissue infection"[tiab] OR "skin and soft tissue infections"[tiab] OR '
     'cellulitis[tiab] OR impetigo[tiab] OR "skin abscess"[tiab] OR erysipelas[tiab] OR '
     '"Staphylococcal Skin Infections"[Mesh] OR "bite wound"[tiab] OR "Bites and Stings"[Mesh])',
     "infecciones de piel y tejidos blandos OR impétigo OR celulitis"),
    ("parasitos", "Parásitos intestinales",
     '(giardia*[tiab] OR amebiasis[tiab] OR amoebiasis[tiab] OR "soil-transmitted helminth"[tiab] OR '
     'deworming[tiab] OR "Intestinal Diseases, Parasitic"[Mesh])', "parasitosis intestinal OR geohelmintiasis"),
    ("boca_ojos", "Infección odontogénica y conjuntivitis",
     '("odontogenic infection"[tiab] OR "odontogenic infections"[tiab] OR "dental abscess"[tiab] OR '
     '"bacterial conjunctivitis"[tiab] OR "Conjunctivitis, Bacterial"[Mesh])',
     "infección odontogénica OR conjuntivitis bacteriana"),
    ("fiebre_grave", "Fiebre sin foco, sepsis y meningitis",
     '("fever without source"[tiab] OR "fever without a source"[tiab] OR "febrile infant"[tiab] OR '
     '"febrile infants"[tiab] OR "bacterial meningitis"[tiab] OR "Meningitis, Bacterial"[Mesh])',
     "fiebre sin foco OR meningitis bacteriana"),
]


def obtener(url, intentos=5):
    ultimo = None
    for intento in range(intentos):
        try:
            peticion = urllib.request.Request(url, headers={"User-Agent": AGENTE})
            return urllib.request.urlopen(peticion, timeout=60).read().decode("utf-8")
        except Exception as error:  # red inestable o límite de frecuencia: reintentar
            ultimo = error
            time.sleep(3 + 3 * intento)
    raise ultimo


def ecuaciones_pubmed(terminos):
    return {
        "pubmed_guias": f"{terminos} AND {NINOS} AND {GUIA} AND {FILTROS}",
        "pubmed_region": f"{terminos} AND {NINOS} AND {REGION} AND {TRATAMIENTO} AND {FILTROS}",
    }


def pubmed_ids(ecuacion):
    base = "https://eutils.ncbi.nlm.nih.gov/entrez/eutils/esearch.fcgi?db=pubmed&retmode=json&retmax=2000&term="
    resultado = json.loads(obtener(base + urllib.parse.quote(ecuacion)))["esearchresult"]
    time.sleep(0.4)
    return int(resultado["count"]), resultado["idlist"]


def pubmed_registros(ids):
    registros = []
    for inicio in range(0, len(ids), 150):
        lote = ids[inicio:inicio + 150]
        datos = json.loads(obtener("https://eutils.ncbi.nlm.nih.gov/entrez/eutils/esummary.fcgi?db=pubmed"
                                   "&retmode=json&id=" + ",".join(lote)))["result"]
        time.sleep(0.4)
        for pmid in lote:
            r = datos.get(pmid, {})
            ident = {a["idtype"]: a["value"] for a in r.get("articleids", [])}
            registros.append({
                "pmid": pmid, "doi": ident.get("doi", ""), "pmcid": ident.get("pmc", ""),
                "titulo": r.get("title", ""), "revista": r.get("source", ""),
                "anio": (r.get("pubdate", "") or "")[:4], "idioma": ",".join(r.get("lang", [])),
                "tipos": "; ".join(r.get("pubtype", [])),
                "autores": ", ".join(a["name"] for a in r.get("authors", [])[:3]),
            })
    return registros


def openalex(terminos_es):
    """Trabajos con autores de instituciones colombianas, por título y resumen."""
    filtro = ("title_and_abstract.search:" + urllib.parse.quote(f"({terminos_es}) AND (niños OR pediatría OR pediátrico OR infantil OR children)")
              + ",authorships.institutions.country_code:CO,from_publication_date:2010-01-01,type:article")
    url = f"https://api.openalex.org/works?filter={filtro}&per-page=200&select=id,doi,title,publication_year,language,primary_location,type,ids"
    datos = json.loads(obtener(url))
    time.sleep(0.3)
    registros = []
    for w in datos.get("results", []):
        fuente = ((w.get("primary_location") or {}).get("source") or {}).get("display_name", "")
        registros.append({
            "pmid": (w.get("ids", {}).get("pmid", "") or "").rsplit("/", 1)[-1],
            "doi": (w.get("doi") or "").replace("https://doi.org/", ""), "pmcid": "",
            "titulo": w.get("title") or "", "revista": fuente, "anio": str(w.get("publication_year") or ""),
            "idioma": w.get("language") or "", "tipos": w.get("type") or "", "autores": "",
        })
    return int(datos.get("meta", {}).get("count", 0)), registros


def doaj(terminos_es):
    consulta = f"({terminos_es}) AND (niños OR pediatría OR pediátrico) AND (guía OR consenso OR recomendaciones OR tratamiento)"
    url = "https://doaj.org/api/search/articles/" + urllib.parse.quote(consulta) + "?pageSize=100"
    datos = json.loads(obtener(url))
    time.sleep(0.5)
    registros = []
    for item in datos.get("results", []):
        b = item.get("bibjson", {})
        doi = next((i["id"] for i in b.get("identifier", []) if i.get("type") == "doi"), "")
        anio = b.get("year", "")
        if anio and int(anio) < 2010:
            continue
        registros.append({
            "pmid": "", "doi": doi, "pmcid": "", "titulo": b.get("title", ""),
            "revista": b.get("journal", {}).get("title", ""), "anio": anio,
            "idioma": ",".join(b.get("journal", {}).get("language", [])), "tipos": "article",
            "autores": ", ".join(a.get("name", "") for a in b.get("author", [])[:3]),
        })
    return int(datos.get("total", 0)), registros


def clave_de(registro):
    if registro["doi"]:
        return "doi:" + registro["doi"].lower()
    if registro["pmid"]:
        return "pmid:" + registro["pmid"]
    return "titulo:" + re.sub(r"[^a-z0-9]", "", registro["titulo"].lower())[:80]


def main():
    modo = sys.argv[1] if len(sys.argv) > 1 else "contar"
    carpeta = Path(sys.argv[2]) if len(sys.argv) > 2 else None
    bitacora, todos = [], {}
    for clave, nombre, terminos, terminos_es in TEMAS:
        for estrategia, ecuacion in ecuaciones_pubmed(terminos).items():
            total, ids = pubmed_ids(ecuacion)
            bitacora.append({"tema": clave, "base": "PubMed", "estrategia": estrategia,
                             "ecuacion": ecuacion, "resultados": total, "recuperados": len(ids)})
            print(f"{clave:22s} {estrategia:14s} {total}")
            if modo == "ejecutar":
                for r in pubmed_registros(ids):
                    todos.setdefault(clave_de(r), {**r, "temas": set(), "origen": set()})
                    todos[clave_de(r)]["temas"].add(clave)
                    todos[clave_de(r)]["origen"].add(estrategia)
        for base, funcion in (("OpenAlex", openalex), ("DOAJ", doaj)):
            try:
                total, registros = funcion(terminos_es)
            except Exception as error:
                total, registros = -1, []
                print(f"{clave:22s} {base:14s} ERROR {str(error)[:80]}")
            bitacora.append({"tema": clave, "base": base, "estrategia": base.lower(),
                             "ecuacion": terminos_es, "resultados": total, "recuperados": len(registros)})
            print(f"{clave:22s} {base:14s} {total} (recuperados {len(registros)})")
            if modo == "ejecutar":
                for r in registros:
                    todos.setdefault(clave_de(r), {**r, "temas": set(), "origen": set()})
                    todos[clave_de(r)]["temas"].add(clave)
                    todos[clave_de(r)]["origen"].add(base.lower())
    if modo != "ejecutar":
        return
    carpeta.mkdir(parents=True, exist_ok=True)
    (carpeta / "bitacora_busqueda.json").write_text(
        json.dumps({"fecha": date.today().isoformat(), "desde": DESDE, "hasta": HASTA,
                    "estrategias": bitacora}, ensure_ascii=False, indent=1), encoding="utf-8")
    campos = ["clave", "temas", "origen", "anio", "titulo", "revista", "tipos", "idioma", "autores",
              "pmid", "pmcid", "doi"]
    with open(carpeta / "registros.csv", "w", newline="", encoding="utf-8") as archivo:
        salida = csv.DictWriter(archivo, fieldnames=campos, lineterminator="\n")
        salida.writeheader()
        for clave, r in sorted(todos.items(), key=lambda par: (sorted(par[1]["temas"]), par[1]["anio"]), reverse=False):
            salida.writerow({**{c: r.get(c, "") for c in campos}, "clave": clave,
                             "temas": ";".join(sorted(r["temas"])), "origen": ";".join(sorted(r["origen"]))})
    suma = sum(b["recuperados"] for b in bitacora)
    print(f"\nRegistros recuperados: {suma}; únicos tras quitar duplicados: {len(todos)}")


if __name__ == "__main__":
    main()
