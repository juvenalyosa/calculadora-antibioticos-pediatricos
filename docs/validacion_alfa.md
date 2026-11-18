# Validación alfa

La metodología del proyecto (van der Zanden et al., 2020) valida la versión
alfa calculando, con dos pacientes tipo, la primera y la segunda línea de cada
enfermedad, con dosis normal y con ajuste renal, y repitiendo hasta que no
aparezca ningún error.

| Caso tipo | Edad | Peso | Talla |
|---|---|---|---|
| Lactante | 1 año (12 meses) | 10 kg | 75 cm |
| Adolescente | 13 años (156 meses) | 50 kg | 162 cm |

## Cómo está automatizada

La matriz de validación es el archivo `tests/data/matriz_alfa.csv`. Cada fila
es **un cálculo hecho a mano** por el equipo clínico. La prueba
`tests/test_alpha_matrix.cpp` ejecuta el motor con la base de conocimiento
real y exige que coincida con cada fila.

«Sin ningún error» equivale a que `ctest` pase. Si una fila no coincide, la
salida indica el caso, el valor esperado y el calculado.

## Columnas de la matriz

| Columna | Descripción |
|---|---|
| `caso` | Nombre libre que identifica la fila en los mensajes de error |
| `edad_meses`, `peso_kg`, `talla_cm` | Datos del paciente |
| `tfg` | TFG en mL/min/1,73 m². Vacío = función renal normal |
| `enfermedad_id` | `id` de la enfermedad |
| `linea` | `primera`, `segunda` o `alergia` |
| `opcion` | Número de la opción dentro de la línea |
| `medicamento_id` | `id` del medicamento |
| `dosis_por_toma` | Dosis esperada por toma, en la unidad del medicamento |
| `intervalo_horas` | Intervalo esperado. Vacío = dosis única |
| `presentacion_id` | Opcional: presentación a comprobar |
| `cantidad` | Cantidad esperada de esa presentación (mL o unidades), ya redondeada |
| `calculado_por` | Iniciales de quien hizo el cálculo a mano |

Formato: punto decimal y sin comas dentro de los campos.

## Procedimiento

1. **Definir los escenarios renales.** Se proponen dos valores fijos de TFG
   para ambos pacientes, uno por tramo habitual de ajuste (por ejemplo 40 y
   15). El equipo clínico los confirma.
2. **Calcular a mano.** Dos personas calculan cada fila de forma
   independiente a partir de la guía, sin usar la calculadora. Las
   discrepancias entre ellas se resuelven antes de registrar la fila.
3. **Registrar** las filas en la matriz.
4. **Ejecutar** `ctest --preset dev`.
5. **Analizar cada diferencia.** Puede venir del programa, de un dato mal
   digitado en las hojas o del cálculo manual. Se corrige la causa y se
   registra en la tabla de errores del informe técnico (causa, corrección,
   fecha).
6. **Repetir** hasta que todas las filas pasen.

Cobertura mínima por enfermedad: 2 pacientes × primera y segunda línea ×
normal y con ajuste renal = 8 filas, más las alternativas en alergia.

## Estado actual

Las 321 filas incluidas cubren los dos casos tipo en todos los esquemas de la
base en **borrador**, con función renal normal y con TFG de 40, 20 y 5 en los
medicamentos que tienen bandas de ajuste. Las calculó el desarrollo con un segundo programa,
independiente del motor y escrito a partir de las reglas de la guía de datos
(`tools/verificacion_cruzada.py`, que regenera la matriz)
(`calculado_por` = «desarrollo (verificación cruzada; falta cálculo
clínico)»). Sirven para detectar errores del motor, pero no constituyen
validación clínica: deben reemplazarse por los cálculos a mano del equipo
clínico a medida que se verifique cada enfermedad.

Las bandas de ajuste renal salen de fichas técnicas y del Kinderformularium,
no de las guías clínicas, y están en borrador. Las bandas que indican «evitar»
no tienen fila: la matriz no puede expresar ese resultado.
