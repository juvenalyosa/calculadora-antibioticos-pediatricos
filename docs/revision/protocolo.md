# Protocolo de la revisión de alcance de guías

Revisión de alcance (*scoping review*) de guías de práctica clínica sobre el
tratamiento antibiótico de infecciones prevalentes en la infancia, para
alimentar la base de conocimiento de la calculadora. Se informa según la
extensión PRISMA para revisiones de alcance (PRISMA-ScR). El protocolo se
redactó durante la ejecución y **no se registró** antes de empezar; esa es una
limitación declarada.

## 1. Pregunta

¿Qué esquemas antibióticos (o antiparasitarios) recomiendan las guías vigentes
para las infecciones prevalentes en niños de 1 mes a menos de 18 años —
medicamento, vía, dosis, intervalo, dosis máxima y duración—, o cuándo
recomiendan no darlos, con prioridad para las guías válidas en Colombia?

| Elemento | Definición |
|---|---|
| Población | Niños y adolescentes de 1 mes a menos de 18 años |
| Concepto | Recomendaciones de tratamiento antimicrobiano, o de no tratar |
| Contexto | Colombia primero; Latinoamérica; OMS/OPS; guías internacionales de sociedades científicas o agencias |

Jerarquía para elegir la recomendación que carga la calculadora:

1. Guía o lineamiento colombiano vigente.
2. Guía latinoamericana o de la OMS/OPS.
3. Guía internacional de una sociedad científica o agencia reconocida, la más
   reciente.

Cuando una guía no da un dato necesario para calcular (por ejemplo, la dosis
máxima), se toma de la siguiente en la jerarquía o de la ficha técnica, y la
nota del esquema lo declara.

## 2. Fuentes de información y búsqueda

Búsqueda ejecutada el 18 de noviembre de 2026 con el programa
`docs/revision/buscar.py`, que contiene las ecuaciones completas y guarda la
bitácora (`bitacora_busqueda.json`) y los registros (`registros.csv`).

| Base | Estrategia | Registros |
|---|---|---|
| PubMed (MEDLINE) | Por tema: términos de la infección + niños + filtro de guía (tipo de publicación o título) + 2010-2026 + inglés o español | 1 587 (las dos estrategias de PubMed) |
| PubMed (MEDLINE) | Por tema: términos de la infección + niños + Colombia o Latinoamérica + antibióticos | (incluido arriba) |
| OpenAlex | Por tema, en español: título y resumen + autores de instituciones colombianas + 2010-2026 | 269 |
| DOAJ | Por tema, en español: términos de la infección + niños + guía, consenso o tratamiento | 198 |

Once temas: otitis; faringoamigdalitis; rinosinusitis; neumonía; bronquiolitis,
laringitis, bronquitis y tos ferina; infección urinaria; diarrea aguda y
disentería; piel y tejidos blandos y mordeduras; parásitos intestinales;
infección odontogénica y conjuntivitis; fiebre sin foco, sepsis y meningitis.

**Fuentes previstas que no se pudieron consultar de forma automatizada:**
LILACS (Biblioteca Virtual en Salud) y SciELO rechazaron las consultas
programáticas (HTTP 403); se intentó y quedó registrado. Su cobertura se
compensó en parte con OpenAlex y DOAJ, que indexan buena parte de las revistas
latinoamericanas, y con la búsqueda manual de guías del Ministerio de Salud,
AIEPI, ACORL, ACIN e IETS hecha en las revisiones previas
(`docs/revisiones/D_ckid_y_colombia.md` y `E_enfermedades_faltantes.md`).
Una revisión sistemática formal debería repetir la búsqueda en LILACS y SciELO
desde su interfaz web.

## 3. Criterios de elegibilidad

**Inclusión** (todos): (1) guía de práctica clínica, consenso, declaración de
posición, protocolo oficial de un ministerio o de la OMS/OPS, o revisión de
guías; (2) recomendación de tratamiento (elección, dosis o duración del
antimicrobiano, o la recomendación de no darlo) de una de las infecciones;
(3) población que incluye niños fuera del período neonatal; (4) inglés o
español; (5) 2010 a 2026.

**Exclusión en el cribado** (un motivo por registro): E1 no es guía; E2 sin
contenido de tratamiento; E3 población fuera del alcance; E4 infección fuera
del alcance; E5 versión reemplazada por otra del mismo organismo presente en
los registros; E6 no es un documento recuperable (fe de erratas, comentario,
resumen de otra guía incluida).

**Exclusión en el texto completo:** T1 sin dosis pediátricas utilizables; T2
no es guía al leerlo; T3 duplicado o resumen de otro incluido; T4 población
fuera del alcance.

## 4. Selección

- **Duplicados:** se quitaron por DOI, luego por PMID y luego por título
  normalizado (`buscar.py`).
- **Cribado por título y resumen:** en cuatro grupos temáticos, con los
  criterios anteriores escritos (`cribado_G1.csv` a `cribado_G4.csv`). Cuando
  el título no bastaba, se leyó el resumen en PubMed (anotado como «resumen
  consultado»). En caso de duda se incluyó para revisar a texto completo
  («dudoso»).
- **Texto completo:** se recuperó automáticamente lo que PubMed Central u
  OpenAlex declaran de acceso abierto (`recuperar.py`, resultado en
  `recuperacion.csv`), más las copias obtenidas en las revisiones previas.

**El cribado y la extracción los realizó un modelo de lenguaje (Claude,
Anthropic), un solo revisor por registro, sin doble revisión humana.** Es la
principal limitación metodológica: una revisión sistemática exige dos
revisores independientes. Las decisiones quedan en archivos para que el
equipo clínico las audite, empezando por los registros marcados «dudoso».

## 5. Extracción

Con un formato fijo (`instrucciones_extraccion.md`): por documento, decisión a
texto completo y si ya estaba en la base; por esquema, las columnas de la hoja
`esquemas` con cita textual y página, y su comparación con la base (igual,
distinta o nueva); y las dosis máximas que declare cada documento.

## 6. Síntesis e incorporación a la calculadora

Por infección se compara lo extraído con lo cargado y se aplica la jerarquía
del punto 1. Toda fila nueva o modificada entra en estado «borrador», con su
fuente y página. La versión de la base de conocimiento cambia con cada
incorporación.

## 7. Verificación de la calculadora

Prueba exhaustiva con una población pediátrica modelada con los patrones de
crecimiento de la OMS (adoptados en Colombia por la Resolución 2465 de 2016),
descrita en `docs/validacion/LEEME.md`.
