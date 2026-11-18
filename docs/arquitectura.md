# Arquitectura

## Idea central

El programa separa tres cosas que cambian a ritmos distintos:

| Capa | Qué contiene | Quién la cambia | Dónde vive |
|---|---|---|---|
| **Datos** | Enfermedades, dosis, ajustes renales, presentaciones, fuentes | Equipo clínico | `data/` |
| **Motor** | Cómo se calcula una dosis a partir de los datos | Desarrolladores | `src/core/` |
| **Interfaz** | Cómo se piden los datos y se muestra el resultado | Desarrolladores | `src/cli/` (consola), `src/gui/` (gráfica) |

Ningún antibiótico ni dosis está escrito en el código C++. Actualizar una guía
es cambiar datos; el motor y sus pruebas no se tocan.

```
        ┌──────────────┐   ┌──────────────┐   ┌───────────────────┐
        │ calc-dosis   │   │ calc-gui     │   │ otras interfaces  │
        │ (consola)    │   │ (Qt Widgets) │   │ (web, etc.)       │
        └──────┬───────┘   └──────┬───────┘   └─────────┬─────────┘
               └──────────────────┼─────────────────────┘
                                  ▼
        ┌──────────────────────────────────────────────────────────┐
        │ calc_core                                                │
        │                                                          │
        │  kb_loader ──> KnowledgeBase ──> DoseEngine ──> plan     │
        │  (lee y valida)                  (calcula)        │      │
        │                                                   ▼      │
        │  GfrEstimator        ReportSink          AdvisorRegistry │
        │  (estima TFG)        (reportes)          (módulos, IA)   │
        └──────────────────────────────────────────────────────────┘
                                  ▲
                     data/kb/base_conocimiento.json
```

## Módulos de `calc_core`

Cada cabecera de `src/core/include/calc/` documenta su propio contrato.

| Cabecera | Responsabilidad |
|---|---|
| `units.hpp` | Magnitudes con tipos fuertes (kg, cm, mL, mg/dL, TFG) y conversiones |
| `warnings.hpp` | Advertencias con gravedad y código estable |
| `patient.hpp` | Edad, paciente, validación de alcance y plausibilidad |
| `renal.hpp` | Función renal, interfaz `GfrEstimator` y catálogo de fórmulas (CKiD U25, Schwartz de cabecera) |
| `knowledge_base.hpp` | Modelo en memoria de los datos clínicos |
| `kb_loader.hpp` | Lectura del JSON y validación de todas sus reglas |
| `dose_engine.hpp` | Motor: solicitud → plan de tratamiento |
| `text.hpp` | Formato de números y frases, común a todas las interfaces |
| `error_report.hpp` | Reporte de errores del usuario e interfaz `ReportSink` |
| `advisor.hpp` | Punto de extensión para módulos de apoyo (IA) |

Dependencias: `calc_core` solo depende de la biblioteca estándar; nlohmann/json
es un detalle interno que no aparece en las cabeceras públicas.

## Orden del cálculo

Para cada medicamento de cada opción, `DoseEngine::calculate` aplica siempre
los mismos cuatro pasos y deja constancia de cada uno en el desglose:

1. **Dosis base.** Según la base del esquema: `mg/kg/día × peso` repartido por
   el intervalo, `mg/kg/dosis × peso`, o dosis fija.
2. **Topes.** Máximo por toma y luego máximo diario.
3. **Ajuste renal.** Si hay TFG y el medicamento tiene una banda que la
   contiene: la dosis por toma se multiplica por `factor_dosis` y el intervalo
   pasa a ser el mayor entre el del esquema y `intervalo_minimo_horas`. Una
   banda puede indicar *evitar*; entonces no se propone administración.
4. **Presentación.** Regla de tres `dosis ÷ concentración`, redondeo al paso
   medible o partible, y cálculo de la desviación entre lo calculado y lo que
   realmente se entrega. Un redondeo nunca sobrepasa el tope por toma; si la
   desviación supera el 10 %, la presentación se marca como no adecuada.

Antes de todo, `DoseEngine::plan` valida al paciente (un dato bloqueante
impide calcular) y descarta las opciones que no le aplican por edad o peso,
informando el motivo.

## Reglas de diseño

Estas reglas son las que hacen confiable el cálculo; cualquier cambio debe
respetarlas.

1. **Determinista.** Mismas entradas y misma base producen el mismo resultado.
   No hay aleatoriedad, red ni reloj dentro del motor.
2. **Transparente.** Todo resultado trae el desglose y la fuente, para que el
   usuario pueda rehacer la cuenta a mano.
3. **Sin fallos silenciosos.** Tope aplicado, esquema sin tope, ajuste renal,
   dato no verificado, falta de información o redondeo excesivo generan una
   advertencia con código estable (`TOPE_MAXIMO_APLICADO`, `SIN_TOPE_MAXIMO`,
   `SIN_DATO_AJUSTE_RENAL`, `DATOS_NO_VERIFICADOS`...).
4. **Una sola puerta para los datos.** Todo dato clínico entra por
   `kb_loader`, que valida y reporta todos los problemas de una vez.
5. **Unidades con tipos.** Peso, talla, creatinina y TFG tienen tipos
   distintos; confundirlos es un error de compilación.
6. **Sin datos identificables.** El programa no guarda nombre, documento ni
   historia clínica; los reportes solo llevan las variables del cálculo.

## Cómo extender

### Agregar una enfermedad, antibiótico o presentación

Solo datos: ver [guia_datos.md](guia_datos.md). No se toca C++.

### Fórmulas de TFG

Cuando el usuario digita la creatinina, la TFG se estima con una de estas
fórmulas (ambas con creatinina enzimática y talla):

| Fórmula | Cálculo | Datos | Referencia |
|---|---|---|---|
| **CKiD U25** (por defecto) | `k × talla (m) ÷ creatinina`, con `k` según edad y sexo | talla, sexo | Pierce et al., Kidney Int 2021;99:948-956 |
| Schwartz de cabecera | `0,413 × talla (cm) ÷ creatinina` | talla | Schwartz et al., J Am Soc Nephrol 2009 |

Constante `k` de CKiD U25 (edad en años, con decimales):

| Edad | Masculino | Femenino |
|---|---|---|
| < 12 años | 39,0 × 1,008^(edad − 12) | 36,1 × 1,008^(edad − 12) |
| 12 a < 18 años | 39,0 × 1,045^(edad − 12) | 36,1 × 1,023^(edad − 12) |
| 18 a 25 años | 50,8 | 41,4 |

Schwartz equivale a un `k` fijo de 41,3 para todos; CKiD U25 lo baja en los
niños pequeños y en las mujeres. En el caso tipo de 1 año (75 cm, creatinina
0,4 mg/dL) Schwartz da 77,4 y CKiD U25 da 67,0 (niño) o 62,0 (niña). El sexo
solo se usa aquí: ninguna dosis depende de él.

Para agregar otra fórmula: crear una clase que herede de `GfrEstimator`
(`renal.hpp`), añadirla al catálogo (`GfrFormula`, `kGfrFormulas`, `id`,
`makeGfrEstimator`) y probarla en `tests/test_renal.cpp`. La consola
(`--formula`) y la interfaz (selector del paso 2) la ofrecen sin más cambios.
El motor no cambia: recibe la TFG ya estimada dentro de `RenalFunction`.

### Agregar un destino para los reportes de error

Crear una clase que herede de `ReportSink` (`error_report.hpp`). Hoy existe
`JsonlFileSink` (archivo local). Para la fase beta está previsto un destino
HTTP hacia la hoja de errores y correcciones, usando el archivo local como
cola cuando no haya conexión.

### Agregar un módulo de apoyo o de inteligencia artificial

El punto de entrada es `Advisor` (`advisor.hpp`). Un módulo recibe la
solicitud y el plan **ya calculado, en solo lectura**, y devuelve
observaciones que la interfaz muestra aparte, atribuidas al módulo.

```cpp
class MiModulo final : public calc::Advisor {
 public:
  std::string id() const override { return "mi-modulo"; }
  std::vector<calc::Annotation> review(const calc::AdvisorContext& ctx) const override {
    // Leer ctx.request y ctx.plan; devolver observaciones.
  }
};

registry.add(std::make_unique<MiModulo>());
auto observaciones = registry.reviewAll({request, plan});
```

Garantías que da la forma de la interfaz:

- un módulo **no puede modificar una dosis** (recibe el plan como `const`);
- si un módulo falla o no hay conexión, el cálculo se muestra igual;
- cada observación queda marcada con el módulo que la emitió.

Esto permite cumplir el requisito del proyecto de una arquitectura capaz de
integrar IA sin comprometer la validación: lo validado en las fases alfa y
beta es el motor determinista; un módulo de IA añade, nunca sustituye. Usos
razonables: explicar el desglose en lenguaje natural, señalar interacciones,
clasificar los reportes de error de la beta, o apoyar a los asistentes en la
extracción de tablas de las guías (siempre con verificación humana, fuera del
programa). `tests/test_advisor.cpp` trae dos módulos de ejemplo.

### Interfaz gráfica (`src/gui/`)

Asistente por pasos con Qt Widgets (`QWizard`), todo en C++:

    paciente → función renal → enfermedad → resultado

| Archivo | Responsabilidad |
|---|---|
| `main.cpp` | Arranque: idioma, base de conocimiento, servicios, ventana |
| `calculator_wizard.*` | Ventana principal, orden de los pasos, botón de reporte |
| `session.hpp` | Lo que el usuario ha introducido; lo comparten las páginas |
| `pages/*_page.*` | Una clase por paso |
| `report_dialog.*` | Formato semiautomatizado de reporte de error |
| `html.*` | Resultado del núcleo → HTML (usa `text.hpp`, igual que la consola) |
| `theme.*` | Aspecto visual: tabla de colores (clara y oscura), hoja de estilo e icono |
| `widgets/hero_header.*` | Encabezado de cada página: banda de color con el título y el progreso por pasos |
| `widgets/age_band.*` | Dibujo del grupo de edad del paciente (ayuda visual, no interviene en el cálculo) |
| `widgets/ui.*` | Piezas comunes: encabezado de página, tarjeta, etiquetas de campo |
| `app_paths.*` | Dónde están la base de conocimiento y el archivo de reportes |
| `screenshot_tour.*` | Recorrido automático con capturas (prueba y manual) |

Reglas que sigue la interfaz:

- **No calcula nada.** Valida y calcula con las mismas funciones del núcleo
  que usa la consola; por eso ambas dan siempre el mismo resultado.
- **La sesión se actualiza en cada cambio**, no al pulsar «Siguiente», para
  que un reporte de error capture lo que el usuario tiene en pantalla.
- **«Siguiente» se deshabilita** mientras haya una advertencia bloqueante.
- **Los módulos de apoyo** se registran en `main.cpp` y sus observaciones se
  muestran al final del resultado, separadas y atribuidas.
- **Gama del proyecto:** azul marino `#14213D`, azul `#1F4E79` y marfil
  `#F6F3EE`, con un detalle fino en dorado. Solo los avisos (ámbar, rojo,
  verde) usan colores fuera de ella.
- **Un solo origen para los colores.** Widgets y resultado en HTML toman los
  colores de `theme()`; no se escriben colores en las páginas. El estilo
  Fusion de Qt da el mismo aspecto en Windows, macOS y Linux, y el tema sigue
  el modo claro u oscuro del sistema (se puede forzar con `--tema`).
- **Tipografía Helvetica.** Se usa la primera instalada de esta lista:
  Helvetica Neue, Helvetica, Arial, Nimbus Sans, Liberation Sans. Windows no
  trae Helvetica y su licencia no permite distribuirla: allí se ve en Arial.

Para agregar un paso: crear la página en `pages/`, darle un identificador en
`CalculatorWizard::PageId`, registrarla con `setPage` y añadir su nombre en
`currentStepName()` para los reportes.

Los reportes se guardan en la carpeta de datos de la aplicación del usuario
(`reportes_de_error.jsonl`), una línea por reporte.

Pendiente para distribuir: empaquetar con `macdeployqt` (macOS) y
`windeployqt` (Windows), y compilar y probar en Windows.

### Actualización automática de los datos

Previsto como módulo `update`: al iniciar, consultar un manifiesto con la
versión y la huella de la base publicada; si es más nueva, descargarla,
comprobar su firma, pasarla por `kb_loader` y solo entonces reemplazar la
anterior, que se conserva como respaldo. Los campos `version`, `fecha` y
`esquema_version` de la base ya existen para esto.

## Decisiones pendientes del equipo clínico

Valores que hoy son supuestos razonables del desarrollo y deben confirmarse:

| Parámetro | Valor actual | Dónde |
|---|---|---|
| Desviación máxima aceptable por redondeo | 10 % | `limits::kMaxRoundingDeviation` |
| TFG bajo la cual se avisa si falta el dato de ajuste renal | 90 | `limits::kReducedGfrThreshold` |
| Fórmula de TFG por defecto | CKiD U25 | `kDefaultGfrFormula` en `renal.hpp` |
| Constantes de CKiD U25 | cotejadas con la tabla 2 de Pierce et al. 2021 y con el NIDDK; la ecuación solo se validó en enfermedad renal crónica | `renal.cpp` |
| Rango de edad en que se considera válida la fórmula de Schwartz | 1 a 16 años | `BedsideSchwartzEstimator` |
| Peso «poco plausible» para la edad | fuera de 0,5 a 2,5 veces el esperado | `limits` en `patient.hpp` |
| Paso de redondeo de líquidos por defecto | 0,5 mL | `kb_loader.cpp` |
| Referencia farmacológica para el ajuste renal | por medicamento: fichas técnicas europeas (EMA, AEMPS) o Kinderformularium; las fuentes no siempre coinciden | hojas `medicamentos` y `ajuste_renal` |
| Unidad de la TFG en las bandas | se comparan con mL/min/1,73 m², aunque algunas fichas escriben mL/min sin normalizar | hoja `ajuste_renal` |
| Valor con el que se calcula cuando la guía da un rango | el extremo superior (se muestra el rango completo) | columna `dosis_usual` de `esquemas` |
| Topes tomados de otra fuente | amoxicilina 4 g/día (PIDS/IDSA 2011), cefuroxima oral 1 g/día (FDA), dosis de adulto de IDSA para clindamicina y trimetoprim-sulfametoxazol; cada uno está escrito en la nota del esquema | columnas `max_por_dosis` y `max_por_dia` |
| Dosis de amoxicilina en neumonía | 90 mg/kg/día (guía colombiana, PIDS/IDSA) frente a 40 a 45 mg/kg/día (guía finlandesa): están las dos | hoja `esquemas` |
| Presentaciones comerciales | confirmadas en el registro del INVIMA (noviembre de 2026); el registro no garantiza disponibilidad | hoja `presentaciones` |

## Pruebas

- **Unitarias** (`tests/test_*.cpp`): usan una base sintética con fármacos
  ficticios (`tests/support/test_kb.hpp`), de modo que no cambian cuando se
  actualiza el contenido clínico.
- **Matriz alfa** (`tests/test_alpha_matrix.cpp`): compara el motor con los
  cálculos hechos a mano sobre la base real. Ver [validacion_alfa.md](validacion_alfa.md).
- **Base al día** (`kb_al_dia`): falla si el JSON no corresponde a las hojas CSV.
- **Recorrido de la interfaz** (`interfaz_recorrido`): ejecuta los cuatro pasos
  de la interfaz gráfica sin pantalla. Solo existe si se construyó con Qt.
