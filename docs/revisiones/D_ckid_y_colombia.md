# D. CKiD U25, Colombian paediatric antibiotic guidance, and presentations registered in Colombia

Date of retrieval: 2026-11-17. Every number below is backed by a verbatim quote from a document that was downloaded and read in full text during this review. Where a document could not be opened it is marked **NOT VERIFIED**. Nothing was filled in from memory.

Method note: documents were downloaded and converted to text locally, so quotes are copied from the source text, not from a summariser. Line breaks inside quotes were joined; nothing else was altered. Superscripts are lost in text extraction, so `1.008(age-12)` in a quote means 1.008^(age−12).

---

## Question 1 — CKiD U25 creatinine equation

### 1.1 Sources opened

| # | Source | URL |
|---|---|---|
| S1 | Pierce CB et al. Kidney Int 2021;99(4):948–956, PubMed Central author manuscript (PMCID PMC9083470, PMID 33301749) | https://pmc.ncbi.nlm.nih.gov/articles/PMC9083470/ |
| S2 | NIDDK, "eGFR Equations for Children, Adolescents, & Young Adults" (page states "Last Reviewed May 2025") | https://www.niddk.nih.gov/research-funding/research-programs/kidney-clinical-research-epidemiology/laboratory/glomerular-filtration-rate-equations/children-adolescents-young-adults |
| S3 | CKiD study's own online calculator (the URL cited inside the paper) | https://ckid-gfrcalculator.shinyapps.io/eGFR/ |
| S4 | Schwartz GJ et al. "New equations to estimate GFR in children with CKD", J Am Soc Nephrol 2009 (PMC2653687) | https://pmc.ncbi.nlm.nih.gov/articles/PMC2653687/ |

The older CKiD site (statepi.jhsph.edu/ckid) only returned a "moved" stub; it was not used.

### 1.2 The table, verbatim

**S1, Table 2** ("Sex-specific values of K in constant and age-dependent models for height/sCr and 1/cysC glomerular filtration rate estimating equations"), height/sCr columns, age-dependent model:

| Age, yr | Male, age dependent | Female, age dependent |
|---|---|---|
| 1–12 | `39.0×1.008(age-12)` | `36.1×1.008(age-12)` |
| 12–15 | `39.0×1.045(age-12)` | `36.1×1.023(age-12)` |
| 15–18 | (same cell as 12–15) | (same cell as 12–15) |
| 18–25 | `50.8` | `41.4` |

Table footnote, verbatim: "Height is measured in meters; sCr, mg/dl; and cysC, mg/L."

Supporting text in S1 (Results): "For males, the value of K ranged from 35.7 (39:0 × 1:008(1–12)) for those aged 1 year to 50.8 for those aged ≥18 years. For boys aged 1 to 12 years, K increased by a factor of 1.008 per year of age; for boys aged 12 to 18 years, K increased more rapidly (1.045 per year of age). For females, the values of K were systematically lower and less variable, ranging from 33.1 (36:1 1:008(1–12)) for those aged 1 year to 41.4 for those aged ≥18 years. For girls aged 1 to 12 years, K increased at the same rate as in boys of the same age. For girls aged 12 to 18 years, K increased by 1.023 for each year increase in age."

**S2, Table 1** ("Sex- and age-dependent values of K for CKiD U25 eGFRcr"), verbatim:

| Age, years | Female | Male |
|---|---|---|
| 1 to <12 | 36.1 × 1.008(Age–12) | 39.0 × 1.008(Age–12) |
| 12 to <18 | 36.1 × 1.023(Age–12) | 39.0 × 1.045(Age–12) |
| 18 to 25 | 41.4 | 50.8 |

S2 equation block, verbatim: "eGFR = κ × (ht/SCr) where: eGFR = estimated GFR in mL/min/1.73 m2; κ = sex- and age-dependent κ values (see Table 1); SCr = enzymatically assayed (recommended) in mg/dL; ht = height in meters".

### 1.3 Constant-by-constant check

| Calculator constant | Original paper (S1) | NIDDK (S2) | Result |
|---|---|---|---|
| Male, age < 12: 39.0 × 1.008^(age−12) | `39.0×1.008(age-12)` | 39.0 × 1.008(Age–12) | MATCH |
| Male, 12 to < 18: 39.0 × 1.045^(age−12) | `39.0×1.045(age-12)` | 39.0 × 1.045(Age–12) | MATCH |
| Male, 18–25: 50.8 | `50.8` | 50.8 | MATCH |
| Female, age < 12: 36.1 × 1.008^(age−12) | `36.1×1.008(age-12)` | 36.1 × 1.008(Age–12) | MATCH |
| Female, 12 to < 18: 36.1 × 1.023^(age−12) | `36.1×1.023(age-12)` | 36.1 × 1.023(Age–12) | MATCH |
| Female, 18–25: 41.4 | `41.4` | 41.4 | MATCH |
| Knots at 12 and 18 years | "3 linear pieces with knots at 12 and 18 years and a constant value for K for subjects aged ≥18 years" | rows "1 to <12", "12 to <18", "18 to 25" | MATCH |
| Height in metres, creatinine in mg/dL | "Height is measured in meters; sCr, mg/dl" | "SCr ... in mg/dL", "ht = height in meters" | MATCH |
| Valid for ages 1–25 years | "children and young adults (aged 1–25 years)" | "for use in individuals ages 1 to 25" | MATCH |

All six constants, both knots, the units and the age range match the original.

Notes for the implementer:
- The paper's Table 2 labels the bands "1–12", "12–15", "15–18", "18–25" without stating which side of the boundary is inclusive. The "<12" / "<18" convention comes from the NIDDK table. It does not matter numerically: the function is continuous at both knots (my arithmetic, not a quote: 39.0 × 1.045^6 = 50.79 and 36.1 × 1.023^6 = 41.38, which round to the published 50.8 and 41.4).
- A secondary catalogue record and one R package surfaced in search with different numbers (41.8 / 37.6). Those are the paper's *constant-K* (age-independent) model, not the age-dependent one: "the sex-specific constants were 41.8 for males and 37.6 for females" (S1). They are not an error in the calculator.
- The CKiD calculator (S3) enforces input limits, verbatim: "Age must be between 1-25", "Use decimals to capture partial years (e.g., 13.75 for a 13 years 9 months old child)", height "Value must be between 60-200 cm or 23.6-78.7 inches", creatinine "Value must be between 0.1-10 mg/dL or 8.8-884.2 μmol/L". The calculator takes decimal age, which agrees with the calculator's use of decimal years.
- Conversion given by NIDDK (S2): "serum creatinine µmol/L to mg/dL, divide by 88.4".

### 1.4 Population, and whether it is recommended outside CKD

- Derivation population (S1, Methods): "a longitudinal cohort study of children and young adults (aged 1–25 years) with mild to moderate CKD recruited from 63 sites across the United States and Canada."
- Size (S1, abstract): "Using data (2655 observations from 928 participants) from the Chronic Kidney Disease in Children Study".
- GFR reference method (S1): "Glomerular filtration rate (GFR) was measured directly by plasma iohexol disappearance."
- **The paper does not recommend it outside CKD.** S1, limitations, verbatim: "Second, our study population did not include children and young adults free of CKD. Therefore, we cannot speak to the validity of our equations for estimating GFR in healthy patients or to their clinical utility for screening otherwise healthy patients for kidney problems."
- Validation was internal only (S1): "we recognize the limitation that no formal external validation was performed and the results should be interpreted accordingly."
- NIDDK (S2) scope statement: "The CKiD U25 equations are for use in individuals ages 1 to 25, with mild-to-moderate chronic kidney disease (CKD)."
- NIDDK (S2) adds later evidence for the creatinine equation only: "the CKiD U25 creatinine equation (U25 eGFRcr) has been shown to provide valid estimates of GFR in pediatric and young adult populations with eGFR ≥ 75 ml/min/1.73m2, as demonstrated in a cohort of healthy European children and young adults." The underlying external study (NIDDK reference 3) was not opened here, so that claim is verified only as a statement by NIDDK.
- Race subgroup (S1): "For the U25 age-dependent sCr equation, the mean arithmetic bias (eGFR – mGFR) for White, Black, and other racial groups was 1.1 (95% CI, 0.2–2.1), −3.2 (95% CI, −5.6 to −0.8), and 0.9 (95% CI, −1.4 to 3.3) ml/min per 1.73 m2, respectively".
- Not addressed by these sources: use below 1 year of age (outside the stated range; NIDDK lists a separate neonatal equation for "term-born infants up to 4 weeks of age").

### 1.5 Reported accuracy (creatinine equation, validation set)

S1, Table 3, row "CKiD U25 age and sex dependent" (plasma/sCr-based), verbatim cell values:

| Metric | Aged <18 yr | Aged ≥18 yr |
|---|---|---|
| Arithmetic bias, eGFR − mGFR (95% CI), mL/min/1.73 m² | 0.0 (−0.9 to 1.0) | 0.7 (−1.6 to 3.0) |
| p10 (% within 10% of mGFR) | 36.4 | 44.9 |
| p30 (% within 30% of mGFR) | 86.2 | 90.7 |
| Test RMSE | 9.9 | 9.6 |

- Validation set size (S1, Table 3 footnote): "children and adolescents aged 1 to 18 years (n = 784 person-visits from 308 participants) and young adults aged 18 to 25 years (n = 107 person-visits from 69 participants)."
- Overall (S1): "For the sCr and the cysC equations, the mean biases were 0.2 (95% CI, −0.8 to 1.15) and −0.8 (95% CI, −1.6 to 0.0) ml/ min per 1.73 m2, respectively." Correlation with mGFR: "0.90 for sCr".
- By age band (S1): "All age-specific mean bias estimates were <5% for males and <7% for females (largest, −6.6% for those aged 10–14 years)."
- NIDDK restates the same P30 (S2): "The U25 eGFRcr equation has a P30 value of 86.2 in children younger than 18 and a P30 value of 90.7 in adolescents and young adults ages 18 to 25."

### 1.6 Original bedside Schwartz (2009)

- Equation, S4 abstract, verbatim: "with height measured in cm, a bedside calculation of 0.413*(height/serum creatinine), provides a good approximation to the estimated GFR formula."
- Derivation age range, S4 abstract: "we developed equations to estimate GFR using data from the baseline visits of 349 children (aged 1 to 16 yr) in the Chronic Kidney Disease in Children (CKiD) cohort. Median iohexol-GFR (iGFR) was 41.3 ml/min per 1.73 m2 (interquartile range 32.0 to 51.7), and median serum creatinine was 1.3 mg/dl."
- S4 Results: "The median age was 10.8 yr." S4 Methods: "Eligible individuals were 1 to 16 yr of age with mild to moderate CKD".
- S4 caveat: "Additional studies of children with higher GFR are needed to validate these formulas for use in screening all children for CKD."
- The same equation with height in metres, as restated by the U25 paper (S1): "GFR = 41:3 × [height (in m)/sCr (in mg/dl)]", and its limitation: "The data used to develop this equation came primarily from children aged 8 to 15 years and lacked an adequate sample of both young children (e.g., aged <5 years) and young adults (e.g., aged ≥18 years)".
- NIDDK (S2): "The CKiD “bedside” equation was developed using data primarily from children ages 8 to 15 with mild-to-moderate CKD and is reserved for quick, urgent estimates during routine clinical care of children." Units given there: "eGFR = 41.3 × (ht/SCr) ... SCr = standardized serum creatinine in mg/dL; ht = height in meters". P30: "The 2009 creatinine-based CKiD “bedside” equation has a P30 value of 77.2 in children younger than 18."

So "0.413 × height (cm) / SCr (mg/dL)" and "41.3 × height (m) / SCr" are the same equation; enrolment range 1–16 years, data concentrated at 8–15 years.

---

## Question 2 — Colombian and Latin American guidance

### 2.1 Documents opened

| # | Document | Year | Host actually fetched | URL |
|---|---|---|---|---|
| C1 | Ministerio de Salud y Protección Social / Universidad de Antioquia. "Guía de práctica clínica para la evaluación del riesgo y manejo inicial de la neumonía en niños y niñas menores de 5 años y bronquiolitis en niños y niñas menores de 2 años". Guía No. 42, "Para uso de profesionales de la salud" (65-page quick-reference version, ISBN 978-958-8838-75-5) | 2014 | **Third-party mirror**, not the Ministry | https://medicosgeneralescolombianos.com/Guias_2014/GPC_42_Neumonia_y_Bronquilitis_compressed.pdf |
| C2 | Ministerio de Salud y Protección Social / Colciencias / Universidad de Antioquia. "Guía de referencia rápida para prevención, diagnóstico y tratamiento de la enfermedad diarreica aguda en niños menores de 5 años". Guía No. 8, "Para profesionales de la salud" | 2013 | Ministry (official) | https://www.minsalud.gov.co/sites/rid/Lists/BibliotecaDigital/RIDE/INEC/IETS/GPC_Prof_Sal_EDA.pdf |
| C3 | Ministerio de Salud y Protección Social / OPS. "Atención Integrada a las Enfermedades Prevalentes de la Infancia — Libro Clínico". "Bogotá D.C., Colombia: OPS, 2012 (Serie 3) 886 páginas. Tercera Edición Corregida y Aumentada. ISBN: 978-958-8472-13-3" | 2012 | Ministry (official) | https://www.minsalud.gov.co/sites/rid/Lists/BibliotecaDigital/RIDE/VS/PP/libro-clinico-atencion-integrada-enfermedades.pdf |
| C4 | ACORL. "Guía para el diagnóstico y tratamiento de faringoamigdalitis aguda bacteriana". Acta de Otorrinolaringología & Cirugía de Cabeza y Cuello 2024;52(4):533–555 ("Guías oficiales ACORL") | 2024 | Journal PDF | https://www.medigraphic.com/pdfs/actaotorrino/aoc-2024/aoc244c.pdf |
| C5 | ACORL. "Guía de práctica clínica para el diagnóstico y el tratamiento de otitis media aguda". Acta otorrinolaringol. cir. cabeza cuello 2024;52(4):574–591, DOI 10.37076/acorl.v52i4.834 | 2024 | Journal PDF | https://www.medigraphic.com/pdfs/actaotorrino/aoc-2024/aoc244e.pdf |
| C6 | ACIN (Asociación Colombiana de Infectología). Gutiérrez-Tobar IF et al. "Guía de práctica clínica colombiana para el tratamiento de infecciones bacterianas de piel y tejidos blandos superficiales en Población pediátrica". Infectio 2023;27(2):114–131 | 2023 | Journal PDF (served from the journal's "prueba." subdomain) | https://prueba.revistainfectio.org/index.php/infectio/article/download/1131/1270/3820 |

Caveats on these sources:
- C1 was read from a mirror. The Ministry's own library only yielded the *caregiver* version of Guía 42 (https://www.minsalud.gov.co/sites/rid/Lists/BibliotecaDigital/RIDE/DE/CA/gpc-neumonia-ninos-menores-2-anos.pdf), which contains no doses. The Ministry's guideline portal (gpc.minsalud.gov.co) did not respond. The mirror's text carries the Ministry's title page, ISBN and citation line, but the official copy of the professional version was **not** opened.
- C3 is the AIEPI *Libro Clínico*, not the separate *Cuadro de Procedimientos* booklet. The booklet itself could not be located as a PDF, so the dose tables quoted below are from the Libro Clínico of the same series and edition. Printed page = PDF page − 2.
- C1, C2 and C3 are 12–14 years old. Whether the Ministry has issued updates was not established.

Opened and excluded because they cover adults only:
- IETS/Ministry "Lineamiento para el desarrollo de una estrategia de uso racional de antibióticos en faringoamigdalitis de origen bacteriano, en adultos inmunocompetentes" (https://acin.revistainfectio.org/images/guias/lineamientos-antibioticos-faringoamigdalitis.pdf).
- ACIN "Guía de práctica clínica para el diagnóstico y tratamiento de la neumonía adquirida en la comunidad" (DOI 10.22354/24223794.1293), whose abstract limits it to "adultos inmunocompetentes".

### 2.2 Recommendations by condition

#### Community-acquired pneumonia

| Source | Drug and dose | Doses/day | Max | Duration | Age scope | Where |
|---|---|---|---|---|---|---|
| C1 | Amoxicillin oral 90 mg/kg/day | not stated | not stated | 5 days | < 5 years, outpatient | PDF p. 46, question 10 |
| C1 | Penicillin G (cristalina) 250 000 U/kg/day | 6 | not stated | 7 days | < 5 years | p. 46 |
| C1 | Ampicillin 200 mg/kg/day (severe pneumonia, < 2 Hib vaccine doses) | 4 | not stated | 7 days | < 5 years | p. 46 (good-practice point) |
| C1 | Amoxicillin-clavulanate "dosis altas (90 mg / kg al día amoxicilina)" after failure at 72 h | not stated | not stated | 5 days | < 5 years | p. 46 (good-practice point) |
| C3 | Amoxicillin 90 mg/kg/day | 2 (every 12 h) | not stated | 5 days | < 5 years (table covers 4–21 kg) | printed pp. 491–492 |
| C3 | Penicillin G 250 000 U/kg/day; ampicillin 200 mg/kg/day | 6; 4 | not stated | not stated here | < 5 years | printed pp. 491–492 |

Quotes:
- C1: "Se recomienda usar la amoxicilina oral en dosis de 90 mg/kg/día durante 5 días en todo menor de 5 años atendido ambulatoriamente con diagnóstico de neumonía y evidencia de consolidación neumónica en la radiografía simple del tórax. Recomendación Fuerte. Calidad de la evidencia Alta"
- C1: "Se recomienda usar la amoxicilina oral a dosis de 90 mg/kg/día durante 5 días, en todo menor con diagnóstico de neumonía y sin acceso a la radiografía simple del tórax."
- C1: "Se recomienda usar en todo menor con diagnóstico de neumonía, la penicilina cristalina, a dosis de 250.000 u/kg/día, repartido en 6 dosis durante 7 días."
- C1: "Punto de buena práctica: Se recomienda usar en todo menor con diagnóstico de neumonía grave y en ausencia de por lo menos dos dosis aplicadas de vacuna contra Haemophilus influenzae tipo b, ampicilina, 200 mg/kg/día, repartido en 4 dosis durante 7 días."
- C1: "Punto de buena práctica: Se recomienda en todo menor con diagnóstico de neumonía tratado con amoxicilina a 90 mg/kg/día y que presente aparición de signos de peligro, tiraje, cianosis o estridor en las siguientes 72 horas de inicio del tratamiento, clasificarlo como neumonía grave, remitirlo a un lugar de mayor complejidad y usar amoxicilina con ácido clavulánico a dosis altas (90 mg / kg al día amoxicilina) por 5 días."
- C1 supporting text (p. 48–49): "El tratamiento empírico que ha demostrado mejores resultados es amoxicilina a dosis de 45 a 90 mg/k/día según la epidemiologia regional de sensibilidad de neumococo, durante 5 días"
- C3: "la mejor opción antibiótica es Amoxicilina, 90 mg/kg/día repartido en 2 dosis diarias. Para los niños con NEUMONÍA, AIEPI recomendaba Amoxicilina oral 90 mg/kg/día en 3 dosis diarias. ... Los estudios han mostrado que la administración de la Amoxicilina en dosis cada 12 horas es una alternativa factible en lugar de cada 8 horas."
- C3: "Ya no se recomiendan dosis bajas de 50 mg/kg/día de Amoxicilina, porque en Colombia tenemos una resistencia a neumococo de baja a media, del 53% y una resistencia real del 32%, lo que obliga a administración siempre a dosis altas."
- C3 duration: "la opinión de un grupo de expertos infectólogos pediatras del país recomienda el tratamiento del niño con neumonía por un tiempo ideal de 5 días."

C3 weight table (printed p. 492), verbatim headings "AMOXICILINA 90 mg/kg/día — Suspensión 250mg/5ml | 500mg/5ml | 700 ó 750mg/5ml — Dosis cada 12 horas — CADA 12 HORAS DURANTE 5 DÍAS":

| Peso | 250 mg/5 ml | 500 mg/5 ml | 700 ó 750 mg/5 ml |
|---|---|---|---|
| 4 a 6 kg | 5 ml | 2,5 ml | 1,5 ml |
| 7 a 9 kg | 7 ml | 3,5 ml | 2,5 ml |
| 10 a 12 kg | 10 ml | 5 ml | 3,5 ml |
| 13 a 15 kg | 12,5 ml | 6,5 ml | 4,5 ml |
| 16 a 18 kg | 14,5 ml | 7,5 ml | 5 ml |
| 19 a 21 kg | 18 ml | 9 ml | 6 ml |

Gap: C1 gives no number of daily doses for oral amoxicillin; the "every 12 h" comes from C3 only. Neither document states a maximum dose. No Colombian paediatric document covering pneumonia in children ≥ 5 years (macrolide dosing) was found.

#### Bronchiolitis

- C1, question 9 (PDF p. 45): "No se recomienda el uso de antibiótico (incluyendo betalactámico o macrólido) para el tratamiento ambulatorio o el tratamiento hospitalario de menores de 2 años de edad con bronquiolitis. Recomendación Fuerte. Calidad de la evidencia Moderada"
- No antibiotic dose applies. Age scope: < 2 years.

#### Acute otitis media

| Source | Drug and dose | Doses/day | Max | Duration | Age scope | Where |
|---|---|---|---|---|---|---|
| C5 | Amoxicillin 90 mg/kg/day | 2 | not stated | ≥ 5 days; 5–7 days mild-moderate; 10 days severe | paediatric | Rec. 14 and 16, journal pp. 585–586 |
| C5 | Amoxicillin-clavulanate 90 mg/kg/day (14:1; "90 mg/kg/6,4 mg/kg día") after failure | 2 | not stated | as above | paediatric | Rec. 17, Table 5 |
| C5 | Cefdinir 14 mg/kg/day (penicillin allergy) | 1–2 | not stated | not stated | paediatric | Table 5 |
| C5 | Cefuroxime 30 mg/kg/day | 2 | not stated | not stated | paediatric | Table 5 |
| C5 | Cefpodoxime 10 mg/kg/day | 2 | not stated | not stated | paediatric | Table 5 |
| C5 | Ceftriaxone 50 mg/kg/day IM or IV | 1 | not stated | 3 days (table: 1–3 days) | paediatric | Rec. 18, Table 5 |
| C5 | Clindamycin 30–40 mg/kg/day | 3 | not stated | not stated | paediatric | Table 5 |
| C5 | Sultamicillin 25–50 mg/kg/day | 2 | not stated | not stated | paediatric | Rec. 17 |
| C3 | Amoxicillin 90 mg/kg/day | 2 | not stated | 10 days | < 5 years (table 4–21 kg) | printed pp. 548–549 |
| C3 | Amoxicillin-clavulanate 90 mg/kg/day, 14:1, or cefuroxime axetil 30 mg/kg/day (recurrence < 6 weeks) | 2 | not stated | 10 days | < 5 years | printed p. 549 |

Quotes:
- C5 Rec. 14: "Si el médico considera el inicio del manejo antibiótico, se recomienda amoxicilina en dosis de 90 mg/kg al día dividida en dos dosis. Esto en ausencia del uso de antibiótico en los últimos 30 días, ausencia de conjuntivitis purulenta y ausencia de alergia a la penicilina." (Grado de recomendación: fuerte a favor. Nivel de evidencia: A.)
- C5 Rec. 16: "Se recomienda en pacientes con otitis media aguda no complicada el uso de antibiótico por al menos cinco días ... se sugiere cinco a siete días de antibiótico para el paciente con otitis media aguda leve a moderada (33). Para el paciente con otitis media aguda grave, se sugiere un tratamiento antibiótico por 10 días."
- C5 Rec. 17: "El antibiótico a elección es amoxicilina/clavulanato de 90 mg/kg/día dividido en dos dosis. Otra opción de manejo incluye sultamicilina de 25-50 mg/kg/día dividido en dos dosis"
- C5 Rec. 18: "se recomienda uso de ceftriaxona de 50 mg/kg/día por tres días por vía intramuscular o intravenosa"
- C5 Table 5 cells: "Amoxicilina de 90 mg/kg por día dividido en dos dosis"; "Cefdinir de 14 mg/kg por día en 1-2 dosis**"; "Amoxicilina/clavulanato de 90 mg/kg/6,4 mg/kg día (14:1), dividido en dos dosis*"; "Cefuroxima de 30 mg/kg por día dividido en dos dosis."; "Cefpodoxima de 10 mg/kg por día dividido en dos dosis**"; "Clindamicina de 30-40 mg/kg por día dividido en tres dosis con o sin cefalosporina de tercera generación"; "Ceftriaxona de 50 mg IM o IV cada día por 1-3 días**". Note that this last cell reads "50 mg", without "/kg"; the recommendation text (Rec. 18) says 50 mg/kg/day. The table cell looks like a typographical omission in the source.
- C5 on when to treat: "En paciente con otitis media aguda grave (dolor intenso, otalgia mayor de 48 horas, fiebre definida como temperatura mayor a 39 °C) o que tenga menos de 24 meses, se indica el inicio del manejo antibiótico en la consulta inicial."
- C3: "Se tratará con antibiótico a todos los niños con clasificación de OTITIS MEDIA AGUDA, el antibiótico de elección es Amoxicilina 90 mg/kg/día, en dos dosis diarias ... durante 10 días."
- C3: "La consulta (2010) con un grupo de infectólogos pediatras del país concluyó que para Colombia el tiempo ideal de manejo del niño con otitis media aguda es de 10 días."
- C3: "trate con un antibiótico de segunda línea, el ideal Amoxicilina–Clavulanato 90 mg/kg/día en dos dosis diarias por 10 días o Cefuroxima axetilo, 30 mg/kg/día en dos dosis diarias por 10 días."

C3 table (printed p. 549), headings "AMOXICILINA 90mg/kg/día Durante 10 días — 250mg/5ml | 500mg/5ml | 700 o 750mg/5ml" and "AMOXICILINA CLAVULANATO 14:1 90 mg/kg/día — 600/42.9 mg en 5 ml", "CADA 12 HORAS DURANTE 10 DÍAS":

| Peso (kg) | Amox 250 mg/5 ml | Amox 500 mg/5 ml | Amox 700 o 750 mg/5 ml | Amox-clav 600/42.9 mg en 5 ml |
|---|---|---|---|---|
| 4–6 | 5 ml | 2,5 ml | 1,5 ml | 2 ml |
| 7–9 | 7 ml | 3,5 ml | 2,5 ml | 3 ml |
| 10–12 | 10 ml | 5 ml | 3,5 ml | 4 ml |
| 13–15 | 12,5 ml | 6,5 ml | 4,5 ml | 5,5 ml |
| 16–18 | 14,5 ml | 7,5 ml | 5 ml | 6,5 ml |
| 19–21 | 18 ml | 9 ml | 6 ml | 7,5 ml |

Discrepancy to note: duration. C3 (2012) says 10 days for all; C5 (2024) says 5–7 days for mild-moderate and 10 days for severe.

#### Streptococcal pharyngotonsillitis

| Source | Drug and dose | Doses/day | Max | Duration | Age scope | Where |
|---|---|---|---|---|---|---|
| C4 | Amoxicillin 50 mg/kg every 24 h, or 25 mg/kg every 12 h | 1 or 2 | 1000 mg | 10 days | children | Table 6, journal p. 547 |
| C4 | Benzathine penicillin < 27 kg: 600 000 IU IM; > 27 kg: 1 200 000 IU IM | single | — | single dose | by weight | Table 6 |
| C4 | Azithromycin 12 mg/kg/day | 1 | 500 mg | 5 days | children | Table 6 (second line) |
| C4 | Clindamycin 10 mg/kg/dose every 6–8 h | 3–4 | 300 mg/dose | 10 days | children | Table 6 (second line) |
| C4 | Clarithromycin 7,5 mg/kg/dose every 12 h | 2 | 250 mg/dose | 10 days | children | Table 6 |
| C4 | Cefuroxime 30 mg/kg/day every 12 h | 2 | not stated | 10 days | children | Table 6 |
| C4 | Amoxicillin-clavulanate 50 mg/kg every 24 h | 1 | 1 g | 5 days (second line) / 10 days (recurrent) | children | Table 6 |
| C3 | Benzathine penicillin 600 000 U IM (25 000–50 000 U/kg) | single | — | single dose | 3–5 years or < 27 kg | printed p. 554 |

Quotes:
- C4 Table 6 ("Recomendaciones de esquema de tratamiento antimicrobiano en FAAB"), first line: "Amoxicilina Adultos: 500 mg VO cada 8 horas o 1000 mg cada 12 horas Niños: 50 mg/ kg VO cada 24 horas o 25 mg/kg cada 12 horas (máximo 1000 mg) 10 días"; "Penicilina benzatínica <27 kg: 600.000 UI vía IM >27 kg: 1.200.000 UI vía IM Dosis única".
- C4 Table 6, second line ("en caso de alergia, intolerancia o no disponibilidad"): "Azitromicina Niños: 12 mg/kg/día VO cada 24 horas (máximo 500 mg) Adultos: 500 mg VO cada 24 horas 5 días"; "Clindamicina Niños: 10 mg/kg/dosis VO cada 6 a 8 horas (máximo 300 mg dosis) + Adultos: 300-600 mg VO cada 8 horas 10 días"; "Amoxicilina con ácido clavulánico Niños: 50 mg/kg VO cada 24 horas (máximo 1 gramo) Adultos: 500/125 mg VO cada 8 horas 5 días"; "Claritromicina Niños: 7,5 mg/kg/dosis VO cada 12 horas (máximo 250 mg dosis) Adultos: 250 mg VO cada 12 horas 10 días"; "Cefuroxime Niños: 30 mg/kg/día VO cada 12 horas Adultos: 500 mg VO cada 12 horas 10 días".
- C4 Table 6, recurrent: "Clindamicina Niños: 7 mg/kg/dosis VO cada 8 horas (máximo 300 mg dosis) + Adultos: 300-600 mg VO cada 8 horas 10 días".
- C4 Table 6 footnote: "+ no disponible en suspensión" (attached to clindamycin).
- C4 on drug choice: "El tratamiento de primera línea para la FAAB es la penicilina V ... sin embargo, la disponibilidad del medicamento en nuestro medio es reducida, razón por la cual, se debe considerar la Amoxicilina oral como primera línea de tratamiento contra FAAB o la penicilina benzatínica intramuscular, como otra opción terapéutica."
- C3: "En los niños de tres a cinco años o menores de 27 kg, se administra una sola dosis de 600.000 unidades (25.000 a 50.000 U/kg)." Table: "PENICILINA BENZATÍNICA — Dosis única intramuscular — Tres a cinco años | < 27 kg | 600.000 U".

Note: in C4 the amoxicillin maximum "1000 mg" is written once after both regimens; the text does not say whether it is per dose or per day.

#### Acute diarrhoea

| Source | Drug and dose | Doses/day | Max | Duration | Age scope | Where |
|---|---|---|---|---|---|---|
| C2 | No routine antimicrobials | — | — | — | < 5 years | PDF p. 31, question 23 |
| C2 | Nalidixic acid 50–60 mg/kg/day (first-line empirical when indicated) | not stated | not stated | 7 days | < 5 years | p. 31 |
| C2 | Cefotaxime, ceftriaxone or ciprofloxacin (second line, IV, in hospital) | no dose given | — | — | < 5 years | p. 31 |
| C2 | Metronidazole first choice for Giardia / E. histolytica; TMP-SMX for cholera | no dose given | — | — | < 5 years | p. 32 |
| C3 | Nalidixic acid 55 mg/kg/day | 4 (every 6 h) | not stated | 5 days | < 5 years (table 5–25 kg) | printed pp. 528–529 |
| C3 | Ceftriaxone 100 mg/kg once daily IV (hospital only) | 1 | not stated | 5 days | children | printed p. 529 |
| C3 | Metronidazole 10 mg/kg three times daily (amoebiasis) | 3 | not stated | 5 days (10 if severe) | children | printed p. 529 |
| C3 | Metronidazole 5 mg/kg three times daily (giardiasis) | 3 | not stated | 5 days | children | printed p. 529 |

Quotes:
- C2: "No se recomienda dar rutinariamente antimicrobianos a los niños con EDA menores de 5 años. Recomendación: Fuerte en contra"
- C2 indications: "Se recomienda dar antimicrobianos a los niños menores 5 años con EDA que presenten las siguientes condiciones clínicas: Sospecha o confirmación de bacteriemia o sepsis. Sospecha de infección por Vibrio cholerae en el contexto de un brote detectado de cólera en la región. Diarrea con sangre, fiebre alta (mayor o igual a 39ºC) y regular estado general, o diarrea con sangre y no tengan opción de revaluación clínica en 48 horas."
- C2: "Se recomienda utilizar ácido nalidíxico (opción terapéutica empírica de primera línea) en dosis de 50 a 60 mg/Kg por día por 7 días, cuando esté indicado dar antimicrobiano a los niños menores de 5 años con EDA. Recomendación: Fuerte a favor"
- C2: "Se recomiendan como alternativas de segunda línea cefotaxime, ceftriaxone o ciprofloxacina cuando esté indicado dar antimicrobiano a los niños menores de 5 años con EDA. Se recomienda la administración intravenosa de estos antibióticos, cuyo uso debe restringirse al manejo intrahospitalario"
- C2: "Se recomienda como primera elección metronidazol y como alternativas tinidazol o secnidazol, en los niños menores 5 años con EDA para los casos de Giardia y Entamoeba histolytica. Para los casos de sospecha o brote de cólera, se recomienda usar trimetoprim/sulfametozazol."
- C3: "El antibiótico de elección para tratar la disentería por Shigella es el ácido nalidíxico, a 55 mg/kg/día repartido en 4 dosis diarias." Table header: "ACIDO NALIDÍXICO — Dosis: 55 mg/kg/día dividido en dosis c/6 horas durante 5 días — Jarabe 250 mg/5 ml"; rows: "5 – 7 kg 1,5 ml; 8 – 10 kg 2,5 ml; 11 – 13 kg 3,5 ml; 14 – 16 kg 4,0 ml; 17 – 19 kg 5,0 ml; 20 – 22 kg 5,5 ml; 23 – 25 kg 6,5 ml".
- C3 table: "DISENTERÍA POR SHIGELLA — ÁCIDO NALIDÍXICO Niños: 55 mg/kg/día en cuatro dosis diarias por cinco días — Ceftriaxona Niños: 100 mg/kg una vez al día IV durante cinco días SÓLO PARA USO HOSPITALARIO"; "AMEBIASIS Metronidazol Niños: 10mg/kg tres veces al día durante cinco días (10 días para la enfermedad grave)"; "GIARDIASIS Metronidazol Niños: 5 mg/kg tres veces al día durante cinco días".
- C3: "NOTA: no debe usarse para el tratamiento de la diarrea bacteriana ni Ampicilina, ni Amoxicilina, ni Trimetoprim sulfametoxazol, ni quinolonas."

Two points that matter for the calculator:
1. C2 and C3 disagree on nalidixic acid duration (7 days vs 5 days) and C2 gives no dosing interval.
2. **Nalidixic acid has no current registration in the INVIMA registry** (see 3.3). The first-line drug of both Colombian documents therefore cannot be mapped to a presentation that is registered today.

#### Skin and soft-tissue infections

Source C6 (ACIN, 2023). Scope, verbatim: "Las recomendaciones sobre tratamiento van dirigidas a población de niños, niñas y adolescentes mayores de 1 mes de edad y menores de 18 años con diagnóstico confirmado de IPTB. No se incluye a la población neonatal".

C6 Table 3 ("Medicamentos antimicrobianos en infecciones de piel y tejidos blandos en pediatría", journal p. 120), verbatim dose cells:

| Entity | Antibiotic | "Dosis pediátrica" (verbatim) |
|---|---|---|
| Impétigo | Cefalexina | "50 mg/kg/día V.O. dividido en 3 o 4 dosis" |
| Impétigo | Clindamicina | "30-40 mg/kg/día V.O. dividido en 3 o 4 dosis" |
| Impétigo | Amoxicilina / clavulánico | "45 mg/kg/día V.O. dividido en 3 dosis" |
| Impétigo | Ungüento de mupirocina, Crema 2% | "Aplicar en las lesiones afectadas de 2 a 3 veces al día" |
| Impétigo | Ungüento de Ácido fusídico, Crema al 2% | "Aplicar en las lesiones afectadas de 2 a 3 veces al día" |
| Impétigo | TMP/SMX | "8 -12 mg/kg/día V.O. dividido en 2 dosis" |
| IPTB por SAMS | Oxacilina | "150-200 mg/kg/día E.V. dividido en 4 a 6 dosis" |
| IPTB por SAMS | Cefazolina | "100 mg/kg/día E.V. dividida en 3 o 4 dosis" |
| IPTB por SAMS | Cefalexina | "50-75 mg/kg/día V.O dividido en 3 o 4 dosis" |
| IPTB por SAMS | Dicloxacilina | "50 mg/kg/día V.O. dividido en 4 dosis" |
| IPTB por SAMS | Doxiciclina | "> 8 años: 4 mg/kg/día V.O. dividido en 2 dosis" |
| IPTB por SAMR | Vancomicina | "40 - 60 mg/kg/día E.V. dividido en 4 dosis, considerar dosis de carga en casos severos" |
| IPTB por SAMR | Linezolide | "30 mg/kg/día E.V. dividido en 3 dosis" |
| IPTB por SAMR | Clindamicina | "30-40 mg/kg/día E.V. dividida en 3 o 4 dosis" |
| IPTB por SAMR | TMP/SMX | "8 mg/kg/día V.O. dividido en 2 dosis" |
| IPTB por Streptococcus | Penicilina G | "60.000-100.000 UI/kg/día V.O dividida en 4 o 6 dosis" |
| IPTB por Streptococcus | Cefalexina | "50 mg/kg/día V.O. dividida en 3 o 4 dosis" |
| IPTB por Streptococcus | Cefazolina | "100 mg/kg/día E.V. dividida en 3 dosis" |
| IPTB por Streptococcus | Penicilina V | "100 mg/kg/día V.O. dividida en 3 dosis" |

Durations in C6:
- Topical: "El tratamiento tópico se sugiere para el impétigo con extensión limitada (es decir menor al 2% del área total de la superficie corporal) durante 5 a 7 días (o hasta la resolución completa)."
- Oral, impetigo/ecthyma: "Se sugiere el tratamiento oral de primera línea con una cefalosporina de primera generación (cefalexina) o como alternativa una penicilina antiestafilocócica (dicloxacilina o flucloxacilina) durante 7 a 10 días"
- Cellulitis: "se prefieren esquemas cortos, por lo que se recomienda en promedio 5 días, lapso que puede ser extendido ante la no mejoría o complicaciones durante este periodo"
- MRSA: "Si se sospecha o confirma SAMR, se recomienda clindamicina o trimetoprim-sulfametoxazol como terapia antibiótica oral."
- Access notes in C6: "La retapamulina no se encuentra disponible en Colombia"; "La flucloxacilina no cuenta con registro INVIMA para ser comercializado en Colombia".

Caveats on C6 Table 3 (as printed, not corrected here):
- No maximum doses are given anywhere in the table.
- The TMP/SMX doses do not say which component the mg/kg refers to.
- "Penicilina G ... V.O" is how the row is printed; penicillin G is a parenteral drug, so the route in that cell looks like an error in the source.

Additional AIEPI (C3) items for skin/soft tissue:
- Infant < 2 months, local skin/umbilical infection (printed p. 480): "El antibiótico de elección es la Cefalexina; la dosis recomendada para el tratamiento de las lesiones superficiales no complicadas de la piel es de 50 mg/kg/día en dos dosis diarias." Table header: "CEFALEXINA 25mg/kg/dosis Suspensión 250mg/5 ml — CADA 12 HORAS DURANTE 7 DÍAS"; rows "2 kg 1 ml; 3 kg 1,5 ml; 4 kg 2 ml; 5 kg 2,5 ml".
- Facial cellulitis of dental origin (printed p. 556): "El tratamiento antibiótico ideal es Clindamicina, 40 mg/kg/día en 4 dosis, más Penicilina Cristalina, 200.000 U/kg/día en seis dosis".
- Oral disease with localised cellulitis (printed p. 557): "el tratamiento de elección es la Amoxicilina/Clavulanato, 90 mg/kg/día en dos dosis diarias por 10 días."

### 2.3 Concentrations and ratios the Colombian documents assume

| Document | Amoxicillin suspension | Amoxicillin-clavulanate |
|---|---|---|
| C3 AIEPI 2012 | "250mg/5ml", "500mg/5ml", "700 ó 750mg/5ml" | "AMOXICILINA CLAVULANATO 14:1" — "600/42.9 mg en 5 ml" |
| C5 ACORL AOM 2024 | none stated | "90 mg/kg/6,4 mg/kg día (14:1)" |
| C1 GPC 42 (2014) | none stated | "dosis altas (90 mg / kg al día amoxicilina)", ratio not stated |
| C4 ACORL pharyngitis 2024 | none stated | adults "500/125 mg"; no paediatric ratio |
| C6 ACIN skin 2023 | none stated | "45 mg/kg/día", ratio not stated |
| C3 AIEPI, other drugs | nalidixic acid "Jarabe 250 mg/5 ml"; cefalexin "Suspensión 250mg/5 ml" | — |

Cross-check against the registry (section 3): amoxicillin 250, 500 and 750 mg/5 mL are registered; **no 700 mg/5 mL product was found**. Amoxicillin-clavulanate 600/42.9 mg per 5 mL (14:1) is registered.

### 2.4 Not found or not verified for Question 2

- **SLIPE consensus on pneumonia / otitis / pharyngitis: NOT VERIFIED.** Search results only cite it bibliographically ("Consenso de la Sociedad Latinoamericana de Infectología Pediátrica (SLIPE) sobre Neumonía Adquirida en la Comunidad", Rev Enf Infecciosas Pediatr 2010;24(94)). No full text could be opened. SLIPE's own publications page (https://slipe.org/web/publicaciones/) was opened and lists no pneumonia, otitis or pharyngitis consensus. No SLIPE dose is reported here.
- **AIEPI "Cuadro de Procedimientos" booklet: not opened.** The Libro Clínico (C3) was used instead.
- **Official Ministry copy of the professional version of Guía 42: not opened** (mirror used; see 2.1).
- **Sociedad Colombiana de Pediatría documents: none found.**
- **Colombian paediatric pneumonia guidance for ages ≥ 5 years: none found.**
- No maximum doses appear in C1, C2, C3, C5 or C6. Only C4 states maxima.

---

## Question 3 — Presentations registered in Colombia

### 3.1 Source and how to read it

The INVIMA web registry lookup is an interactive form and was not queried. The source used is INVIMA's own open-data publication of the same registry:

- "CÓDIGO ÚNICO DE MEDICAMENTOS VIGENTES", attribution "Instituto Nacional de Vigilancia de Medicamentos y Alimentos - INVIMA, Bogotá D.C.", description "INFORMACIÓN DE LOS CÓDIGOS ÚNICOS DE MEDICAMENTOS PARA LOS REGISTROS SANITARIOS VIGENTES." Data last updated 2026-11-16. Dataset: https://www.datos.gov.co/d/i7cb-raxc — query form used: `https://www.datos.gov.co/resource/i7cb-raxc.json?$where=atc='J01CA04'` (one query per ATC code).
- Companion INVIMA datasets, also queried: "EN TRÁMITE DE RENOVACIÓN" (https://www.datos.gov.co/d/vgr4-gemg), "VENCIDOS" (https://www.datos.gov.co/d/vwwf-4ftk), "OTROS ESTADOS" (https://www.datos.gov.co/d/spzp-dfuc).

Status definitions used below:
- **Confirmed** = at least one registration in the VIGENTES dataset with `estadoregistro` "Vigente" and at least one presentation code with `estadocum` "Activo". Counts are numbers of distinct registrations (expedientes).
- **Registered, not marketed** = found only in OTROS ESTADOS with status "Temp. no comerc - Vigente".
- **Not found** = no matching record in VIGENTES.

Limits of this source:
- It shows marketing authorisation, not shelf availability, shortages or price.
- The Plan de Beneficios en Salud list and the national essential medicines list were **not** consulted, so coverage/financing status is not reported.
- Searches were by ATC code. A product filed under a wrong ATC code would be missed.
- Composition fields are free text with data-entry inconsistencies (for example a suspension recorded as "4 mg" where the product name says 200 mg/5 mL). Strengths below were read from the declared composition together with the product name. Per-5-mL figures derived from a "per 100 mL" composition are my arithmetic.

### 3.2 Results

| Drug | Presentation asked | Status | Evidence (registrations; example registro sanitario) |
|---|---|---|---|
| Amoxicillin (J01CA04) | Suspension 125 mg/5 mL | Confirmed | 3 (2 with active code); "AMOXICILINA 125 MG/5 ML POLVO PARA SUSPENSION", INVIMA 2021M-0008761-R1 |
| | Suspension 250 mg/5 mL | Confirmed | 25 (24 with active code); e.g. INVIMA 2020M-010613-R3 |
| | Suspension 500 mg/5 mL | Confirmed | 1; "AMOXIDAL® 500 MG/5ML", INVIMA 2019M-0019164 |
| | Suspension 750 mg/5 mL (not asked) | Confirmed | 2; INVIMA 2024M-0021243, INVIMA 2020M-014711-R3 |
| | Suspension 700 mg/5 mL (named in AIEPI) | Not found | — |
| | Capsule 500 mg | Confirmed | 32; e.g. INVIMA 2021M-007223-R4 |
| | Tablets 500 mg, 875 mg, 1 g (not asked) | Confirmed | 1, 5 and 3 |
| | Capsule 250 mg | Not found | — |
| Amoxicillin-clavulanate (J01CR02) | 250 + 62.5 mg/5 mL (4:1) | Confirmed | 6; "CURAM® 250 MG/5 ML", INVIMA 2020M-010184-R2 |
| | 400 + 57 mg/5 mL (7:1) | Confirmed | 12 (11 with active code); "CLAVULIN® JUNIOR 12 H", INVIMA 2021M-010635-R2 |
| | 600 + 42.9 mg/5 mL (14:1) | Confirmed | 7; "CLAVULIN ES 600", INVIMA 2021M-0002672-R2 |
| | 125 + 31.25 mg/5 mL | Not found | — |
| | 200 + 28.5 mg/5 mL | Not found | — |
| | Tablets 500/125 and 875/125 (not asked) | Confirmed | 11 and 15 |
| Cefuroxime axetil (J01DC02) | Suspension 250 mg/5 mL | Confirmed | 1; "ZINNAT SUSPENSION 250 MG/5 ML.", INVIMA 2023M-010278-R3 |
| | Suspension 125 mg/5 mL | Not found | — |
| | Tablet 250 mg | Confirmed | 1; INVIMA 2016M-0017261 |
| | Tablet 500 mg | Confirmed | 4; e.g. INVIMA 2020M-0010199-R1 |
| Cefalexin (J01DB01) | Suspension 250 mg/5 mL | Confirmed | 15; e.g. INVIMA 2019M-14116-R2 |
| | Suspension 125 mg/5 mL | Not found | — |
| | Capsule 500 mg | Confirmed | 23; e.g. INVIMA 2023M-0020856 |
| | Tablets 500 mg and 1 g (not asked) | Confirmed | 4 and 1 |
| Azithromycin (J01FA10) | Suspension 200 mg/5 mL | Confirmed | 18; "ZITROMAX® POLVO PARA SUSPENSION ORAL.", INVIMA 2023M-014161-R4 |
| | Tablet 500 mg | Confirmed | 43 (+1 capsule 500 mg); e.g. INVIMA 2022M-005294-R3 |
| | Tablet 250 mg | Registered, not marketed | "BACTROSAFE® 250 MG", INVIMA 2019M-0014959-R1 |
| Clarithromycin (J01FA09) | Suspension 125 mg/5 mL | Confirmed | 2; "KLARICID® SUSPENSION 125 MG/ 5 ML", INVIMA 2020M-0019918 |
| | Suspension 250 mg/5 mL | Confirmed | 6; e.g. INVIMA 2023M-004412-R3 |
| | Tablet 500 mg | Confirmed | 19 (18 with active code); e.g. INVIMA 2022M-0017274-R1 |
| | Tablet 250 mg | Not found | — |
| Doxycycline (J01AA02) | 100 mg | Confirmed | capsules 9, tablets 4; e.g. INVIMA 2023M-0006011-R2 |
| Ciprofloxacin (J01MA02) | Tablet 500 mg | Confirmed | 23; e.g. INVIMA 2016M-0011422-R1 |
| | Tablet 250 mg | **Not found** | no oral strength other than 500 mg in VIGENTES |
| | IV 200 mg/100 mL | Confirmed | 5 (plus 1 registered as "2 MG/ML"); "CIPROFLOXACINA 200 MG/100 ML", INVIMA 2023M-0011926-R2 |
| | IV 100 mg/10 mL (not asked) | Confirmed | 3 |
| Metronidazole (P01AB01, J01XD01) | Suspension 250 mg/5 mL | Confirmed | 6; e.g. INVIMA 2020M-0002613-R2 |
| | Suspension 125 mg/5 mL (not asked) | Confirmed | 2; INVIMA 2022M-003677- R5 |
| | Tablet 500 mg | Confirmed | 9 (+1 capsule 500 mg); e.g. INVIMA 2022M-0012280-R2 |
| | Tablet 250 mg | Not found | — |
| | IV 500 mg/100 mL (not asked) | Confirmed | 4 |
| Trimethoprim-sulfamethoxazole (J01EE01) | Suspension 40 + 200 mg/5 mL | Confirmed | 6 (5 with active code); "TRIMETOPRIM SULFA 40MG / 200MG / 5ML SUSPENSIÓN", INVIMA 2022M-006179-R4 |
| | Suspension 80 + 400 mg/5 mL (not asked) | Confirmed | 8 |
| | Tablet 80 + 400 mg | Confirmed | 5; e.g. INVIMA 2021M-006045-R4 |
| | Tablet 160 + 800 mg | Confirmed | 11; e.g. INVIMA 2023M-002312-R5 |
| Clindamycin (J01FF01) | Capsule 300 mg | Confirmed | 10; "DALACIN® C 300 MG CAPSULAS", INVIMA 2023M-14339-R4 |
| | Any oral liquid | **Not found** | none in VIGENTES, OTROS ESTADOS or VENCIDOS. Consistent with ACORL 2024 Table 6 footnote: "+ no disponible en suspensión" |
| | Capsule 150 mg | Not found | 300 mg is the only oral strength |
| Ceftriaxone (J01DD04) | Vial 1 g | Confirmed | 11 (10 with active code); e.g. INVIMA 2023M-0021066 |
| | Vial 500 mg | Registered, not marketed | "DUTABIL® 500 IM", INVIMA 2019M-0019168 |
| Benzathine penicillin (J01CE08) | Vial 1 200 000 IU | Confirmed | 4; e.g. INVIMA 2023M-0014375-R2 |
| | Vial 2 400 000 IU | Confirmed | 4; e.g. INVIMA 2020M-0014285-R1 |
| Penicillin G sodium (J01CE01) | Vial 1 000 000 IU | Confirmed | 2; INVIMA 2023M-0014213-R2 |
| | Vial 5 000 000 IU | Confirmed | 2; INVIMA 2023M-0014212-R2 |
| Cefpodoxime (J01DD13) | Marketed at all? | Confirmed, very limited | 2: "DOXCEF® SUSPENSION" (800 mg per 100 mL, i.e. 40 mg/5 mL), INVIMA 2023M-0008747-R2; "DOXCEF ® 100" tablet 100 mg, INVIMA 2023M-0008377-R2. Generic tablets 100 and 200 mg are registered but not marketed |
| Cefdinir (J01DD15) | Marketed at all? | Confirmed, very limited | 2: "TERCEF® 125 MG / 5 ML", INVIMA 2026M-0022884; "TERCEF® 250 MG/5 ML", INVIMA 2026M-0022880. Both registrations issued in 2026. No capsules found |
| Vancomycin, oral (J01XA01; nothing under A07AA09) | Oral form | Confirmed (capsules only) | capsule 125 mg: "VANEOTIK® 125 MG", INVIMA 2019M-0019400; capsule 250 mg: 2 (INVIMA 2019M-0019371, INVIMA 2025M-0022412). No oral solution. IV vials 500 mg and 1 g also registered |
| Mupirocin (D06AX09) | 2% ointment | Confirmed | 4; "MUPICARE®UNGUENTO 2 %", INVIMA 2022M-0006299-R2 |

### 3.3 Extra checks prompted by the guidelines

| Drug | Why checked | Finding |
|---|---|---|
| Nalidixic acid (J01MB02) | First-line for dysentery in C2 and C3 | **No current registration.** VIGENTES and EN TRÁMITE DE RENOVACIÓN: 0 records. Former products are in VENCIDOS (e.g. INVIMA M-011385, expired 2011) and OTROS ESTADOS ("ACIDO NALIDIXICO SUSPENSION", INVIMA 2017M-0005731-R1, "Perdida Fuerza Ejec"; "WINTOMYLON TABLETAS", INVIMA 2015M-007491-R3, "Perdida Fuerza Ejec") |
| Penicillin V (J01CE02) | ACORL says availability is "reducida" | 1 registration: "PENVIOTINA® POLVO", powder for oral solution, INVIMA 2021M-0010931-R1 |
| Dicloxacillin (J01CF01) | Alternative in C6 | Capsule 500 mg (15) and suspension 250 mg/5 mL (10) |
| Fusidic acid (D06AX01) | Topical option in C6 | 2% cream (12 registrations, 11 active) and one ointment |

### 3.4 What this means for the presentation list in the calculator

- Presentations in the request that are **not** currently registered and marketed: ciprofloxacin 250 mg tablets, any clindamycin oral liquid, amoxicillin-clavulanate below 250/62.5 per 5 mL.
- Presentations the request did not list but that exist and that Colombian guidance refers to: amoxicillin 750 mg/5 mL; amoxicillin-clavulanate 400/57 and 600/42.9 per 5 mL.
- The AIEPI "700 mg/5 ml" amoxicillin column has no registered product behind it; only the "750" half of "700 ó 750" does.
- Cefuroxime and cefalexin suspensions exist only at 250 mg/5 mL.

---

## Summary of what could not be verified

1. SLIPE consensus documents (pneumonia, otitis, pharyngitis): full text not opened; no doses reported.
2. Official Ministry-hosted copy of the professional version of Guía No. 42: not opened; a mirror was read.
3. AIEPI "Cuadro de Procedimientos" booklet: not opened; the AIEPI Libro Clínico (2012) was read instead.
4. Whether the 2012–2014 Ministry documents have been superseded: not established.
5. Sociedad Colombiana de Pediatría guidance, and Colombian paediatric pneumonia guidance for children ≥ 5 years: none found.
6. The external study behind NIDDK's statement that U25 creatinine is valid at eGFR ≥ 75 in healthy children: not opened.
7. Plan de Beneficios en Salud and essential medicines list: not consulted; the INVIMA interactive registry form was not queried (INVIMA's open-data copy of the registry was used).
8. Actual market availability (stock, shortages) of any registered presentation.
