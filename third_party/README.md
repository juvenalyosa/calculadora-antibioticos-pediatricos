# Bibliotecas de terceros

Se incluyen dentro del repositorio (no se descargan al compilar) para que la
construcción funcione sin red y el depósito del código fuente sea autocontenido.

| Biblioteca | Versión | Licencia | Uso | Origen |
|---|---|---|---|---|
| nlohmann/json | 3.11.3 | MIT (`nlohmann_json/LICENSE.MIT`) | Lectura de la base de conocimiento y escritura de reportes | https://github.com/nlohmann/json |
| doctest | 2.4.11 | MIT (`doctest/LICENSE.txt`) | Pruebas automáticas (no se distribuye con la aplicación) | https://github.com/doctest/doctest |

Para actualizar una biblioteca: reemplazar el archivo de cabecera por el de la
nueva versión, actualizar esta tabla y ejecutar todas las pruebas.

Estas licencias deben citarse en el manual técnico.
