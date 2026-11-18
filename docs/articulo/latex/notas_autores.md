# Notas para los autores (no se envían a la revista)

Material de trabajo que salió del manuscrito al pasarlo a formato de artículo.

## Datos por completar en el manuscrito

Están marcados en rojo en `manuscrito.tex`:

- Autores, filiación (facultad y grupo) y autor de correspondencia.
- Financiación (convocatoria y código del proyecto) y conflictos de interés.
- Contribución de cada autor (taxonomía CRediT).
- Repositorio público y DOI para la disponibilidad de datos y código.
- Confirmar la frase de responsabilidad en la declaración de uso de
  inteligencia artificial, después de que los autores revisen el texto.
- Revista objetivo. Al elegirla, ajustar el formato (límite de palabras, de
  tablas y figuras, estilo de referencias y si pide resumen en inglés).

## Auditoría de las referencias (18 de noviembre de 2026)

Cada referencia con DOI se comprobó contra Crossref y, si existía, contra
PubMed: autores, título, revista, año, volumen, páginas, retractaciones y
correcciones. Las que no tienen DOI se comprobaron en el sitio de la
institución emisora o en el documento original.

| Referencia | Observación |
|---|---|
| Stevens 2014 (IDSA, piel) | El borrador anterior citaba el DOI 10.1093/cid/ciu444, que es el del resumen ejecutivo. Se corrigió al de la guía completa, 10.1093/cid/ciu296 (e10–e52). |
| Espitaleta 2023 (consenso colombiano de infección urinaria) | Crossref y OpenAlex registran un solo autor (Baquero-Rodríguez); el PDF lista como primeros autores a Espitaleta y Figueroa. Se siguió el PDF. Conviene avisar a la revista del error de metadatos. |
| Guías de ACORL 2024 | Volumen 52, número 4, de 2024; Crossref registra la publicación en línea el 28 de febrero de 2025. |
| Buettcher 2021 (consenso suizo) | Tiene fe de erratas (10.1007/s00431-020-03820-3); no afecta los datos usados. |
| Hospital Infantil de México 2022 y Sociedad Argentina de Pediatría | Crossref no registra la página final. Los documentos argentinos no tienen autores personales en Crossref; se citan el comité o los autores que registra PubMed. |
| SADI 2024 | Crossref registra tres autores y no da páginas. |
| Gaddey 2019 y Sakulchit 2017 | Sin DOI; se comprobaron en PubMed. |
| Flórez 2015 | Metadatos comprobados en Crossref; el servidor rechazó la descarga del texto. Se cita para identificar el documento, no por su contenido. |
| Resolución 2465 de 2016 | Texto consultado en el normograma de la Superintendencia Nacional de Salud; no se obtuvo el anexo técnico oficial. |

## Revisión crítica simulada

**Críticas mayores**

1. *Un solo revisor, automatizado.* Respuesta: se declara como limitación
   principal y todas las decisiones están en archivos auditables. Acción:
   que dos miembros del equipo revisen una muestra aleatoria del cribado y
   todos los esquemas incorporados, e informar el acuerdo entre revisores.
   Un revisor de la revista probablemente lo pida antes de aceptar.
2. *Más de la mitad de los textos no se obtuvo.* Acción: obtenerlos por la
   biblioteca de la universidad y repetir la extracción.
3. *Verificación circular.* El verificador y la segunda implementación
   parten de la misma descripción de reglas que el motor. Las comprobaciones
   C se derivan de principios de las guías, no del código. Acción: cálculo
   manual independiente de los casos tipo.
4. *El tope por medicamento no proviene de guías pediátricas.* Se declara
   como decisión de diseño; cada tope tiene fuente y el resultado lo advierte.

**Críticas menores**

1. La razón R mezcla esquemas de carga y de mantenimiento; conviene
   presentarla también por tipo de esquema.
2. La comparación con tablas publicadas podría extenderse a cefalexina,
   trimetoprim-sulfametoxazol y metronidazol.
3. La población modelada podría variar también la talla.
4. Falta una lista de verificación PRISMA-ScR completa; la mayoría de las
   revistas la piden como archivo adicional.

## Pendientes que quedan para después (parte clínica)

- Doble verificación clínica de los 113 esquemas y de los 28 topes.
- Decisión sobre los diez esquemas sin tope.
- Aclarar la ceftriaxona del lineamiento meningocócico y las ambigüedades
  de las guías colombianas de rinosinusitis y piel.
- Elegir entre las fuentes de ajuste renal.
- Búsqueda en LILACS y SciELO desde su interfaz web.
- Evaluación de usabilidad con profesionales.
