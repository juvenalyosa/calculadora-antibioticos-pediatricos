# Arquitectura

## Idea central

El programa separa tres cosas que cambian a ritmos distintos:

| Capa | Qué contiene | Quién la cambia | Dónde vive |
|---|---|---|---|
| **Datos** | Enfermedades, dosis, ajustes renales, presentaciones, fuentes | Equipo clínico | `data/` |
| **Motor** | Cómo se calcula una dosis a partir de los datos | Desarrolladores | `src/core/` |
| **Interfaz** | Cómo se piden los datos y se muestra el resultado | Desarrolladores | `src/cli/` (hoy), `src/gui/` (previsto) |

Ningún antibiótico ni dosis está escrito en el código C++. Actualizar una guía
es cambiar datos; el motor y sus pruebas no se tocan.

```
        ┌──────────────┐   ┌──────────────┐   ┌───────────────────┐
        │ calc-dosis   │   │ GUI Qt       │   │ otras interfaces  │
        │ (consola)    │   │ (previsto)   │   │ (web, etc.)       │
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
| `renal.hpp` | Función renal e interfaz `GfrEstimator` (Schwartz de cabecera) |
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
3. **Sin fallos silenciosos.** Tope aplicado, ajuste renal, dato no
   verificado, falta de información o redondeo excesivo generan una
   advertencia con código estable (`TOPE_MAXIMO_APLICADO`,
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

### Agregar otra fórmula de TFG

Crear una clase que herede de `GfrEstimator` (`renal.hpp`), junto a
`BedsideSchwartzEstimator`, con sus pruebas en `tests/test_renal.cpp`. El
motor no cambia: recibe la TFG ya estimada dentro de `RenalFunction`.

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

### Agregar la interfaz gráfica

Crear `src/gui/` con su `CMakeLists.txt`, enlazar con `calc::core` y añadir
`add_subdirectory(src/gui)` tras una opción `CALC_BUILD_GUI` en el archivo
raíz. La interfaz por pasos prevista (Qt Widgets, `QWizard`) es:

    paciente → función renal → enfermedad → resultado

En cada paso hay un botón «Reportar un error» que llama a `makeErrorReport`
con el nombre del paso. La interfaz debe usar `text.hpp` para escribir números
y dosis igual que la consola.

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
| Rango de edad en que se considera válida la fórmula de Schwartz | 1 a 16 años | `BedsideSchwartzEstimator` |
| Peso «poco plausible» para la edad | fuera de 0,5 a 2,5 veces el esperado | `limits` en `patient.hpp` |
| Paso de redondeo de líquidos por defecto | 0,5 mL | `kb_loader.cpp` |
| Referencia farmacológica para el ajuste renal | por definir | hoja `medicamentos` |

## Pruebas

- **Unitarias** (`tests/test_*.cpp`): usan una base sintética con fármacos
  ficticios (`tests/support/test_kb.hpp`), de modo que no cambian cuando se
  actualiza el contenido clínico.
- **Matriz alfa** (`tests/test_alpha_matrix.cpp`): compara el motor con los
  cálculos hechos a mano sobre la base real. Ver [validacion_alfa.md](validacion_alfa.md).
- **Base al día** (`kb_al_dia`): falla si el JSON no corresponde a las hojas CSV.
