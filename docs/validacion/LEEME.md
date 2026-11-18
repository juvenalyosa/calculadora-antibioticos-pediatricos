# Prueba exhaustiva con población modelada

Comprueba que la calculadora hace exactamente lo que dicen sus reglas y las
guías cargadas, en toda la gama de pacientes pediátricos, no solo en los dos
casos tipo. Corre con `ctest` (prueba `validacion_poblacion`) o a mano:

```bash
python3 docs/validacion/ejecutar.py build/dev/src/cli/calc-dosis build/validacion
```

## Población

`poblacion.py` genera pacientes virtuales con los patrones de crecimiento de la
OMS (carpeta `oms/`; adoptados en Colombia por la Resolución 2465 de 2016),
por el método LMS: valor = M × (1 + L·S·z)^(1/L).

| Dimensión | Valores |
|---|---|
| Edad | 27 edades, de 1 mes a 17 años y 11 meses |
| Sexo | Masculino y femenino |
| Peso | Puntaje z −3, −2, 0, +2 y +3 (de la desnutrición grave a la obesidad) |
| Talla | Mediana para la edad |
| Función renal | No evaluada, y TFG de 75, 45, 20 y 5 mL/min/1,73 m² |
| Enfermedades | Todas las de la base |

Hasta los 5 años el peso sale del patrón de peso para la edad; desde los 61
meses, del índice de masa corporal para la edad y la talla mediana, porque la
OMS no publica peso para la edad después de los 10 años. Son 1 350 pacientes
virtuales, que con todas las enfermedades dan más de 130 000 resultados.

## Qué se comprueba

`verificar.py` no usa código del motor: vuelve a calcular a partir de las
hojas de datos y de las reglas escritas en `docs/guia_datos.md`.

| | Comprobación |
|---|---|
| A | Cada dosis, intervalo y cantidad de presentación coincide con el recálculo independiente |
| B | Cada opción aparece calculada o «no aplica» según sus límites de edad y peso |
| C1 | La dosis por kilogramo entregada no supera el máximo de la guía |
| C2 | La dosis no supera los topes por toma ni por día (del esquema o del medicamento) |
| C3 | Con función renal reducida la dosis no aumenta ni el intervalo se acorta |
| C4 | Donde la ficha técnica indica «evitar», el medicamento no se calcula; donde falta el dato renal, se advierte |
| C5 | Todo esquema por peso sin ningún tope muestra la advertencia `SIN_TOPE_MAXIMO` |
| C6 | Las presentaciones marcadas adecuadas desvían 10 % o menos y no superan el tope por toma |
| C7 | A igual edad y función renal, la dosis no disminuye al aumentar el peso |
| D | Dos ejecuciones producen exactamente el mismo archivo |
| E | Concordancia con tablas publicadas de dosis por peso (AIEPI 2012 y OMS AWaRe 2022) |

Las comprobaciones A a D deben pasar sin excepciones; la E es descriptiva,
porque esas tablas usan bandas de peso y no la dosis exacta por kilogramo.

El resultado queda en `informe.json`.
