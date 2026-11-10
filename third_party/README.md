# Bibliotecas de terceros

Se incluyen dentro del repositorio (no se descargan al compilar) para que la
construcción funcione sin red y el depósito del código fuente sea autocontenido.

| Biblioteca | Versión | Licencia | Uso | Origen |
|---|---|---|---|---|
| nlohmann/json | 3.11.3 | MIT (`nlohmann_json/LICENSE.MIT`) | Lectura de la base de conocimiento y escritura de reportes | https://github.com/nlohmann/json |
| doctest | 2.4.11 | MIT (`doctest/LICENSE.txt`) | Pruebas automáticas (no se distribuye con la aplicación) | https://github.com/doctest/doctest |

## Dependencia externa (no incluida en el repositorio)

| Biblioteca | Versión | Licencia | Uso |
|---|---|---|---|
| Qt (módulos Core, Gui, Widgets) | 6.5 o superior; desarrollado con 6.8.3 | LGPL v3 | Interfaz gráfica `calc-gui` |

Qt se enlaza de forma dinámica y se instala aparte (https://www.qt.io). Al
distribuir la aplicación hay que cumplir la LGPL: entregar las bibliotecas de
Qt como archivos separados, incluir el texto de la licencia y permitir que el
usuario las reemplace. El núcleo (`calc_core`) y la consola no dependen de Qt.

Para actualizar una biblioteca: reemplazar el archivo de cabecera por el de la
nueva versión, actualizar esta tabla y ejecutar todas las pruebas.

Estas licencias deben citarse en el manual técnico.
