# Instrucciones de extracción a texto completo

Lectura de los documentos incluidos en el cribado (`cribado.csv`, decisión
«incluir») de un grupo temático. Todo dato debe tener cita textual y página
o tabla; lo que no se pueda verificar se marca, no se completa de memoria.

## Dónde están los textos

- `papers/revision/` (columna `archivo` de `docs/revision/recuperacion.csv`):
  XML de PubMed Central, PDF o HTML de acceso abierto.
- Copias obtenidas en revisiones anteriores (texto ya extraído en `.txt`), en
  la carpeta temporal de la sesión: `dl/` y `dl2/` (por ejemplo IDSA 2012,
  PIDS/IDSA 2011, IDSA 2014 de piel, IDSA/SHEA 2017, ACORL 2024, ACIN 2023,
  consenso colombiano de infección urinaria 2023, AIEPI 2012, OMS AWaRe 2022).
- `papers/*.pdf`: los 31 documentos del corpus inicial.
- Si un documento colombiano o latinoamericano no está en ninguna de esas
  carpetas, se puede intentar una sola vez su página de acceso abierto
  (columna `url` de `recuperacion.csv`). Nada más.

## Salidas por grupo (X = G1, G2, G3 o G4)

1. `docs/revision/texto_completo_X.csv` — una fila por documento incluido en
   el cribado del grupo: `clave, recuperado (si|no), decision (incluir|excluir),
   motivo (si se excluye: T1 sin dosis pediátricas utilizables, T2 no es guía
   al leerlo, T3 duplicado o resumen de otro incluido, T4 población fuera del
   alcance), fuente_en_base (id de data/fuente/fuentes.csv si ya está; vacío si
   no), ambito, organizacion, anio, titulo_corto`.
2. `docs/revision/esquemas_X.csv` — una fila por esquema pediátrico
   extraído de cada documento incluido y recuperado, con las columnas de
   `data/fuente/esquemas.csv` que apliquen (enfermedad_id, linea, opcion,
   descripcion_opcion, medicamento_id, via, base, dosis_min, dosis_usual,
   dosis_max, intervalo_horas, max_por_dosis, max_por_dia, duracion_dias_min,
   duracion_dias_max, edad_min_meses, edad_max_meses, peso_min_kg, peso_max_kg,
   nota) más: `clave` (documento), `pagina`, `cita` (textual, en el idioma del
   documento), `comparacion` (igual | distinta | nueva) y `detalle_comparacion`
   (qué difiere del esquema de la base con el mismo enfermedad_id y
   medicamento_id). Use los identificadores de enfermedad y medicamento de la
   base; si la enfermedad o el medicamento no existen, invente un id en
   minúsculas y márquelo en la nota como «nuevo».
3. `docs/revision/maximos_X.csv` — dosis máximas pediátricas o de adulto que
   los documentos declaren por medicamento: `medicamento_id, via,
   max_por_dosis, max_por_dia, unidad, poblacion, clave, pagina, cita`.

## Prioridades

- Extraer todo de los documentos colombianos y latinoamericanos.
- De los internacionales, extraer la versión más reciente de cada organismo
  principal por infección (OMS, NICE, AAP, IDSA/PIDS, ESPID, ESPGHAN, sociedades
  nacionales europeas). De los demás basta la fila de `texto_completo_X.csv` y
  una nota de si concuerdan con la base.
- Las revisiones de guías no aportan esquemas propios: regístrelas en
  `texto_completo_X.csv` y anote qué guías comparan.

Reglas: no inventar números; dosis en la unidad del medicamento (mg o UI);
«mg/kg/día» y «mg/kg/dosis» no se confunden; si una guía da un rango, poner
mínimo y máximo y la usual vacía; si la cita no aclara las tomas al día,
dejar el intervalo vacío y decirlo en la nota.
