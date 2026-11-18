"""Genera la tabla de los documentos incluidos para el material suplementario.

Uso: python3 docs/articulo/tabla_incluidos.py

Lee docs/revision/texto_completo_G*.csv y escribe
docs/articulo/latex/datos/incluidos.tex (filas de una longtable).
"""
import csv
from pathlib import Path

RAIZ = Path(__file__).resolve().parent.parent
GRUPOS = {
    "G1": "Vías respiratorias altas, boca y ojos",
    "G2": "Vías respiratorias bajas",
    "G3": "Urinaria, piel, fiebre y meningitis",
    "G4": "Diarrea",
}
ESPECIALES = {"\\": r"\textbackslash{}", "&": r"\&", "%": r"\%", "#": r"\#",
              "_": r"\_", "$": r"\$", "{": r"\{", "}": r"\}", "~": r"\textasciitilde{}"}


def escapar(texto):
    return "".join(ESPECIALES.get(c, c) for c in texto.strip())


def identificador(clave):
    tipo, _, valor = clave.partition(":")
    if tipo == "doi":
        return r"\texttt{\scriptsize " + escapar(valor) + "}"
    if tipo == "pmid":
        return "PMID " + escapar(valor)
    return "Sin identificador"


def ambito(texto):
    """Clasifica por el comienzo del campo, como en el diagrama de flujo."""
    texto = texto.strip().lower()
    for prefijo, etiqueta in (("colombia", "Colombia"), ("latino", "Latinoamérica"), ("oms", "OMS")):
        if texto.startswith(prefijo):
            return etiqueta
    return "Internacional"


def main():
    filas = []
    total = 0
    for grupo, nombre in GRUPOS.items():
        archivo = RAIZ / "revision" / f"texto_completo_{grupo}.csv"
        with open(archivo, encoding="utf-8") as f:
            incluidos = [r for r in csv.DictReader(f) if r["decision"] == "incluir"]
        incluidos.sort(key=lambda r: (r["anio"], r["organizacion"]))
        filas.append(r"\multicolumn{4}{@{}l}{\textbf{" + f"{grupo}. {nombre} ({len(incluidos)})" + r"}} \\")
        for r in incluidos:
            filas.append(" & ".join([
                escapar(r["anio"]),
                escapar(r["organizacion"]) + r"\newline{\footnotesize " + escapar(r["titulo_corto"]) + "}",
                ambito(r["ambito"]),
                identificador(r["clave"]),
            ]) + r" \\")
        filas.append(r"\addlinespace")
        total += len(incluidos)
    destino = RAIZ / "articulo" / "latex" / "datos" / "incluidos.tex"
    destino.write_text("% Generado por docs/articulo/tabla_incluidos.py; no editar a mano.\n"
                       + "\n".join(filas) + "\n", encoding="utf-8")
    print(f"{total} documentos en {destino}")


if __name__ == "__main__":
    main()
