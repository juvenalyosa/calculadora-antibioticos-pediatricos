#!/usr/bin/env python3
"""Construye la base de conocimiento JSON a partir de las hojas CSV.

Flujo de trabajo de los datos clínicos:

    hojas de cálculo (equipo clínico)
        └─ exportar como CSV ─> data/fuente/*.csv
              └─ este script ─> data/kb/base_conocimiento.json
                    └─ calc-dosis validar ─> reglas clínicas y cruces

Este script solo CONVIERTE: comprueba columnas, números y valores sí/no. Las
reglas de contenido (dosis mínima <= usual <= máxima, bandas renales sin
solaparse, etc.) las valida el programa en C++ para que exista un único lugar
con esas reglas.

Uso:
    python3 tools/construir_kb.py            # genera el JSON
    python3 tools/construir_kb.py --check    # falla si el JSON está desactualizado

Solo usa la biblioteca estándar de Python 3.9 o superior.
"""

from __future__ import annotations

import argparse
import csv
import json
import sys
from pathlib import Path

RAIZ = Path(__file__).resolve().parent.parent
CARPETA_FUENTE = RAIZ / "data" / "fuente"
ARCHIVO_SALIDA = RAIZ / "data" / "kb" / "base_conocimiento.json"

# Versión de la FORMA del JSON. Debe coincidir con kSupportedSchemaVersion
# (src/core/include/calc/kb_loader.hpp).
ESQUEMA_VERSION = 1

ORDEN_LINEAS = ["primera", "segunda", "alergia"]

# Columnas obligatorias de cada hoja (el orden no importa).
COLUMNAS = {
    "fuentes": ["id", "titulo", "organizacion", "anio", "url"],
    "medicamentos": [
        "id", "nombre", "unidad", "ajuste_renal_tipo", "ajuste_renal_nota",
        "ajuste_renal_fuente_id", "ajuste_renal_estado",
    ],
    "presentaciones": [
        "id", "medicamento_id", "descripcion", "via", "tipo", "cantidad",
        "volumen_ml", "unidad", "divisible", "paso_redondeo",
    ],
    "ajuste_renal": [
        "medicamento_id", "tfg_min", "tfg_max", "factor_dosis",
        "intervalo_minimo_horas", "dosis_por_kg", "max_por_dosis", "evitar", "nota",
    ],
    "topes_medicamento": [
        "medicamento_id", "via", "max_por_dosis", "max_por_dia", "fuente_id", "pagina", "nota",
    ],
    "enfermedades": [
        "id", "nombre", "grupo", "requiere_antibiotico", "mensaje", "fuente_id",
    ],
    "esquemas": [
        "enfermedad_id", "linea", "opcion", "descripcion_opcion",
        "medicamento_id", "via", "base", "dosis_min", "dosis_usual",
        "dosis_max", "intervalo_horas", "max_por_dosis", "max_por_dia",
        "duracion_dias_min", "duracion_dias_max", "edad_min_meses",
        "edad_max_meses", "peso_min_kg", "peso_max_kg", "nota", "fuente_id",
        "pagina", "estado", "verificado_por_1", "verificado_por_2",
    ],
}


class ErrorDatos(Exception):
    """Problema en las hojas CSV que impide generar la base."""


# --- Lectura de hojas --------------------------------------------------------

def leer_hoja(carpeta: Path, nombre: str) -> list[dict[str, str]]:
    """Lee una hoja CSV y devuelve sus filas no vacías.

    Cada fila lleva la clave interna "_origen" ("esquemas.csv fila 7") para
    que los mensajes de error indiquen exactamente dónde corregir.
    """
    ruta = carpeta / f"{nombre}.csv"
    if not ruta.exists():
        raise ErrorDatos(f"No existe la hoja {ruta}")

    # utf-8-sig tolera la marca BOM que añade Excel al exportar.
    with ruta.open(encoding="utf-8-sig", newline="") as archivo:
        lector = csv.DictReader(archivo)
        encabezados = [e.strip() for e in (lector.fieldnames or [])]
        faltantes = [c for c in COLUMNAS[nombre] if c not in encabezados]
        if faltantes:
            raise ErrorDatos(f"{ruta.name}: faltan las columnas {', '.join(faltantes)}")

        filas = []
        for numero, fila in enumerate(lector, start=2):
            limpia = {(k or "").strip(): (v or "").strip() for k, v in fila.items() if k}
            if not any(limpia.values()):
                continue  # fila en blanco
            limpia["_origen"] = f"{ruta.name} fila {numero}"
            filas.append(limpia)
    return filas


# --- Conversión de celdas ----------------------------------------------------

def numero(fila: dict[str, str], columna: str) -> int | float | None:
    """Convierte una celda numérica. Acepta coma o punto decimal.

    Devuelve un entero cuando el valor no tiene decimales, porque campos como
    «intervalo_horas» deben llegar al JSON como enteros.
    """
    texto = fila[columna]
    if not texto:
        return None
    try:
        valor = float(texto.replace(" ", "").replace(",", "."))
    except ValueError:
        raise ErrorDatos(
            f"{fila['_origen']}: «{columna}» debe ser un número (se encontró «{texto}»)"
        ) from None
    return int(valor) if valor.is_integer() else valor


def si_no(fila: dict[str, str], columna: str) -> bool | None:
    """Convierte una celda sí/no. Vacío significa "no indicado"."""
    texto = fila[columna].lower()
    if not texto:
        return None
    if texto in ("si", "sí", "s", "true", "1"):
        return True
    if texto in ("no", "n", "false", "0"):
        return False
    raise ErrorDatos(f"{fila['_origen']}: «{columna}» debe ser si o no (se encontró «{texto}»)")


def sin_vacios(datos: dict) -> dict:
    """Quita las claves sin valor para que el JSON solo tenga lo informado.

    El número 0 y el valor False SÍ se conservan: son datos, no ausencias.
    """
    return {k: v for k, v in datos.items() if v not in (None, "", [], {})}


# --- Construcción ------------------------------------------------------------

def construir_medicamentos(carpeta: Path) -> list[dict]:
    presentaciones = leer_hoja(carpeta, "presentaciones")
    bandas = leer_hoja(carpeta, "ajuste_renal")
    topes = leer_hoja(carpeta, "topes_medicamento")

    medicamentos = []
    for fila in leer_hoja(carpeta, "medicamentos"):
        identificador = fila["id"]
        ajuste = sin_vacios({
            "tipo": fila["ajuste_renal_tipo"],
            "estado": fila["ajuste_renal_estado"],
            "nota": fila["ajuste_renal_nota"],
            "fuente_id": fila["ajuste_renal_fuente_id"],
            "bandas": [
                sin_vacios({
                    "tfg_min": numero(b, "tfg_min"),
                    "tfg_max": numero(b, "tfg_max"),
                    "factor_dosis": numero(b, "factor_dosis"),
                    "intervalo_minimo_horas": numero(b, "intervalo_minimo_horas"),
                    "dosis_por_kg": numero(b, "dosis_por_kg"),
                    "max_por_dosis": numero(b, "max_por_dosis"),
                    "evitar": si_no(b, "evitar"),
                    "nota": b["nota"],
                })
                for b in bandas if b["medicamento_id"] == identificador
            ],
        })
        medicamentos.append({
            "id": identificador,
            "nombre": fila["nombre"],
            "unidad": fila["unidad"],
            "ajuste_renal": ajuste,
            "topes": [
                sin_vacios({
                    "via": t["via"],
                    "max_por_dosis": numero(t, "max_por_dosis"),
                    "max_por_dia": numero(t, "max_por_dia"),
                    "fuente_id": t["fuente_id"],
                    "pagina": t["pagina"],
                    "nota": t["nota"],
                })
                for t in topes if t["medicamento_id"] == identificador
            ],
            "presentaciones": [
                sin_vacios({
                    "id": p["id"],
                    "descripcion": p["descripcion"],
                    "via": p["via"],
                    "tipo": p["tipo"],
                    "cantidad": numero(p, "cantidad"),
                    "volumen_ml": numero(p, "volumen_ml"),
                    "unidad": p["unidad"],
                    "divisible": si_no(p, "divisible"),
                    "paso_redondeo": numero(p, "paso_redondeo"),
                })
                for p in presentaciones if p["medicamento_id"] == identificador
            ],
        })

    # Filas huérfanas: apuntan a un medicamento que no está en la hoja.
    conocidos = {m["id"] for m in medicamentos}
    for fila in presentaciones + bandas + topes:
        if fila["medicamento_id"] not in conocidos:
            raise ErrorDatos(
                f"{fila['_origen']}: el medicamento «{fila['medicamento_id']}» "
                "no está en medicamentos.csv"
            )
    return medicamentos


def construir_componente(fila: dict[str, str]) -> dict:
    """Convierte una fila de esquemas.csv en un componente de una opción."""
    return sin_vacios({
        "medicamento_id": fila["medicamento_id"],
        "via": fila["via"],
        "base": fila["base"],
        "dosis": sin_vacios({
            "min": numero(fila, "dosis_min"),
            "usual": numero(fila, "dosis_usual"),
            "max": numero(fila, "dosis_max"),
        }),
        "intervalo_horas": numero(fila, "intervalo_horas"),
        "max_por_dosis": numero(fila, "max_por_dosis"),
        "max_por_dia": numero(fila, "max_por_dia"),
        "duracion_dias": sin_vacios({
            "min": numero(fila, "duracion_dias_min"),
            "max": numero(fila, "duracion_dias_max"),
        }),
        "edad_meses": sin_vacios({
            "min": numero(fila, "edad_min_meses"),
            "max": numero(fila, "edad_max_meses"),
        }),
        "peso_kg": sin_vacios({
            "min": numero(fila, "peso_min_kg"),
            "max": numero(fila, "peso_max_kg"),
        }),
        "nota": fila["nota"],
        "fuente_id": fila["fuente_id"],
        "pagina": fila["pagina"],
        "estado": fila["estado"],
        "verificado_por": [v for v in (fila["verificado_por_1"], fila["verificado_por_2"]) if v],
    })


def construir_enfermedades(carpeta: Path) -> list[dict]:
    esquemas = leer_hoja(carpeta, "esquemas")
    enfermedades = []

    for fila in leer_hoja(carpeta, "enfermedades"):
        identificador = fila["id"]
        requiere = si_no(fila, "requiere_antibiotico")

        # Las filas con igual (línea, opción) son componentes de una misma
        # opción: así se expresa una terapia combinada.
        lineas: dict[str, dict[int, dict]] = {}
        for esquema in esquemas:
            if esquema["enfermedad_id"] != identificador:
                continue
            numero_opcion = numero(esquema, "opcion")
            if not isinstance(numero_opcion, int):
                raise ErrorDatos(f"{esquema['_origen']}: «opcion» debe ser un número entero")
            opcion = lineas.setdefault(esquema["linea"], {}).setdefault(
                numero_opcion,
                {
                    "numero": numero_opcion,
                    "descripcion": esquema["descripcion_opcion"],
                    "componentes": [],
                },
            )
            opcion["componentes"].append(construir_componente(esquema))

        desconocidas = [nombre for nombre in lineas if nombre not in ORDEN_LINEAS]
        if desconocidas:
            raise ErrorDatos(
                f"enfermedad «{identificador}»: línea no reconocida «{desconocidas[0]}» "
                f"(valores permitidos: {', '.join(ORDEN_LINEAS)})"
            )

        enfermedad = sin_vacios({
            "id": identificador,
            "nombre": fila["nombre"],
            "grupo": fila["grupo"],
            "mensaje": fila["mensaje"],
            "fuente_id": fila["fuente_id"],
            "lineas": [
                {"linea": nombre, "opciones": [lineas[nombre][n] for n in sorted(lineas[nombre])]}
                for nombre in ORDEN_LINEAS if nombre in lineas
            ],
        })
        enfermedad["requiere_antibiotico"] = True if requiere is None else requiere
        enfermedades.append(enfermedad)

    conocidas = {e["id"] for e in enfermedades}
    for esquema in esquemas:
        if esquema["enfermedad_id"] not in conocidas:
            raise ErrorDatos(
                f"{esquema['_origen']}: la enfermedad «{esquema['enfermedad_id']}» "
                "no está en enfermedades.csv"
            )
    return enfermedades


def construir(carpeta: Path) -> dict:
    """Arma la base de conocimiento completa como un diccionario."""
    ruta_metadatos = carpeta / "metadatos.json"
    if not ruta_metadatos.exists():
        raise ErrorDatos(f"No existe {ruta_metadatos}")
    metadatos = json.loads(ruta_metadatos.read_text(encoding="utf-8"))

    return {
        "esquema_version": ESQUEMA_VERSION,
        "version": metadatos["version"],
        "fecha": metadatos["fecha"],
        "fuentes": [
            sin_vacios({
                "id": f["id"],
                "titulo": f["titulo"],
                "organizacion": f["organizacion"],
                "anio": numero(f, "anio"),
                "url": f["url"],
            })
            for f in leer_hoja(carpeta, "fuentes")
        ],
        "medicamentos": construir_medicamentos(carpeta),
        "enfermedades": construir_enfermedades(carpeta),
    }


# --- Programa principal ------------------------------------------------------

def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("--fuente", type=Path, default=CARPETA_FUENTE,
                        help="carpeta con las hojas CSV (por defecto data/fuente)")
    parser.add_argument("--salida", type=Path, default=ARCHIVO_SALIDA,
                        help="archivo JSON a generar (por defecto data/kb/base_conocimiento.json)")
    parser.add_argument("--check", action="store_true",
                        help="no escribe: falla si el JSON existente está desactualizado")
    argumentos = parser.parse_args()

    try:
        base = construir(argumentos.fuente)
    except ErrorDatos as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 1

    texto = json.dumps(base, ensure_ascii=False, indent=2) + "\n"

    if argumentos.check:
        actual = argumentos.salida.read_text(encoding="utf-8") if argumentos.salida.exists() else ""
        if actual != texto:
            print(f"ERROR: {argumentos.salida} está desactualizado respecto a las hojas CSV. "
                  "Ejecute: python3 tools/construir_kb.py", file=sys.stderr)
            return 1
        print(f"OK: {argumentos.salida} está al día.")
        return 0

    argumentos.salida.parent.mkdir(parents=True, exist_ok=True)
    argumentos.salida.write_text(texto, encoding="utf-8")
    print(f"Generado {argumentos.salida}: {len(base['enfermedades'])} enfermedades, "
          f"{len(base['medicamentos'])} medicamentos, {len(base['fuentes'])} fuentes.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
