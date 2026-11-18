# Patrones de crecimiento de la OMS

Tablas LMS usadas para modelar la población de prueba (`../poblacion.py`).
Colombia adoptó estos patrones para niños y adolescentes de 0 a 18 años
(Resolución 2465 de 2016 del Ministerio de Salud y Protección Social).

| Archivo | Contenido | Origen |
|---|---|---|
| `weianthro.txt` | Peso para la edad, 0 a 1826 días (patrón de 2006) | Paquete `anthro` de la OMS: `data-raw/growthstandards/weianthro.txt` en github.com/WorldHealthOrganization/anthro |
| `lenanthro.txt` | Longitud o talla para la edad, 0 a 1826 días (patrón de 2006) | Mismo paquete: `lenanthro.txt` |
| `oms_2007_5_a_19.csv` | Índice de masa corporal y talla para la edad, 61 a 228 meses (referencia de 2007) | Tablas ampliadas de la OMS (`bmi-…-z-who-2007-exp.xlsx` y `hfa-…-z-who-2007-exp.xlsx`), convertidas a CSV sin cambiar los valores |

Descargadas el 18 de noviembre de 2026. Los datos son de la Organización
Mundial de la Salud; aquí solo se copian para que la prueba sea reproducible.
