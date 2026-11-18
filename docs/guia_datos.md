# Guía de datos para el equipo clínico

Esta guía explica cómo registrar la información de las guías de práctica
clínica para que la calculadora la use. No hace falta saber programar.

## Cómo se trabaja

1. La información se registra en **siete hojas** (archivos CSV en `data/fuente/`).
   Se pueden abrir y editar con Excel o Google Sheets; al terminar se exportan
   de nuevo como CSV con el mismo nombre.
2. Un desarrollador ejecuta `python3 tools/construir_kb.py`, que genera la base
   de conocimiento, y `calc-dosis validar`, que revisa las reglas.
3. Si hay errores, el programa indica la hoja, la fila y el problema.

Reglas generales:

- **No cambiar los nombres de las columnas** ni borrar columnas.
- Los **identificadores** (`id`) van en minúsculas, sin espacios ni tildes,
  con guion bajo: `amoxicilina_clavulanato`. Una vez creados no se cambian.
- Los números pueden llevar coma o punto decimal.
- Una celda vacía significa «no aplica» o «no definido».
- Todas las dosis se escriben **en la unidad del medicamento** (mg o UI)
  definida en la hoja `medicamentos`.

## Estados de revisión

Cada esquema y cada ajuste renal tiene un estado:

| Estado | Significado | Qué hace el programa |
|---|---|---|
| `muestra` | Dato de ejemplo para desarrollo | Advierte que no debe usarse |
| `borrador` | Digitado por una persona, sin segunda revisión | Advierte que no está verificado |
| `verificado` | Revisado de forma independiente por **dos** personas contra la fuente | Sin advertencia |

Para marcar `verificado` hay que escribir las iniciales de las dos personas
en `verificado_por_1` y `verificado_por_2`. El programa rechaza un
`verificado` sin dos verificadores distintos.

## Hoja `fuentes`

Una fila por guía o referencia.

| Columna | Obligatoria | Descripción |
|---|---|---|
| `id` | Sí | Identificador, p. ej. `aap_oma_2013` |
| `titulo` | Sí | Título completo de la guía |
| `organizacion` | No | Sociedad o entidad que la publica |
| `anio` | No | Año de publicación |
| `url` | No | Enlace o DOI |

## Hoja `medicamentos`

Una fila por antibiótico.

| Columna | Obligatoria | Descripción |
|---|---|---|
| `id` | Sí | Identificador |
| `nombre` | Sí | Nombre visible |
| `unidad` | Sí | `mg` o `UI` |
| `ajuste_renal_tipo` | Sí | `no_requiere`, `por_bandas` o `sin_dato` |
| `ajuste_renal_nota` | No | Aclaración que verá el usuario |
| `ajuste_renal_fuente_id` | Si está verificado | `id` de la fuente del ajuste |
| `ajuste_renal_estado` | Sí | `muestra`, `borrador` o `verificado` |

`sin_dato` significa «todavía no lo hemos compilado»: el programa advertirá al
usuario que la dosis no está ajustada. `no_requiere` es una afirmación clínica
y debe tener fuente.

## Hoja `presentaciones`

Una fila por presentación disponible.

| Columna | Obligatoria | Descripción |
|---|---|---|
| `id` | Sí | Identificador único en toda la hoja |
| `medicamento_id` | Sí | `id` del medicamento |
| `descripcion` | Sí | Texto visible, p. ej. `Suspensión 250 mg/5 mL` |
| `via` | Sí | `oral`, `iv`, `im` o `topica` |
| `tipo` | Sí | `liquido` (se mide en mL) o `solido` (se cuenta en unidades) |
| `cantidad` | Sí | Principio activo: en líquidos, el contenido en `volumen_ml`; en sólidos, por unidad |
| `volumen_ml` | En líquidos | Volumen que contiene la `cantidad` (para 250 mg/5 mL: 250 y 5) |
| `unidad` | En sólidos | `tableta`, `cápsula`, `vial`... en singular |
| `divisible` | No | `si` si se puede partir por la mitad |
| `paso_redondeo` | No | Mínima fracción medible. Por defecto 0,5 mL en líquidos; 0,5 o 1 unidad en sólidos |

Los viales reconstituidos se registran como `liquido` con la concentración
final (p. ej. 250 mg en 1 mL) y la forma de reconstituir en la descripción.

## Hoja `ajuste_renal`

Una fila por banda de TFG. Solo para medicamentos con `ajuste_renal_tipo` =
`por_bandas`.

| Columna | Obligatoria | Descripción |
|---|---|---|
| `medicamento_id` | Sí | `id` del medicamento |
| `tfg_min` | Sí | Límite inferior de la banda (incluido) |
| `tfg_max` | Sí | Límite superior de la banda (excluido) |
| `factor_dosis` | No | Fracción de la dosis por toma que se conserva: 1 = igual, 0,5 = la mitad |
| `intervalo_minimo_horas` | No | Intervalo mínimo entre tomas en esa banda |
| `dosis_por_kg` | No | Dosis de reemplazo por toma, por kilogramo: sustituye a la del esquema en esa banda |
| `max_por_dosis` | No | Tope por toma propio de la banda |
| `evitar` | No | `si` si el medicamento debe evitarse en esa banda |
| `nota` | No | Aclaración que verá el usuario |

Cómo se aplica, en este orden, con una TFG dentro de la banda:

1. Si la banda trae `dosis_por_kg`, la dosis por toma pasa a ser esa dosis por
   el peso, sea cual sea la del esquema.
2. La dosis por toma se multiplica por `factor_dosis`.
3. Si supera el `max_por_dosis` de la banda, se usa ese tope.
4. El intervalo pasa a ser **el mayor** entre el del esquema y
   `intervalo_minimo_horas`.

Una banda con solo `nota` no cambia la dosis, pero muestra la nota como aviso. Las bandas no pueden solaparse. Una TFG que no cae
en ninguna banda no recibe ajuste.

Ejemplo: «TFG 10 a 29: misma dosis cada 12 h; menor de 10: cada 24 h» son dos
filas con `factor_dosis` 1 e `intervalo_minimo_horas` 12 y 24.

Ejemplo con dosis de reemplazo: «TFG 10 a 30: 15 mg/kg cada 12 h, máximo 500 mg
por toma» es una fila con `dosis_por_kg` 15, `max_por_dosis` 500 e
`intervalo_minimo_horas` 12.

## Hoja `topes_medicamento`

Dosis máxima de cada medicamento por vía, sin importar la enfermedad: en la
práctica, la dosis máxima del adulto. El programa la aplica a los esquemas que
no traen su propio tope de ese tipo (por toma o por día), para que una dosis
por kilogramo no crezca sin límite en los pacientes de mayor peso. El
resultado lo advierte con `TOPE_DEL_MEDICAMENTO_APLICADO`.

| Columna | Obligatoria | Descripción |
|---|---|---|
| `medicamento_id` | Sí | `id` del medicamento |
| `via` | Sí | `oral`, `iv`, `im` o `topica`; una fila por vía |
| `max_por_dosis` | Uno de los dos | Máximo por toma |
| `max_por_dia` | Uno de los dos | Máximo diario |
| `fuente_id` | Sí | `id` de la fuente que da el máximo |
| `pagina` | No | Página o tabla |
| `nota` | No | Aclaración |

Si el esquema de una guía trae su propio tope, ese manda.

## Hoja `enfermedades`

Una fila por enfermedad.

| Columna | Obligatoria | Descripción |
|---|---|---|
| `id` | Sí | Identificador |
| `nombre` | Sí | Nombre visible |
| `grupo` | No | Agrupación en la interfaz, p. ej. `Respiratorias altas` |
| `requiere_antibiotico` | Sí | `si` o `no` |
| `mensaje` | Si es `no` | Texto que explica por qué no se indica antibiótico |
| `fuente_id` | No | `id` de la guía principal |

Las enfermedades virales frecuentes se registran con `requiere_antibiotico` =
`no` para que la calculadora lo diga de forma explícita.

## Hoja `esquemas`

Es la hoja principal: una fila por cada medicamento de cada opción de
tratamiento.

| Columna | Obligatoria | Descripción |
|---|---|---|
| `enfermedad_id` | Sí | `id` de la enfermedad |
| `linea` | Sí | `primera`, `segunda` o `alergia` |
| `opcion` | Sí | Número de la opción dentro de la línea (1, 2, 3...) |
| `descripcion_opcion` | Sí | Texto visible de la opción |
| `medicamento_id` | Sí | `id` del medicamento |
| `via` | Sí | `oral`, `iv`, `im` o `topica` |
| `base` | Sí | Cómo está expresada la dosis (ver abajo) |
| `dosis_min`, `dosis_usual`, `dosis_max` | Salvo `sin_calculo` | Rango de la guía y valor con el que se calcula |
| `intervalo_horas` | Según la base | Horas entre tomas. Vacío = dosis única |
| `max_por_dosis` | No | Tope por toma |
| `max_por_dia` | No | Tope diario |
| `duracion_dias_min`, `duracion_dias_max` | No | Duración del tratamiento |
| `edad_min_meses`, `edad_max_meses` | No | Edad en que aplica: desde el mínimo (incluido) hasta el máximo (excluido) |
| `peso_min_kg`, `peso_max_kg` | No | Peso en que aplica, con el mismo criterio |
| `nota` | No | Aclaración que verá el usuario |
| `fuente_id` | Sí | `id` de la guía |
| `pagina` | No | Página o tabla dentro de la guía |
| `estado` | Sí | `muestra`, `borrador` o `verificado` |
| `verificado_por_1`, `verificado_por_2` | Si está verificado | Iniciales de quienes verificaron |

Valores de `base`:

| Valor | Significado | Ejemplo |
|---|---|---|
| `mg_kg_dia` | Dosis diaria por kg, repartida según el intervalo | 90 mg/kg/día cada 12 h |
| `mg_kg_dosis` | Dosis por kg en cada toma | 50 mg/kg/dosis cada 24 h |
| `dosis_fija` | No depende del peso | 600 000 UI |
| `sin_calculo` | Indicación de texto (va en `nota`) | Tópico cada 8 h |

Casos frecuentes:

- **La guía da un solo valor** (no un rango): repetirlo en las tres columnas
  de dosis.
- **La guía da un rango** (80 a 90 mg/kg/día): escribir los extremos y decidir
  con cuál calcula la herramienta en `dosis_usual`.
- **Terapia combinada**: una fila por medicamento, con la misma `linea` y el
  mismo número de `opcion`.
- **Dosis por rango de peso** (menos de 27 kg / 27 kg o más): una opción por
  rango, cada una con su `peso_min_kg` o `peso_max_kg`. Con 27 en ambas, un
  paciente de exactamente 27 kg cae en la de «27 o más».
- **Duración que depende de la edad**: una opción por rango de edad, o un
  rango amplio de duración con la precisión en `nota`.

## Versión del contenido

`data/fuente/metadatos.json` guarda la versión y la fecha del contenido
clínico. Se actualiza cada vez que se publica un cambio en los datos; esa
versión aparece en cada resultado y en cada reporte de error.

## Lista de comprobación antes de entregar una hoja

- [ ] Cada fila de `esquemas` cita su `fuente_id` y su `pagina`.
- [ ] Las dosis están en la unidad del medicamento (mg o UI), no en gramos.
- [ ] Los topes máximos provienen de la fuente citada.
- [ ] Lo marcado `verificado` lo revisaron dos personas distintas.
- [ ] `calc-dosis validar` termina sin errores.
- [ ] Hay una fila en la matriz alfa para cada esquema nuevo
      (ver [validacion_alfa.md](validacion_alfa.md)).
