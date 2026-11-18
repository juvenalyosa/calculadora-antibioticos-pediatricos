# Codificación de guías de dosificación de antibióticos pediátricos en una calculadora determinista: síntesis estructurada de la evidencia, concordancia entre guías y verificación computacional

**Tipo de documento:** documento de trabajo (*working paper*). Borrador 1, 18 de noviembre de 2026.

**Autores:** [POR COMPLETAR POR EL EQUIPO INVESTIGADOR]

**Filiación y correspondencia:** [POR COMPLETAR]

**Revista objetivo:** [POR DEFINIR; el texto sigue la estructura IMRyD y el estilo Vancouver]

**Advertencia.** La herramienta descrita es un prototipo académico. Su contenido clínico no ha tenido verificación por profesionales clínicos ni se ha probado con pacientes. Ningún resultado de este documento demuestra seguridad ni eficacia clínica.

## Resumen

**Introducción.** Los errores de dosificación son frecuentes en pediatría y las calculadoras de dosis reducen los errores de cálculo en estudios de simulación. Convertir las guías en reglas calculables exige resolver diferencias entre guías y vacíos en sus recomendaciones.

**Objetivo.** Describir la síntesis de evidencia que alimenta una calculadora determinista de dosis de antibióticos para infecciones pediátricas prevalentes, cuantificar la concordancia entre guías y los vacíos de información, y verificar computacionalmente el motor de cálculo.

**Métodos.** Síntesis estructurada sin metaanálisis de 31 documentos reunidos por el equipo clínico (muestra por conveniencia), completada con búsquedas dirigidas de guías colombianas e internacionales, fichas técnicas y el registro sanitario colombiano. La extracción la hizo un modelo de lenguaje supervisado por el equipo de desarrollo, con una segunda lectura independiente automatizada. Las reglas se codificaron en un motor en C++ y se compararon con una segunda implementación independiente en dos pacientes tipo, con función renal normal y reducida.

**Resultados.** El corpus incluyó 7 guías o consensos, 5 revisiones sistemáticas con metaanálisis, 2 ensayos aleatorizados y 17 documentos de otros diseños; solo 8 aportaron reglas calculables. No fue metodológicamente válido combinar sus resultados en una estimación nueva. La base quedó con 41 entidades clínicas, 88 esquemas y 31 medicamentos, todos sin verificación clínica. La dosis diaria de amoxicilina recomendada para una misma indicación varió entre documentos por un factor de 1,8 a 2,25. De 79 esquemas calculados por peso, 33 tenían dosis máxima en su propia fuente, 21 la tomaron de otra y 25 no tenían ninguna. Las fuentes de ajuste renal discreparon en 12 de 16 medicamentos o formas. La segunda lectura de 38 esquemas no encontró errores numéricos. El motor coincidió con la segunda implementación en los 321 escenarios.

**Conclusiones.** Es factible codificar las guías en un motor trazable que reproduce un cálculo independiente, pero la variabilidad entre guías, la falta de dosis máximas y la discordancia en el ajuste renal obligan a tomar decisiones clínicas explícitas. La validación clínica está pendiente.

**Palabras clave:** antibacterianos; pediatría; cálculo de dosis; sistemas de apoyo a decisiones clínicas; guías de práctica clínica; errores de medicación.

## 1. Introducción

En pediatría la dosis de la mayoría de los medicamentos se calcula para cada paciente a partir del peso y la edad, lo que multiplica las oportunidades de error. Una revisión sistemática de 16 estudios encontró que el error de dosis era el tipo de error de medicación más frecuente en 11 de ellos y el segundo en otros 3 [1]. En un estudio de 10 778 órdenes médicas de 1120 niños hospitalizados en dos instituciones académicas, el 5,7 % tuvo algún error; los eventos adversos potenciales fueron tres veces más frecuentes que en un estudio previo en adultos, el 79 % ocurrió al ordenar el medicamento, el 34 % correspondió a dosis incorrectas y el 28 % involucró antiinfecciosos [2]. Un metaanálisis posterior de 71 estudios confirmó que los errores de medicación son frecuentes en niños hospitalizados, con estimaciones muy variables según el servicio y el método de detección [3].

Los antibióticos concentran buena parte de ese riesgo porque se prescriben mucho. En consultas ambulatorias de Estados Unidos (todas las edades) se estimaron 506 prescripciones de antibióticos por 1000 habitantes al año, de las cuales 353 se consideraron apropiadas; la sinusitis, la otitis media supurativa y la faringitis fueron los diagnósticos con más prescripciones [4]. El uso inadecuado contribuye a la resistencia bacteriana, a la que se atribuyeron 1,14 millones de muertes en el mundo en 2021 [5].

Las herramientas informáticas de apoyo a la prescripción se han propuesto como respuesta. Una revisión de su uso en pediatría concluyó que las calculadoras de dosis disminuyeron los errores de cálculo y que el apoyo a la dosificación redujo los eventos adversos, aunque con tasas altas de alertas ignoradas [6]. Dos estudios experimentales aportan estimaciones directas. En un estudio controlado no aleatorizado con 77 profesionales, una calculadora añadida al formulario pediátrico neerlandés redujo la probabilidad de error de cálculo de 39,0 % a 24,4 % (diferencia de 14,6 puntos; IC 95 %: 3,1 a 26,2) [7]. En un ensayo aleatorizado de simulación con 52 profesionales, un sistema suizo de apoyo a la dosificación redujo la probabilidad de una dosis errónea frente a la ficha técnica con calculadora de bolsillo (razón de momios 0,08; IC 95 %: 0,02 a 0,36) y el tiempo empleado en 45 % [8].

Esa evidencia tiene límites que importan para el diseño de una herramienta nueva. Ambos estudios midieron errores en casos simulados o cuestionarios, no desenlaces en pacientes [7,8]. En el primero, la diferencia dejó de ser significativa cuando el error se definió como una desviación de 10 % o más, y la calculadora introdujo tipos de error nuevos, como los de transcripción [7]. En el segundo, consultar la base de datos sin usar su calculadora no redujo los errores de forma sustancial [8], lo que sugiere que el beneficio depende del cálculo automatizado y no solo del acceso a la información. Además, una calculadora solo es tan buena como las reglas que contiene, y las guías no siempre coinciden: para la neumonía adquirida en la comunidad tratada de forma ambulatoria, la guía finlandesa recomienda amoxicilina a 40-45 mg/kg/día como dosis habitual [9], mientras la guía estadounidense recomienda 90 mg/kg/día [10]. Para el ajuste por función renal, las recomendaciones pediátricas se derivan en su mayoría de datos de adultos [11,12].

Las herramientas evaluadas en esos estudios están ligadas a formularios nacionales de los Países Bajos y Suiza [7,8]. En una búsqueda dirigida, que no fue sistemática, no identificamos una descripción publicada de cómo se convierten en reglas calculables las guías vigentes en Colombia ni de cuántas decisiones quedan sin resolver al hacerlo. No afirmamos que no exista; la pregunta de qué tan completas y concordantes son las fuentes para ese propósito queda, hasta donde pudimos comprobar, sin una respuesta cuantitativa.

Este trabajo describe el desarrollo de una calculadora determinista de dosis de antibióticos para infecciones pediátricas prevalentes. Sus objetivos fueron: (a) sintetizar la evidencia reunida por el equipo clínico y completarla con guías colombianas e internacionales; (b) cuantificar la concordancia entre guías y los vacíos que impiden el cálculo directo; y (c) verificar que el motor reproduce un cálculo independiente. La hipótesis de trabajo fue que las recomendaciones pueden codificarse en un motor trazable que coincide por completo con una segunda implementación, y que la variabilidad entre fuentes es cuantificable y obliga a decisiones clínicas explícitas. El estudio no evalúa desenlaces clínicos.

## 2. Métodos

### 2.1 Diseño

Estudio de desarrollo y verificación de software clínico, con una síntesis estructurada de documentos sin metaanálisis, descrita según los elementos aplicables de la guía SWiM [13]. No es una revisión sistemática: no hubo protocolo registrado ni búsqueda exhaustiva.

### 2.2 Fuentes de información

**Corpus inicial.** El equipo clínico del proyecto reunió 31 documentos en formato PDF. Es una muestra por conveniencia; no se documentó su estrategia de búsqueda.

**Búsquedas dirigidas.** Entre el 17 y el 18 de noviembre de 2026 se buscaron, sin protocolo sistemático, cuatro tipos de fuentes para los vacíos detectados: guías colombianas, guías internacionales de acceso abierto, fichas técnicas de agencias reguladoras con dosificación en insuficiencia renal y el registro sanitario colombiano. Solo se usaron documentos cuyo texto completo se pudo abrir. Las fuentes que no se pudieron abrir (entre ellas *BNF for Children* y el *Red Book*) no aportaron ningún dato.

**Disponibilidad en Colombia.** Se consultó el conjunto de datos abiertos «Código Único de Medicamentos Vigentes» del INVIMA, actualizado el 16 de noviembre de 2026 [18].

### 2.3 Extracción y codificación de los datos

La extracción la realizó un modelo de lenguaje de propósito general (Claude, Anthropic) operado por el equipo de desarrollo; no la hicieron dos revisores humanos independientes. Para cada esquema se registraron el medicamento, la vía, la forma de expresar la dosis (por kilogramo y día, por kilogramo y toma, fija o sin cálculo), los valores mínimo y máximo, el intervalo, las dosis máximas por toma y por día, la duración, los límites de edad o peso, y la página o tabla de la fuente.

Reglas de codificación:

- Donde existía guía colombiana, se registró como primera opción y las internacionales como alternativas.
- Cuando la fuente daba un rango de dosis, el cálculo usa el extremo superior y el resultado muestra el rango completo.
- Cuando la fuente no indicaba una dosis máxima, se dejó sin máximo o se tomó de otra fuente; en ese caso la nota del esquema lo declara.
- Todo esquema quedó en estado «borrador». El programa solo retira la advertencia de «sin verificar» cuando dos personas distintas registran su revisión.

Una segunda instancia del mismo modelo, sin acceso a las conclusiones de la primera, comparó los 38 esquemas de una versión intermedia con los documentos originales. Las tablas y figuras se leyeron sobre imágenes de las páginas, no sobre el texto extraído. Esa lectura es una auditoría automatizada y no sustituye la doble verificación clínica.

### 2.4 Síntesis

Los documentos se clasificaron por diseño e infección. Las estimaciones de efecto de las revisiones sistemáticas y de los ensayos del corpus se presentan tal como fueron publicadas. No se combinaron en una estimación nueva por tres razones: el corpus no proviene de una búsqueda sistemática; las preguntas, poblaciones y desenlaces difieren entre documentos; y ninguna pregunta tenía más de dos ensayos primarios dentro del corpus. Donde el texto traía los conteos, la medida de efecto se recalculó (riesgo relativo con intervalo por el método logarítmico; diferencia de riesgos con intervalo de Wald) para comprobar su reproducibilidad.

La concordancia entre guías se midió con la dosis diaria de amoxicilina oral recomendada en tres indicaciones, como razón entre el valor más alto y el más bajo de los documentos.

### 2.5 Motor de cálculo

El motor, escrito en C++17, aplica cuatro pasos en orden fijo: dosis base según el peso; dosis máxima por toma y por día; ajuste por función renal; y conversión a la presentación comercial, con redondeo a la mínima fracción medible. Una presentación se marca como no adecuada si el redondeo desvía la dosis más de 10 %. Cada resultado incluye el desglose del cálculo y la fuente. El motor es determinista: no usa aleatoriedad, red ni reloj.

La tasa de filtración glomerular se digita o se estima con la ecuación CKiD U25 [14] o con la de Schwartz de cabecera [15]. El ajuste renal se codificó por bandas de filtración a partir de fichas técnicas armonizadas de la Agencia Europea de Medicamentos [16] y del formulario pediátrico neerlandés [17].

### 2.6 Verificación

Se escribió una segunda implementación en Python a partir de la descripción escrita de las reglas, sin reutilizar código del motor. Ambas se compararon en dos pacientes tipo definidos en el protocolo del proyecto (12 meses, 10 kg, 75 cm; y 13 años, 50 kg, 162 cm), en todos los esquemas aplicables, con función renal normal y con filtración de 40, 20 y 5 mL/min/1,73 m² en los medicamentos con bandas de ajuste. Se exigió igualdad de la dosis por toma (tolerancia relativa de 0,1 %), del intervalo y de la cantidad de la presentación. Además se ejecutaron pruebas unitarias sobre una base sintética, y el conjunto se compiló y probó en Linux, macOS y Windows.

### 2.7 Consideraciones éticas y disponibilidad

No participaron pacientes ni se usaron datos identificables. El código, las hojas de datos, la matriz de verificación y el programa que genera las cifras de este documento (`docs/articulo/analisis.py`) están en el repositorio del proyecto [DISPONIBILIDAD PÚBLICA POR DEFINIR].

## 3. Resultados

### 3.1 Composición del corpus

Los 31 documentos fueron heterogéneos en diseño y tema (tabla 1). Doce trataban sobre otitis, 7 sobre neumonía e infecciones respiratorias bajas, 5 sobre faringoamigdalitis, 1 sobre gastroenteritis, 1 sobre infecciones de piel y 5 sobre uso de antimicrobianos en general. Solo 8 aportaron reglas que la calculadora pudiera usar: 7 guías o consensos [9,28,29,39,44-46] y una revisión de evidencia [31].

**Tabla 1. Diseño de los 31 documentos del corpus inicial**

| Diseño | n |
|---|---|
| Guía, consenso o recomendación de sociedad científica | 7 |
| Revisión sistemática con metaanálisis | 5 |
| Ensayo clínico aleatorizado | 2 |
| Protocolo de ensayo clínico | 1 |
| Estudio observacional o cuasiexperimental | 6 |
| Revisión narrativa o de evidencia | 7 |
| Serie de casos | 1 |
| Otros (hoja informativa para pacientes; desarrollo de una aplicación) | 2 |

### 3.2 Estimaciones de efecto publicadas en el corpus

La tabla 2 resume las estimaciones de las cinco revisiones sistemáticas y los dos ensayos. Tres hallazgos son pertinentes para la calculadora. Primero, en la neumonía tratada de forma ambulatoria, los cursos de 3 a 5 días no difirieron de los de 7 a 10 días en fracaso del tratamiento [19]. Segundo, en la faringoamigdalitis estreptocócica no hubo diferencias clínicamente importantes entre clases de antibióticos, con certeza baja [21], y la penicilina V durante 5 días fue no inferior a 10 días en curación clínica, aunque con menor erradicación bacteriológica [24]. Tercero, para la foliculitis y los forúnculos la evidencia fue insuficiente para comparar tratamientos [23].

**Tabla 2. Estimaciones publicadas en las revisiones sistemáticas y ensayos del corpus**

| Estudio | Comparación y población | Estudios (participantes) | Resultado principal, como fue publicado |
|---|---|---|---|
| Kuitunen 2023 [19] | Curso corto (3-5 días) frente a largo (7-10 días); neumonía ambulatoria, niños de 6 meses o más | 4 (1541) | Fracaso del tratamiento: 7,9 % frente a 8,0 %; diferencia de riesgos 0,00 (IC 95 %: −0,03 a 0,02) |
| Lassi 2021 [20] | Amoxicilina frente a placebo; neumonía no grave con sibilancias, 2 a 59 meses | 3 (3222) | Fracaso del tratamiento: RR 0,80 (IC 95 %: 0,68 a 0,94); certeza baja |
| Hedin 2023 [21] | Cefalosporinas frente a penicilina; faringitis estreptocócica | 5 (2018) | Ausencia de resolución de síntomas: OR 0,79 (IC 95 %: 0,55 a 1,12); certeza baja |
| Hedin 2023 [21] | Macrólidos frente a penicilina; faringitis estreptocócica | 6 (1728) | Resolución de síntomas: OR 1,11 (IC 95 %: 0,92 a 1,35); certeza baja |
| Dawit 2021 [22] | Azitromicina frente a amoxicilina-clavulanato; otitis media | 14 | Eficacia al terminar el tratamiento: OR 0,75 (IC 95 %: 0,62 a 0,91) |
| Lin 2021 [23] | Antibióticos orales entre sí; foliculitis y forúnculos | 18 (1300) | Sin diferencias demostradas; certeza baja o muy baja |
| Skoog Ståhlgren 2019 [24] | Penicilina V 5 días frente a 10 días; faringoamigdalitis estreptocócica, 6 años o más | 1 (433) | Curación clínica: 89,6 % frente a 93,3 % (IC 95 % de la diferencia: −9,7 a 2,2); erradicación: 80,4 % frente a 90,7 % |
| Hullegie 2024 [25] | Gotas óticas frente a amoxicilina oral; otitis media con otorrea | 1 (58 de 350 previstos) | Resolución a 3 días: 42 % frente a 65 %; terminado antes de tiempo, no concluyente |

OR: razón de momios; RR: riesgo relativo.

El recálculo con los conteos impresos reprodujo el riesgo relativo de retratamiento de Kuitunen (1,07; IC 95 %: 0,74 a 1,55) y dos de las tres comparaciones de un solo estudio de Lin. En la tercera (cefdinir frente a cefalexina; 25/32 frente a 32/42) se obtuvo 1,03 (IC 95 %: 0,80 a 1,32), frente al 1,00 (0,73 a 1,38) publicado. Para Skoog Ståhlgren, la diferencia de curación clínica fue de −3,7 puntos con un intervalo de Wald de −9,2 a 1,7, más estrecho que el publicado, que usa otro método. En el resumen de Dawit, el intervalo de la razón de momios excluye el 1, pero el texto la describe como una diferencia no significativa; ese resultado debe leerse con cautela.

### 3.3 Contenido de la base de conocimiento

Tras las búsquedas dirigidas la base quedó con 41 entidades clínicas (29 con indicación de antibiótico y 12 en las que se indica no darlo), 88 esquemas, 31 medicamentos, 47 presentaciones y 31 fuentes (tabla 3). De los 88 esquemas, 49 citan una guía colombiana [26,27,35,37,40,41,47], 25 un documento del corpus inicial y 14 otra guía internacional [10,32,38,42,43]. De ellos, 79 se calculan por peso, 5 son dosis fijas y 4 son indicaciones tópicas sin cálculo. Los 88 están en estado «borrador».

**Tabla 3. Entidades clínicas y esquemas por grupo**

| Grupo | Entidades | Con antibiótico | Sin antibiótico |
|---|---|---|---|
| Respiratorias altas | 10 | 6 | 4 |
| Respiratorias bajas | 7 | 4 | 3 |
| Gastrointestinales | 8 | 7 | 1 |
| Piel y tejidos blandos | 6 | 5 | 1 |
| Urinarias | 4 | 3 | 1 |
| Boca y dientes | 3 | 2 | 1 |
| Fiebre y enfermedad grave | 2 | 1 | 1 |
| Ojos | 1 | 1 | 0 |
| **Total** | **41** | **29** | **12** |

### 3.4 Concordancia entre guías

La dosis diaria de amoxicilina recomendada para una misma indicación difirió entre documentos (tabla 4). La razón entre el valor más alto y el más bajo fue de 2,0 en la otitis media aguda, 2,25 en la neumonía ambulatoria y 1,8 en la faringoamigdalitis estreptocócica. En las tres indicaciones las guías colombianas coincidieron con la mayoría de las internacionales.

**Tabla 4. Dosis diaria de amoxicilina oral recomendada, por indicación y documento**

| Indicación | Documento | mg/kg/día | Tomas al día |
|---|---|---|---|
| Otitis media aguda | ACORL 2024, Colombia [26] | 90 | 2 |
| | AIEPI 2012, Colombia [27] | 90 | 2 |
| | Consenso italiano 2025 [28] | 80-90 | 3 |
| | Sociedad Italiana de Pediatría 2019 [29] | 80-90 | 3 |
| | AAP 2013, Estados Unidos [30] | 80-90 | 2 |
| | AAFP 2019, Estados Unidos [31] | 80-90 | 2 |
| | OMS 2022 [32] | 80-90 | 2 o 3 |
| | Sakulchit y Goldman 2017, Canadá [33] | 45-60 o 75-90 | 3 o 2 |
| | Guía israelí descrita por Rothman 2018 [34] | 60-80 | 2 o 3 |
| Neumonía ambulatoria | Guía 42, Colombia 2014 [35] | 90 | No indicado |
| | AIEPI 2012, Colombia [27] | 90 | 2 |
| | PIDS/IDSA 2011, Estados Unidos [10] | 90 | 2 |
| | OMS 2014 [36] | 80 | 2 |
| | Guía finlandesa 2023, dosis habitual [9] | 40-45 | 3 |
| | Guía finlandesa 2023, dosis alta [9] | 80-90 | 3 |
| Faringoamigdalitis estreptocócica | ACORL 2024, Colombia [37] | 50 | 1 o 2 |
| | IDSA 2012, Estados Unidos [38] | 50 | 1 o 2 |
| | Consenso italiano 2024 [39] | 50 | 2 |
| | OMS 2022 [32] | 80-90 | 2 o 3 |

Esa diferencia se traslada sin atenuación al resultado. Para el paciente tipo de 10 kg con neumonía, el motor entrega 900 mg al día con la guía colombiana y 450 mg al día con la dosis habitual finlandesa; para el de 50 kg, 4000 mg (limitado por la dosis máxima) y 2250 mg.

### 3.5 Vacíos de las fuentes

**Dosis máximas.** De los 79 esquemas calculados por peso, 33 tenían una dosis máxima en la fuente citada, 21 la tomaron de otra fuente y 25 quedaron sin ninguna. En 28 la fuente daba un rango en lugar de un valor.

**Ajuste renal.** Las guías clínicas del corpus no traían reglas de ajuste renal para ningún antibiótico. Con fichas técnicas y el formulario neerlandés se codificaron reglas para 12 medicamentos, 3 se registraron como «no requiere ajuste» y 16 quedaron sin dato. En la comparación de fuentes para 15 antibióticos (16 al separar las formas oral e intravenosa de la cefuroxima), 12 presentaron al menos una discrepancia sobre el umbral, la regla o la población. El caso de la amoxicilina muestra la magnitud: con una filtración de 20 mL/min/1,73 m², la ficha técnica europea indica 15 mg/kg por toma cada 12 horas [16] y el formulario neerlandés no ajusta por encima de 10 [17]. Para el paciente de 10 kg con otitis, eso equivale a 150 mg o a 450 mg por toma.

**Disponibilidad.** El ácido nalidíxico, primera opción de las guías colombianas para la disentería [27], no tiene registro sanitario vigente. La nitrofurantoína, recomendada para la infección urinaria baja [40], solo está registrada en cápsulas o tabletas de 100 mg, y la clindamicina no tiene forma líquida oral [18].

**Ambigüedades de las fuentes.** Varias recomendaciones no pudieron codificarse sin interpretación: una guía no precisa a qué componente se refiere la dosis de trimetoprim-sulfametoxazol [40]; otra escribe la ceftriaxona en miligramos sin indicar «por kilogramo» en su tabla [26]; y un máximo impreso para la gentamicina (2000 mg al día) no es verosímil [40].

### 3.6 Verificación

La segunda lectura automatizada de los 38 esquemas de la versión intermedia clasificó 26 como concordantes, 7 como parcialmente concordantes, 1 como discrepante y 4 como no verificables, porque su fuente no estaba entre los documentos disponibles. No encontró errores en ningún valor numérico; las 8 observaciones se referían a notas, a la línea de tratamiento asignada o a una dosis máxima no respaldada por la recomendación citada. Las 8 se corrigieron y las 4 no verificables se reemplazaron por datos de la guía original, obtenida después [38].

El motor coincidió con la segunda implementación en los 321 escenarios (155 con función renal normal y 166 con ajuste renal; 277 con comprobación de la cantidad de la presentación), que cubren las 26 entidades con al menos un esquema calculable. Las 53 pruebas unitarias (5266 comprobaciones) pasaron en los tres sistemas operativos.

## 4. Discusión

Este trabajo muestra tres resultados. Las recomendaciones de 20 documentos pudieron codificarse en un motor trazable que reproduce un cálculo independiente en todos los escenarios probados. La información necesaria para calcular no está completa en las guías: más de la mitad de las dosis por peso (46 de 79) no tiene dosis máxima en su propia fuente y ninguna guía clínica del corpus trae el ajuste renal. Y las fuentes discrepan lo suficiente para duplicar o reducir a un tercio la dosis resultante en un mismo paciente.

**Relación con la literatura.** Los estudios que evaluaron calculadoras de dosis midieron el error de cálculo del usuario [7,8]; el presente trabajo mide un eslabón anterior, la conversión de las guías en reglas. Los dos planos son complementarios: una calculadora puede eliminar el error aritmético y aun así entregar una dosis discutible si la regla codificada lo es. La observación de que una calculadora introduce errores nuevos [7] es coherente con nuestra decisión de mostrar el desglose completo del cálculo, aunque el efecto de esa decisión sobre el usuario no se evaluó aquí. La discordancia en el ajuste renal concuerda con dos revisiones que señalan que las recomendaciones pediátricas se extrapolan de adultos [11,12].

**Interpretación de la variabilidad entre guías.** La diferencia en la dosis de amoxicilina para la neumonía probablemente refleja la epidemiología local de la resistencia del neumococo; la guía finlandesa reserva la dosis alta para la sospecha de neumococo resistente [9]. Esta es una hipótesis explicativa, no un resultado de este estudio. Para una herramienta colombiana, la consecuencia práctica es que la elección de la guía de referencia es una decisión clínica que debe quedar documentada, no un detalle de programación.

**Sobre la síntesis.** No se presenta una estimación combinada nueva. Las revisiones del corpus ya combinan los ensayos disponibles para sus preguntas, y agregar documentos de diseños y preguntas distintos habría producido una cifra sin interpretación. El recálculo aritmético reprodujo la mayoría de las medidas y detectó una diferencia menor y una inconsistencia entre cifra y texto, lo que respalda leer las estimaciones en su fuente y no solo en su resumen.

**Fortalezas.** Cada dato conserva su fuente y su página; las decisiones tomadas donde las fuentes callan o discrepan están escritas en la nota de cada esquema; la verificación usa una implementación independiente; y los análisis son reproducibles con los archivos del proyecto.

**Limitaciones.** Son sustanciales y condicionan toda conclusión:

1. No hay validación clínica. Ningún esquema ha sido revisado por dos profesionales, que es el criterio que el propio programa exige.
2. La extracción y la segunda lectura las hizo un modelo de lenguaje. La concordancia entre ambas no equivale a exactitud: las dos pueden compartir el mismo error de lectura.
3. El corpus es una muestra por conveniencia y las búsquedas no fueron sistemáticas. No se puede afirmar que estén todas las guías pertinentes.
4. La verificación computacional demuestra que dos programas coinciden, no que la regla sea correcta. Ambos se escribieron a partir de la misma descripción.
5. Solo se probaron dos pacientes tipo. No se exploraron sistemáticamente pesos extremos, prematuros ni obesidad.
6. Veintiuna dosis máximas provienen de una fuente distinta a la del esquema, a veces de otra indicación.
7. Las bandas de ajuste renal mezclan fuentes que expresan la filtración en mL/min y en mL/min/1,73 m², y la ecuación CKiD U25 se desarrolló en niños y jóvenes con enfermedad renal crónica; sus autores no la validaron en población sin enfermedad renal [14].
8. No se evaluó la usabilidad ni el efecto de la herramienta sobre los errores de prescripción.

**Lo que este estudio no permite afirmar.** No demuestra que la calculadora reduzca errores, que sus dosis sean las correctas para un paciente dado ni que sea segura para uso clínico.

## 5. Conclusiones

Respecto al primer objetivo, la evidencia reunida se sintetizó en una base de 88 esquemas para 41 entidades clínicas, con las guías colombianas como primera opción donde existen; el corpus inicial no permitió un metaanálisis nuevo y sus estimaciones se presentan como fueron publicadas. Respecto al segundo, la dosis recomendada de un mismo antibiótico para una misma indicación varió hasta 2,25 veces entre documentos, 46 de las 79 dosis por peso no tenían máximo en su propia fuente y las fuentes de ajuste renal discreparon en tres de cada cuatro medicamentos. Respecto al tercero, el motor reprodujo una implementación independiente en los 321 escenarios probados.

El trabajo aporta un inventario explícito de las decisiones clínicas que la conversión de guías en reglas deja abiertas. Antes de cualquier uso con pacientes se requiere la doble verificación clínica de cada esquema, la elección documentada de las fuentes de referencia, el cálculo manual independiente de los casos tipo y una evaluación con usuarios.

## Declaraciones

**Uso de inteligencia artificial.** Un modelo de lenguaje (Claude, Anthropic) se usó para extraer datos de los documentos, escribir el código, ejecutar las búsquedas dirigidas, realizar la segunda lectura automatizada y redactar este borrador. Los autores deben revisar y asumir la responsabilidad de todo el contenido antes de enviarlo a una revista.

**Financiación:** [POR COMPLETAR]

**Conflictos de interés:** [POR COMPLETAR]

**Contribución de los autores:** [POR COMPLETAR]

## Referencias

1. Wong IC, Ghaleb MA, Franklin BD, et al. Incidence and nature of dosing errors in paediatric medications: a systematic review. Drug Saf. 2004;27(9):661-670. doi:10.2165/00002018-200427090-00004
2. Kaushal R, Bates DW, Landrigan C, et al. Medication errors and adverse drug events in pediatric inpatients. JAMA. 2001;285(16):2114-2120. doi:10.1001/jama.285.16.2114
3. Gates PJ, Baysari MT, Gazarian M, et al. Prevalence of medication errors among paediatric inpatients: systematic review and meta-analysis. Drug Saf. 2019;42(11):1329-1342. doi:10.1007/s40264-019-00850-1
4. Fleming-Dutra KE, Hersh AL, Shapiro DJ, et al. Prevalence of inappropriate antibiotic prescriptions among US ambulatory care visits, 2010-2011. JAMA. 2016;315(17):1864-1873. doi:10.1001/jama.2016.4151
5. GBD 2021 Antimicrobial Resistance Collaborators. Global burden of bacterial antimicrobial resistance 1990-2021: a systematic analysis with forecasts to 2050. Lancet. 2024;404(10459):1199-1226. doi:10.1016/S0140-6736(24)01867-1
6. Stultz JS, Nahata MC. Computerized clinical decision support for medication prescribing and utilization in pediatrics. J Am Med Inform Assoc. 2012;19(6):942-953. doi:10.1136/amiajnl-2011-000798
7. van der Zanden TM, de Hoog M, Windster JD, et al. Does a dose calculator as an add-on to a web-based paediatric formulary reduce calculation errors in paediatric dosing? A non-randomized controlled study. Paediatr Drugs. 2020;22(2):229-239. doi:10.1007/s40272-020-00386-3
8. Higi L, Schmitt R, Käser K, et al. Impact of a clinical decision support system on paediatric drug dose prescribing: a randomised within-subject simulation trial. BMJ Paediatr Open. 2023;7(1):e001726. doi:10.1136/bmjpo-2022-001726
9. Backman K, Helminen M, Kekäläinen E, et al. Working group summary of the 2023 full update of the Finnish national guidelines for paediatric lower respiratory tract infections. Acta Paediatr. 2025;114(2):248-257. doi:10.1111/apa.17481
10. Bradley JS, Byington CL, Shah SS, et al. The management of community-acquired pneumonia in infants and children older than 3 months of age: clinical practice guidelines by the Pediatric Infectious Diseases Society and the Infectious Diseases Society of America. Clin Infect Dis. 2011;53(7):e25-e76. doi:10.1093/cid/cir531
11. Schijvens AM, de Wildt SN, Schreuder MF. Pharmacokinetics in children with chronic kidney disease. Pediatr Nephrol. 2020;35(7):1153-1172. doi:10.1007/s00467-019-04304-9
12. Minotti C, Barbieri E, Doni D, et al. Anti-infective medicines use in children and neonates with pre-existing kidney dysfunction: a systematic review. Front Pediatr. 2022;10:868513. doi:10.3389/fped.2022.868513
13. Campbell M, McKenzie JE, Sowden A, et al. Synthesis without meta-analysis (SWiM) in systematic reviews: reporting guideline. BMJ. 2020;368:l6890. doi:10.1136/bmj.l6890
14. Pierce CB, Muñoz A, Ng DK, et al. Age- and sex-dependent clinical equations to estimate glomerular filtration rates in children and young adults with chronic kidney disease. Kidney Int. 2021;99(4):948-956. doi:10.1016/j.kint.2020.10.047
15. Schwartz GJ, Muñoz A, Schneider MF, et al. New equations to estimate GFR in children with CKD. J Am Soc Nephrol. 2009;20(3):629-637. doi:10.1681/ASN.2008030287
16. European Medicines Agency. Amoxil: Article 30 referral, Annex III (summary of product characteristics), section 4.2. Disponible en: https://www.ema.europa.eu/en/documents/referral/amoxil-article-30-referral-annex-iii_en.pdf (consultado el 17 de noviembre de 2026).
17. Nederlands Kenniscentrum voor Farmacotherapie bij Kinderen. Kinderformularium: monografía de amoxicilina, sección de función renal disminuida. Disponible en: https://www.kinderformularium.nl/geneesmiddel/7/amoxicilline (consultado el 17 de noviembre de 2026).
18. Instituto Nacional de Vigilancia de Medicamentos y Alimentos (INVIMA). Código Único de Medicamentos Vigentes [conjunto de datos abiertos]. Actualizado el 16 de noviembre de 2026. Disponible en: https://www.datos.gov.co/d/i7cb-raxc (consultado el 17 de noviembre de 2026).
19. Kuitunen I, Jääskeläinen J, Korppi M, et al. Antibiotic treatment duration for community-acquired pneumonia in outpatient children in high-income countries: a systematic review and meta-analysis. Clin Infect Dis. 2023;76(3):e1123-e1128. doi:10.1093/cid/ciac374
20. Lassi ZS, Padhani ZA, Das JK, et al. Antibiotic therapy versus no antibiotic therapy for children aged 2 to 59 months with WHO-defined non-severe pneumonia and wheeze. Cochrane Database Syst Rev. 2021;1(1):CD009576. doi:10.1002/14651858.CD009576.pub3
21. Hedin K, Thorning S, van Driel ML. Different antibiotic treatments for group A streptococcal pharyngitis. Cochrane Database Syst Rev. 2023;(11):CD004406. doi:10.1002/14651858.CD004406.pub6
22. Dawit G, Mequanent S, Makonnen E. Efficacy and safety of azithromycin and amoxicillin/clavulanate for otitis media in children: a systematic review and meta-analysis of randomized controlled trials. Ann Clin Microbiol Antimicrob. 2021;20(1):28. doi:10.1186/s12941-021-00434-x
23. Lin HS, Lin PT, Tsai YS, et al. Interventions for bacterial folliculitis and boils (furuncles and carbuncles). Cochrane Database Syst Rev. 2021;2(2):CD013099. doi:10.1002/14651858.CD013099.pub2
24. Skoog Ståhlgren G, Tyrstrup M, Edlund C, et al. Penicillin V four times daily for five days versus three times daily for 10 days in patients with pharyngotonsillitis caused by group A streptococci: randomised controlled, open label, non-inferiority study. BMJ. 2019;367:l5337. doi:10.1136/bmj.l5337
25. Hullegie S, Damoiseaux RAMJ, Hay AD, et al. Topical or oral antibiotics in childhood acute otitis media and ear discharge: a randomized controlled non-inferiority trial. Fam Pract. 2024;41(5):857-861. doi:10.1093/fampra/cmae034
26. Ospina-García JC, Téllez-Cortés PA, Baquero-Hoyos MM, et al. Guía de práctica clínica para el diagnóstico y el tratamiento de otitis media aguda. Acta Otorrinolaringol Cir Cabeza Cuello. 2024;52(4):574-591. doi:10.37076/acorl.v52i4.834
27. Ministerio de Salud y Protección Social, Organización Panamericana de la Salud. Atención Integrada a las Enfermedades Prevalentes de la Infancia (AIEPI): libro clínico. 3.ª ed. Bogotá: OPS; 2012. Disponible en: https://www.minsalud.gov.co/sites/rid/Lists/BibliotecaDigital/RIDE/VS/PP/libro-clinico-atencion-integrada-enfermedades.pdf
28. Castelli Gattinara G, Bergamini M, Simeone G, et al. Antibiotic treatment of acute and recurrent otitis media in children: an Italian intersociety consensus. Ital J Pediatr. 2025;51(1):50. doi:10.1186/s13052-025-01894-z
29. Marchisio P, Galli L, Bortone B, et al. Updated guidelines for the management of acute otitis media in children by the Italian Society of Pediatrics: treatment. Pediatr Infect Dis J. 2019;38(12S):S10-S21. doi:10.1097/INF.0000000000002452
30. Lieberthal AS, Carroll AE, Chonmaitree T, et al. The diagnosis and management of acute otitis media. Pediatrics. 2013;131(3):e964-e999. doi:10.1542/peds.2012-3488
31. Gaddey HL, Wright MT, Nelson TN. Otitis media: rapid evidence review. Am Fam Physician. 2019;100(6):350-356.
32. World Health Organization. The WHO AWaRe (Access, Watch, Reserve) antibiotic book. Geneva: WHO; 2022. Disponible en: https://iris.who.int/handle/10665/365237
33. Sakulchit T, Goldman RD. Antibiotic therapy for children with acute otitis media. Can Fam Physician. 2017;63(9):685-687.
34. Rothman S, Pitaro J, Hackett A, et al. Treatment of acute otitis media in the pediatric emergency department. Pediatr Infect Dis J. 2018;37(6):520-525. doi:10.1097/INF.0000000000001837
35. Ministerio de Salud y Protección Social. Guía de práctica clínica para la evaluación del riesgo y manejo inicial de la neumonía en niños y niñas menores de 5 años y bronquiolitis en niños y niñas menores de 2 años. Guía n.º 42. Bogotá: Ministerio de Salud y Protección Social; 2014.
36. World Health Organization. Revised WHO classification and treatment of childhood pneumonia at health facilities: evidence summaries. Geneva: WHO; 2014. Disponible en: https://iris.who.int/handle/10665/137319
37. Romero-Moreno LF, García-Torres N, Izquierdo-Velásquez JC, et al. Guía para el diagnóstico y tratamiento de faringoamigdalitis aguda. Acta Otorrinolaringol Cir Cabeza Cuello. 2024;52(4):533-555. doi:10.37076/acorl.v52i4.812
38. Shulman ST, Bisno AL, Clegg HW, et al. Clinical practice guideline for the diagnosis and management of group A streptococcal pharyngitis: 2012 update by the Infectious Diseases Society of America. Clin Infect Dis. 2012;55(10):e86-e102. doi:10.1093/cid/cis629
39. Chiappini E, Simeone G, Bergamini M, et al. Treatment of acute pharyngitis in children: an Italian intersociety consensus (SIPPS-SIP-SITIP-FIMP-SIAIP-SIMRI-FIMMG). Ital J Pediatr. 2024;50(1):235. doi:10.1186/s13052-024-01789-5
40. Espitaleta Z, Figueroa VH, Meza Martínez AI, et al. Recomendaciones basadas en guías de práctica clínica sobre diagnóstico, tratamiento y seguimiento de infección de tracto urinario en pacientes pediátricos en Colombia. Salud Uninorte. 2023;39(3):1239-1270. doi:10.14482/sun.39.03.456.987
41. Gutiérrez-Tobar IF, Palacios-Ariza MA, Luna-Solarte DA, et al. Guía de práctica clínica colombiana para el tratamiento de infecciones bacterianas de piel y tejidos blandos superficiales en población pediátrica. Infectio. 2023;27(2):114-131. doi:10.22354/24223794.1131
42. Stevens DL, Bisno AL, Chambers HF, et al. Practice guidelines for the diagnosis and management of skin and soft tissue infections: 2014 update by the Infectious Diseases Society of America. Clin Infect Dis. 2014;59(2):e10-e52. doi:10.1093/cid/ciu444
43. McDonald LC, Gerding DN, Johnson S, et al. Clinical practice guidelines for Clostridium difficile infection in adults and children: 2017 update by the Infectious Diseases Society of America (IDSA) and Society for Healthcare Epidemiology of America (SHEA). Clin Infect Dis. 2018;66(7):e1-e48. doi:10.1093/cid/cix1085
44. Cohen R, Raymond J, Gendrel D. Antimicrobial treatment of diarrhea/acute gastroenteritis in children. Arch Pediatr. 2017;24(12S):S26-S29. doi:10.1016/S0929-693X(17)30515-8
45. Subspecialty Group of Respiratory, the Society of Pediatrics, Chinese Medical Association, et al. Evidence-based guideline for the diagnosis and treatment of Mycoplasma pneumoniae pneumonia in children (2023). Pediatr Investig. 2025;9(1):1-11. doi:10.1002/ped4.12469
46. Núñez-Batalla F, Jáudenes-Casaubón C, Sequí-Canet JM, et al. Diagnosis and treatment of otitis media with effusion: CODEPEH recommendations. Acta Otorrinolaringol Esp (Engl Ed). 2019;70(1):36-46. doi:10.1016/j.otorri.2017.07.004
47. Bedoya JD, Vanegas G, Herrera-Noreña P, et al. Guía para el diagnóstico y tratamiento de rinosinusitis en niños. Acta Otorrinolaringol Cir Cabeza Cuello. 2024;52(4):611-636. doi:10.37076/acorl.v52i4.808
