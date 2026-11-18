# Decisiones de integración a la base de conocimiento

Qué se incorporó de la revisión de alcance (versión 0.5.0-borrador de la base)
y qué no, con el motivo. La jerarquía aplicada es la del protocolo: guía
colombiana, luego latinoamericana u OMS, luego internacional reciente. El
programa que aplica estos cambios es `integrar.py`; las citas textuales están
en `esquemas_G*.csv` y `maximos_G*.csv`.

## Sensibilidad de la búsqueda frente al corpus inicial

El corpus reunido por el equipo clínico tenía 31 documentos. La búsqueda
recuperó e incluyó sus 7 guías o consensos (consenso italiano de otitis 2025,
Sociedad Italiana de Pediatría 2019, consenso italiano de faringitis 2024,
guía finlandesa 2023, guía china de micoplasma 2023, guía francesa de
gastroenteritis 2017 y CODEPEH 2019). Los otros 24 son ensayos, revisiones
sistemáticas, revisiones narrativas o estudios observacionales, que la
estrategia excluye por diseño (`corpus_inicial_vs_busqueda.csv`).

## Incorporado

| Cambio | Fuente | Motivo |
|---|---|---|
| **Topes por medicamento y vía** (28) | Guía finlandesa 2023, PIDS/IDSA 2011, Sociedad Canadiense de Pediatría 2014, NICE NG111, consenso suizo 2021, IDSA 2014, SAP 2014, IDSA/SHEA 2017, guía francesa 2017, ficha de la FDA | La prueba exhaustiva mostró dosis irreales en adolescentes pesados cuando el esquema no tiene tope (por ejemplo ceftriaxona 5,5 g/día). El tope del medicamento solo se aplica si el esquema no trae el suyo |
| Meningitis bacteriana (nueva) | OMS AWaRe 2022; Sociedad Canadiense de Pediatría 2014 | El lineamiento colombiano de 2018 es ambiguo en la dosis de ceftriaxona |
| Neumonía complicada (nueva) | SADI 2024 (Argentina); SAP 2021 | No había guía colombiana |
| Rinosinusitis grave hospitalizada (nueva) | ACORL 2024 (Colombia) | Esquemas parenterales de la guía colombiana que no estaban cargados |
| Infección grave de piel por SARM (nueva) | ACIN 2023 (Colombia) | Esquemas intravenosos de la guía colombiana que no estaban cargados |
| Yersiniosis (nueva) | Guía francesa 2017 | Estaba en una guía ya usada y no se había cargado |
| C. difficile: metronidazol por dosis y vancomicina en enfermedad grave | IDSA/SHEA 2017 | Dosis y topes más recientes que los de la guía francesa |
| Shigelosis: ceftriaxona de AIEPI (2 meses a 5 años) | AIEPI 2012 (Colombia) | Único esquema colombiano dosificado con un medicamento registrado |
| Faringoamigdalitis: cefadroxilo en alergia | IDSA 2012; Sociedad Chilena de Infectología 2019 | Alternativa con registro INVIMA |
| Otitis media aguda: clindamicina en alergia de tipo I | ACORL 2024 la recomienda; dosis de AAP 2013 | La guía colombiana no da la dosis |
| Nota de gotas en otitis media crónica | Guía australiana 2021 | AIEPI no da el número de gotas |
| Nota del componente del trimetoprim-sulfametoxazol | Sociedad Chilena de Infectología 2022 | Confirma la interpretación del consenso colombiano |
| Medicamentos nuevos | ampicilina-sulbactam, cefotaxima, levofloxacina, vancomicina intravenosa, linezolid, cefadroxilo | Todos con registro INVIMA vigente |

## No incorporado, con motivo

| Hallazgo | Motivo |
|---|---|
| Chile 2019: amoxicilina 40 a 50 mg/kg/día en otitis y sinusitis | Se justifica por la baja resistencia del neumococo en Chile; la guía colombiana (ACORL 2024) indica 90 |
| Brasil 2024 y México 2022 en otitis y sinusitis | Hay guía colombiana para esas infecciones |
| Consenso italiano 2025: amoxicilina-clavulanato 7:1 más amoxicilina | Hay guía colombiana; queda como discrepancia documentada |
| Esquemas neonatales (ampicilina más gentamicina en menores de 1 mes) | La calculadora no cubre a los menores de 1 mes |
| Daptomicina y ceftarolina (ACIN 2023) | Dosis por tramos de edad y uso especializado; registro INVIMA no verificado |
| Cólera | Dosis única por peso con topes de adulto no comprobados; en Colombia solo aplica en brote |
| Ácido nalidíxico y cefixima | Sin registro sanitario vigente en Colombia |
| Periodontitis y enfermedad periodontal necrosante (EAPD 2025) | Manejo odontológico especializado |
| Antiparasitario para *Dientamoeba* (clioquinol) | No está disponible en Colombia |
| Tos ferina: esquemas de la guía china 2024 | Coinciden en lo esencial con los cargados; difieren en duración |
| Daptomicina, cefazolina, oxacilina, dicloxacilina, ampicilina-sulbactam, cefdinir y gentamicina: tope por medicamento | La revisión no encontró una dosis máxima con fuente; siguen mostrando la advertencia `SIN_TOPE_MAXIMO` |

## Documentos importantes que no se pudieron leer

Acceso cerrado o sin copia: NICE NG240 (meningitis 2024), OMS 2025
(meningitis), IDSA/PIDS 2026 (neumonía, parte 1), consenso SEIP 2025 de
tratamiento etiológico de la gastroenteritis, guía colombiana de enfermedad
diarreica 2015 publicada en revista (se usó la guía del Ministerio de 2013 de
la que procede), ESPGHAN/ESPID 2014, AAP 2026 de infección urinaria. Conviene
obtenerlos por la biblioteca de la universidad y repetir la extracción.
