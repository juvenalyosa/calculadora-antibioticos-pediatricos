# Review A — Verification of `esquemas.csv` against the source articles

Read-only review. No repository file was modified.

- Claims reviewed: `data/fuente/esquemas.csv` (38 rows), `data/fuente/enfermedades.csv` (7 "no antibiotic" messages), `data/fuente/fuentes.csv` (10 sources).
- Method: every row was compared with the text extract of its source. All table/figure-derived rows were additionally read from a rendered image of the PDF page: Finnish Table 3 (PDF p. 6 = journal p. 253), AAFP Figure 2 (PDF p. 4 = p. 353), French Tables 1 and 2 (PDF p. 3–4 = p. S28–S29), JFP Table 1 (PDF p. 3 = p. 878). Rendered pages are in `scratchpad/revisiones/img/`.
- Quotes are verbatim; the only edits are that line-break hyphens of the PDF extraction were joined and, in paper 20, the extraction artefact "e" between numbers was restored to an en dash ("100e150" → "100–150").
- Not verifiable: the four rows that cite `idsa_faringitis_2012` (no PDF among the 31 papers).

## Verdict counts

| Verdict | Rows |
|---|---|
| OK | 26 |
| PARTIAL | 7 |
| DISCREPANCY | 1 |
| UNSUPPORTED (could not verify, source not among the PDFs) | 4 |
| **Total** | **38** |

Verdict meaning used here: **OK** = every number, unit basis, interval, maximum, duration, age limit and route matches the source literally (minor remarks may still be listed); **PARTIAL** = numbers match, but one field or one statement of the row is not supported by the cited source; **DISCREPANCY** = the row states something the source contradicts; **UNSUPPORTED** = could not be verified.

---

## 1. Row-by-row table (38 rows)

| # | Row (enfermedad / linea / opcion / medicamento) | Source | Verdict | Remarks |
|---|---|---|---|---|
| 1 | oma / primera / 1 / amoxicilina | consenso_italiano_oma_2025 | OK | See note 1 |
| 2 | oma / primera / 2 / amoxicilina | aafp_oma_2019 | OK | See note 2 (`pagina` incomplete; conflict with row 1's source) |
| 3 | oma / segunda / 1 / amoxicilina_clavulanato | aafp_oma_2019 | **PARTIAL** | Last sentence of `nota` is not in AAFP. See 1.1 |
| 4 | oma / segunda / 2 / ceftriaxona (im) | aafp_oma_2019 | OK | See note 4 |
| 5 | oma / alergia / 1 / cefuroxima | aafp_oma_2019 | **PARTIAL** | "(alergia no grave)" is not in AAFP; one of four options. See 1.2 |
| 6 | oma / alergia / 2 / claritromicina | sip_oma_2019 | **DISCREPANCY** | `nota` says the guide does not give the number of daily doses; it does (2). See 1.3 |
| 7 | faringoamigdalitis / primera / 1 / amoxicilina | consenso_italiano_faringitis_2024 | **PARTIAL** | The 3 g/day cap is not part of the recommendation. See 1.4 |
| 8 | faringoamigdalitis / primera / 2 / penicilina_benzatinica (<27 kg) | idsa_faringitis_2012 | **UNSUPPORTED** | Could not verify. See 1.5 |
| 9 | faringoamigdalitis / primera / 3 / penicilina_benzatinica (≥27 kg) | idsa_faringitis_2012 | **UNSUPPORTED** | Could not verify. See 1.5 |
| 10 | faringoamigdalitis / alergia / 1 / cefalexina | idsa_faringitis_2012 | **UNSUPPORTED** | Could not verify. See 1.5 |
| 11 | faringoamigdalitis / alergia / 2 / azitromicina | idsa_faringitis_2012 | **UNSUPPORTED** | Could not verify. See 1.5 |
| 12 | neumonia / primera / 1 / amoxicilina (40–45) | finlandia_vri_2023 | OK | — |
| 13 | neumonia / primera / 2 / amoxicilina (80–90) | finlandia_vri_2023 | OK | Source says "usually for 5 days" |
| 14 | neumonia / alergia / 1 / azitromicina | finlandia_vri_2023 | OK | Route not stated for macrolides in the table (note 14) |
| 15 | neumonia / alergia / 2 / claritromicina | finlandia_vri_2023 | **PARTIAL** | `intervalo_horas`=12 is not in the source (disclosed in `nota`). See 1.6 |
| 16 | neumonia / alergia / 3 / doxiciclina | finlandia_vri_2023 | OK | Age boundary remark (note 16) |
| 17 | neumonia_hospitalizada / primera / 1 / penicilina_g | finlandia_vri_2023 | OK | Row choices: 4 doses of "4–6"; usual = upper end (note 17) |
| 18 | neumonia_hospitalizada / alergia / 1 / cefuroxima (iv) | finlandia_vri_2023 | OK | — |
| 19 | neumonia_hospitalizada / alergia / 2 / ceftriaxona (iv) | finlandia_vri_2023 | OK | Row choice: 1 dose of "1–2 doses" (disclosed) |
| 20 | neumonia_micoplasma / primera / 1 / azitromicina | china_micoplasma_2023 | OK | Dose is in the "Justification", not in the recommendation itself (note 20) |
| 21 | neumonia_micoplasma / segunda / 1 / doxiciclina | china_micoplasma_2023 | **PARTIAL** | "segunda línea" is not how the source positions it. See 1.7 |
| 22 | tosferina / primera / 1 / azitromicina | finlandia_vri_2023 | OK | Route not stated in the table |
| 23 | tosferina / primera / 2 / claritromicina | finlandia_vri_2023 | **PARTIAL** | `intervalo_horas`=12 is not in the source (disclosed in `nota`). See 1.6 |
| 24 | shigelosis / primera / 1 / azitromicina | francia_gastroenteritis_2017 | OK | — |
| 25 | shigelosis / segunda / 1 / ciprofloxacina (oral) | francia_gastroenteritis_2017 | OK | usual = upper end (30) |
| 26 | shigelosis / segunda / 2 / ceftriaxona | francia_gastroenteritis_2017 | OK | — |
| 27 | campylobacter / primera / 1 / azitromicina | francia_gastroenteritis_2017 | OK | "severe forms" statement is on p. S27, not in Table 1 |
| 28 | campylobacter / segunda / 1 / ciprofloxacina | francia_gastroenteritis_2017 | OK | — |
| 29 | salmonelosis / primera / 1 / ceftriaxona | francia_gastroenteritis_2017 | OK | Wording remark (note 29) |
| 30 | salmonelosis / segunda / 1 / ciprofloxacina (oral) | francia_gastroenteritis_2017 | OK | — |
| 31 | salmonelosis / segunda / 2 / ciprofloxacina (iv) | francia_gastroenteritis_2017 | OK | Duration is printed once, after the oral alternative (note 31) |
| 32 | clostridioides_difficile / primera / 1 / metronidazol | francia_gastroenteritis_2017 | OK | `nota` drops an exception (note 32) |
| 33 | clostridioides_difficile / segunda / 1 / vancomicina (oral) | francia_gastroenteritis_2017 | OK | — |
| 34 | giardiasis / primera / 1 / metronidazol | francia_gastroenteritis_2017 | OK | Row choice: 3 doses of "2 or 3" (disclosed) |
| 35 | amebiasis / primera / 1 / metronidazol | francia_gastroenteritis_2017 | OK | "coprocultivo" vs "stool test" (note 35) |
| 36 | piel_sarm / primera / 1 / trimetoprim_sulfametoxazol | jfp_uso_racional_2016 | OK | `nota` omits two indications (note 36) |
| 37 | piel_sarm / primera / 2 / clindamicina | jfp_uso_racional_2016 | **PARTIAL** | Source does not call clindamycin first-line. See 1.8 |
| 38 | impetigo / primera / 1 / mupirocina | jfp_uso_racional_2016 | OK | Source table is specific to CA-MRSA (note 38) |

### Non-OK rows: quotes and what should change

#### 1.1 Row 3 — oma / segunda / 1 / amoxicilina_clavulanato — PARTIAL

Dose, ratio, interval and indication match AAFP Figure 2 (p. 353):

> "Amoxicillin/clavulanate (Augmentin), 90 mg per kg/6.4 mg per kg per day (amoxicillin to clavulanate ratio of 14:1) in two divided doses"

> "Patient has taken amoxicillin for AOM in the past 30 days or has purulent conjunctivitis?" … "Treatment failure at 48 to 72 hours†"

Problem: the last sentence of `nota`, «Con presentaciones 7:1 el clavulanato no debe superar 10 mg/kg/día», does **not** appear anywhere in AAFP. It comes from the two Italian documents:

- consenso_italiano_oma_2025, p. 8: "the standard ratio of amoxicillin to clavulanic acid in Italian formulations is 7:1" and "at the recommended amoxicillin dose (80-90 mg/kg/day), the clavulanic acid dose is slightly higher than the maximum recommended (11.7 mg/kg/day vs. 10 mg/kg/day)".
- sip_oma_2019, p. S15: "To avoid the side effects of clavulanic acid, which are much more common on exceeding the recommended dose of 10mg/kg/day, it is more reasonable to administer the standard dose of the 7:1 amoxicillin-clavulanic acid combination alongside an adequate additional dose of amoxicillin alone".

What should change: either remove that sentence from the row, or attribute it explicitly to the Italian consensus (it cannot stand under `fuente_id` = aafp_oma_2019). Also: the duration 5–10 days comes from the text on p. 352, not from Figure 2 — `pagina` should read "Figura 2 (p. 353) y texto p. 352". AAFP states no maximum dose (none omitted).

#### 1.2 Row 5 — oma / alergia / 1 / cefuroxima — PARTIAL

Dose and interval match AAFP Figure 2 (p. 353), penicillin-allergy branch:

> "Cefuroxime, 30 mg per kg per day in two divided doses"

`nota` matches the footnote: "*—Cross-reactivity with second- and third-generation cephalosporins is low and considered safe."

Problems:
1. `descripcion_opcion` says «(alergia no grave a la penicilina)». AAFP Figure 2 only asks "Penicillin allergy?* … Yes" and does not grade the allergy. The only AAFP sentence on this is (p. 352): "Cefdinir or azithromycin (Zithromax) should be the first-line antibiotic in those with penicillin allergy based on risk of cephalosporin allergy." The "not severe allergy" qualifier is from sip_oma_2019, Recommendation 9 (p. S16): "Class II or III cephalosporins are recommended in children with not severe allergy to penicillin, since cross reaction between these molecules is rare".
2. The row picks one of four options the figure lists at the same level: "Cefdinir, 14 mg per kg per day in one dose or two divided doses"; "Cefuroxime, 30 mg per kg per day in two divided doses"; "Cefpodoxime, 10 mg per kg per day in two divided doses"; "Ceftriaxone IM or IV, 50 mg per kg per day for one or three days (maximum dosage: 1 g per day)".
3. Duration 5–10 days is from the generic statement on p. 352, not from Figure 2; `nota` does not say when 5 and when 10.

What should change: cite SIP Rec. 9 for the "alergia no grave" qualifier (or drop it), say in `nota` that AAFP lists cefdinir/cefpodoxime/ceftriaxone as equivalent alternatives, and add p. 352 to `pagina`.

#### 1.3 Row 6 — oma / alergia / 2 / claritromicina — DISCREPANCY

Dose and indication match SIP Recommendation 9 (p. S16):

> "Macrolides (clarithromycin 15mg/kg/day) should only be used in children with a documented history of recent and/or severe allergy to penicillin."

Discrepancy: `nota` says «La guía no indica en cuántas tomas se divide la dosis diaria; se calcula cada 12 horas: confirmar.» The guide does state it, on the same page S16:

> "These favorable pharmacokinetics mean that only 2 daily administrations of clarithromycin, and just 1 of azithromycin, are needed."

What should change: replace that sentence with a reference to p. S16 (2 daily administrations); `intervalo_horas` = 12 is then source-supported and the "confirmar" can go. «La guía no indica la duración» is correct for clarithromycin (Recommendations 11–12 give durations only for "amoxicillin or amoxicillin-clavulanic acid"). No maximum dose is stated by the source.

Additional context the row does not carry (same page S16): "resistance to glucoside macrolides is now high … meaning these antibiotics cannot be proposed for the treatment of AOM"; and consenso_italiano_oma_2025, p. 10, on macrolides: "only very high doses are effective [53]".

#### 1.4 Row 7 — faringoamigdalitis / primera / 1 / amoxicilina — PARTIAL

Dose, interval and duration match:

- Recommendation 6 (p. 7): "In children with GABHS pharyngotonsillitis, a daily dose of amoxicillin (50 mg/kg/day) divided into 2 administrations may be recommended to improve treatment adherence."
- Recommendation 3 (p. 5): "amoxicillin for 10 days is recommended in children with GABHS pharyngotonsillitis."

Problem: `max_por_dia` = 3000. Neither recommendation states a maximum. The 3 g figure comes from a background sentence about the general dose range, limited to children under 40 kg (Question 6, p. 6):

> "amoxicillin dosage for children under 40 kg varies from 40 to 90 mg/kg/day, not exceeding 3 g/day, and is typically divided into two or three doses."

At 50 mg/kg/day a child under 40 kg receives less than 2000 mg/day, so in the population the sentence refers to the cap never applies; the row therefore effectively applies it only to children ≥40 kg, for whom the source gives no dosing at all. The `nota` discloses the origin («lo menciona la guía para menores de 40 kg»), but the field is an extrapolation.

What should change: leave `max_por_dia` empty (or mark it as not source-derived for ≥40 kg and decide the cap from another reference); keep the sentence only as context. `pagina` (Rec. 3 and 6, p. 5–7) is correct.

#### 1.5 Rows 8–11 — faringoamigdalitis (IDSA 2012, estado = muestra) — UNSUPPORTED / could not verify

The IDSA 2012 guideline is not among the 31 PDFs and its dosing table is not reproduced in any of them (searched all extracts for the benzathine doses; no hit as a recommendation). Doses, weight cut-off (27 kg), per-dose maxima (500 mg) and durations of these four rows could not be verified.

What could be verified are the statements in `nota` that refer to the Italian pharyngitis consensus — all three are faithful:

- Rows 8–9 («la reserva para quien no puede tomar o absorber el tratamiento oral, o tiene mala adherencia»): Rec. 7a (p. 7) "benzathine-penicillin therapy should only be recommended for subjects with reduced gastrointestinal absorption or inability to take oral therapies"; Rec. 7b "may be recommended in case of poor adherence to oral amoxicillin, especially in patients at high risk of suppurative complications".
- Row 10: Rec. 4b (p. 5) "a 3rd generation cephalosporin for 5 days should be recommended as an alternative therapy, restricting the use of macrolides to patients at high risk of severe allergic reactions". No drug or dose is named — correct.
- Row 11: same Rec. 4b — correct.

Remarks for whoever replaces these sample rows:
- Row 10 loads cefalexin (a first-generation cephalosporin) for 10 days, while the disease's main source recommends a **3rd-generation** cephalosporin for **5 days**. The row and the consensus are not the same regimen.
- Row 11 (azithromycin 12 mg/kg/day): the Italian consensus makes no dosing recommendation, but its evidence summary (p. 4) says: "AZT at a dosage of 10 mg/kg/day was associated with an increased risk of recurrence and failure to eradicate GABHS. In contrast, AZT administration at 20 mg/kg/day was associated with increased clinical and microbiological success rate compared to a 10-day course of penicillin V. However, this higher dosage AZT regimen was associated with an increased incidence of adverse events".

#### 1.6 Rows 15 and 23 — claritromicina (neumonia / alergia / 2 and tosferina / primera / 2) — PARTIAL

Dose, maximum and duration match Finnish Table 3 (p. 253):

> "Clarithromycin 15 mg/kg/day (up to 1000 mg/day), usually for 7 days." (outpatient CAP, under "Allergy for penicillin: Macrolides")

> "Clarithromycin 15 mg/kg/day (up to 1000 mg/day), usually for 7 days" (pertussis)

Problem: the table gives no number of daily doses for clarithromycin, and no route for the macrolides ("p.o." is printed for amoxicillin and doxycycline only). `intervalo_horas` = 12 is an assumption; the `nota` says so («se calcula cada 12 horas: confirmar»). The field cannot be verified against this source.

What should change: nothing in the numbers; the interval needs a second source before `verificado` (the SIP guideline, p. S16, says of clarithromycin in general: "only 2 daily administrations of clarithromycin … are needed", but that is an AOM document). Row 15's footnote sentence is faithful: "Severe pneumonia should not be treated with macrolides as a monotherapy."

#### 1.7 Row 21 — neumonia_micoplasma / segunda / 1 / doxiciclina — PARTIAL

Dose, interval, per-dose maximum, duration, age and routes match (Justification of Recommendation 6, p. 7):

> "Dosage and administration of tetracyclines (for ages 8 years and above): The recommended dosage for doxycycline is 2.2 mg/(kg⋅dose), administered every 12 h, with a maximum dose of 100 mg per dose. It can be administered orally or intravenously, for a course of 10 days."

Problems:
1. `linea` = segunda. The source does not present tetracyclines as second-line; it positions them by severity and age. Recommendation 6 (p. 6, question "What are the antibiotic treatments for SMPP in children?"): "Tetracyclines are recommended for children aged 8 years and above, while macrolides are recommended for children under 8 years old (GPS)." The same text is repeated for refractory disease in Recommendation 9 (p. 8). For a child ≥8 years with severe MPP it is the recommended treatment, not a second choice; for mild MPP at any age it is not recommended at all. The description «neumonía grave» mitigates this, but the line label is not source-supported.
2. The row picks doxycycline among three tetracyclines the source doses (doxycycline, minocycline, tetracycline); in the expert survey minocycline was the more frequent choice: "Regarding the use of tetracyclines, 81% (17/21) chose minocycline and 48% (10/21) selected doxycycline."

What should change: consider a separate disease entry or option for "neumonía grave/refractaria por micoplasma, ≥8 años" instead of "segunda línea"; mention Recommendation 9 (RMPP) in `pagina`/`nota`. Numbers need no change.

#### 1.8 Row 37 — piel_sarm / primera / 2 / clindamicina — PARTIAL

Dose, interval and duration match JFP Table 1 (p. 878):

> "Clindamycin … Children: 10-20 mg/kg/d divided q8h x 5-10 d" — comment: "D-zone test informs utility; preferred for pregnant patients"

Problem: `linea` = primera. The text on the same page says:

> "First-line empiric therapy in the outpatient setting should be either trimethoprim/sulfamethoxazole (TMP/SMX) or a long-acting tetracycline (TABLE 1)."

> "Clindamycin is an option in settings in which isolates can be tested to ensure a lack of inducible resistance; it is also the preferred agent for pregnant women."

The source names TMP/SMX or a long-acting tetracycline as first-line and clindamycin as "an option". What should change: move clindamycin to an alternative line (or state in `nota` that the source does not rank it first-line). No maximum dose is stated by the source.

### Notes on OK rows (minor remarks, row choices, omissions)

- **Note 1 (row 1).** Rec. 3.2 (p. 7): "amoxicillin should be prescribed, when necessary, at 80–90 mg/kg/day"; Rec. 3.3 (p. 7): "it is recommended to divide the daily dose of amoxicillin (or amoxicillin/clavulanate) into three administrations"; Rec. 4.1 (p. 8): "for 7 or 10 days in high-risk or severe conditions"; Rec. 4.2 (p. 8): "For children without risk factors for unfavourable outcomes (e.g. older than 2 years, without otorrhea, without recurrence, without bilaterality, without severe symptoms, after a period of watchful waiting), a shorter course of 5 days may be recommended"; abstract (p. 1): "The recommended amoxicillin dosage is 90 mg/kg/day, divided into three doses, though two doses may be considered to improve compliance." `pagina` is correct (p. 7, 8, 11). Remarks: (a) `nota` says «7 a 10 días en menores de 2 años o en cuadros graves»; the recommendation says "high-risk or severe conditions" — "under 2 years" appears only as an example quoted from the SIP guideline (p. 7: "children at risk of unfavourable outcomes (e.g., under 2 years of age or with spontaneous otorrhea)"). (b) «La guía admite 2 tomas al día» is softer than the body text (p. 7): "in children with AOM, a twice daily schedule for amoxicillin or amoxicillin-clavulanic acid is not recommended … However, in particular conditions where administration in three doses creates great difficulties for the family, administration in two doses may be considered". No maximum daily dose is stated anywhere in this source.
- **Note 2 (row 2).** Figure 2 (p. 353): "Amoxicillin, 80 to 90 mg per kg per day in two divided doses". Duration text is on p. 352, not in the figure: "A 10-day course of antibiotics is recommended if the child is younger than two years or has severe symptoms. A five- to seven-day course is effective if the child is two years or older and does not have severe symptoms." → `pagina` should add p. 352. Cross-source conflict to be aware of: this row offers twice-daily dosing as a first-line option while row 1's source states a twice-daily schedule "is not recommended" (above).
- **Note 4 (row 4).** Figure 2: "Ceftriaxone IM or IV, 50 mg per kg per day for one or three days (maximum dosage: 1 g per day), or clindamycin, 30 to 40 mg per kg per day in three divided doses for three days, with or without a third-generation cephalosporin". Row matches (50 mg/kg/day, 1000 mg/day cap, IM, IV mentioned). Remarks: `duracion_dias_min/max` = 1–3 also admits 2 days, the source says "one or three"; the clindamycin alternative in the same box is not loaded; ceftriaxone also appears as an initial option in the allergy branch. Other sources restrict parenteral therapy: consenso_italiano_oma_2025 Rec. 9 (p. 10) "parenteral antibiotics are not recommended"; sip_oma_2019 (p. S15) "should be restricted to children with persistent vomiting or who are unable to tolerate oral administration".
- **Rows 12–13.** Table 3: "Normal dose: 40–45 mg/kg/day p.o. in three doses for 5 days." / "High dose (if penicillin resistant pneumococcus suspected): 80–90 mg/kg/day p.o. (up to 4000 mg/day) in three doses usually for 5 days." No maximum is given for the normal dose (none omitted).
- **Note 14 (rows 14, 22).** "Azithromycin 10 mg/kg/day (up to 500 mg/day) once a day for 3 days." (CAP) / "… once a day for 5 days." (pertussis). The table does not print a route for macrolides; `via` = oral is an inference.
- **Note 16 (row 16).** "Doxycycline (children >8 years old) p.o. 2.2–4.4 mg/kg/day (up to 200 mg/day) divided in two doses usually for 7 days". Matches. `edad_min_meses` = 96 includes a child of exactly 8 years, the source says ">8 years old". `dosis_usual` = 4.4 is the upper end of the range (row choice).
- **Note 17 (row 17).** "Penicillin 100 000–300 000 U/kg/day i.v. divided in 4–6 doses (up to 24 000 000 U/day)". Matches; unit UI in `medicamentos.csv`. Row choices: 4 doses (q6h) and `dosis_usual` = 300 000 (upper end). Footnote b also says, and the row does not: "Penicillin combined with clindamycin is recommended in Streptococcus pyogenes pneumonia."
- **Rows 18–19.** "Cefuroxime 100–150 mg/kg/day i.v. divided in three doses (up to 6000 mg/day)." / "Ceftriaxone 100 mg/kg/day i.v. divided in 1–2 doses (up to 4000 mg/day)". Footnote c quoted faithfully in row 18.
- **Note 20 (row 20).** Rec. 5 (p. 6) itself says "the drug dosage is referred to the medication instructions"; the numbers are in the Justification (p. 6): "The recommended dosage is 10 mg/(kg⋅dose), administered once daily, with a maximum dose of 500 mg per administration. Oral administration is preferred, and a sequential treatment course should not exceed 10 days." Row matches.
- **Rows 24–28, 30, 33.** Match Table 1 (p. S28) literally: azithromycin "20 mg/kg/day in once-daily dose (maximum 500 mg/day) Treatment duration: 3 days"; ciprofloxacin oral "20 to 30 mg/kg/ day in 2 divided doses (maximum 1500 mg/day)" with 3 days (Shigella), 5 days (Campylobacter), 3–5 days (Salmonella); ceftriaxone "50 mg/kg/day IV or IM in once daily dose (maximum 2 g/day)", 3 days (Shigella); vancomycin oral "40 mg/kg/day in 4 divided doses. Treatment duration: 10 days" (no maximum stated). Row 27's «formas graves, sobre todo al inicio» is from the text on p. S27 ("severe Campylobacter infections, particularly in the initial phase"), so `pagina` should add p. S27.
- **Note 29 (row 29).** "Antibiotics most often not necessary" / "Ceftriaxone (IV or IM) 50 mg/kg/day IV or IM in once daily dose (maximum 2 g/day) Treatment duration: 3–5 days". Indication list matches. «casi nunca es necesario» is slightly stronger than "most often not necessary".
- **Note 31 (row 31).** "Ciprofloxacin (IV) 20 mg/kg/ day in 2 divided doses or Ciprofloxacin (oral) … Treatment duration: 3–5 days". The duration is printed once, after the oral alternative; applying it to the IV form is the natural reading. No maximum is given for IV (none omitted).
- **Note 32 (row 32).** "Metronidazole (oral) 30 mg/kg/day in 2 divided doses (maximum 1.5 g/day) Treatment duration:10 days". `nota` drops the exception: "The presence of toxins has a poor diagnosis value before 3 years of age **except in cases of intestinal obstruction**", and omits "Infectious disease specialist advice is required in immunocompromized children or in case of recurrence."
- **Row 34.** "Metronidazole (oral) 30–40 mg/kg/day in 2 or 3 doses Treatment duration: 5 days" — no maximum for Giardia in the source (none omitted). "Possibly repeat treatment after 10–15 days."
- **Note 35 (row 35).** "Metronidazole (oral) 30–40 mg/kg/day in 2–3 divided doses (maximum dose: 1.5 g/day) Treatment duration: 7-10 jours". Comment: "Always do a stool test to rule out a bacterial cause" — the row renders "stool test" as «coprocultivo».
- **Note 36 (row 36).** "TMP/SMX … Children: 8-12 mg/kg/d (TMP) divided q12h x 5-10 d". `nota` is faithful but shortens the indication list; source: "Antimicrobial agents are generally reserved for patients who have extensive disease involving multiple infection sites and/or systemic symptoms, purulent cellulitis without drainable foci, or septic phlebitis, or failed to respond to incision and drainage alone. Extreme age is a reasonable indication for antibiotics as well." "Extreme age" is relevant to a paediatric tool and is not in the `nota`.
- **Note 38 (row 38).** "Mupirocin 2% ointment — Adults and children: Apply topically q8h x 5 d — For minor skin infections (impetigo) or secondarily infected skin lesions". Matches. The table is titled "Treating CA-MRSA: Which options, and when" — the source gives this regimen in the context of CA-MRSA, the disease entry is generic impetigo.

---

## 2. "No antibiotic" messages (7)

| Disease | Source | Verdict | Evidence / remark |
|---|---|---|---|
| otitis_media_efusion | codepeh_ome_2019 | PARTIAL | Abstract (p. 36): "medication, in particular antibiotics and corticoids, should not be prescribed and 3 months of watchful waiting should be the first adopted measure. If OME persists, an ENT doctor should assess the possibility of surgical treatment." Message matches this, but drops the restriction to children without risk factors. Recommendation 2 (p. 43): "In the event of an OME in a child with no risk factors, a watchful waiting approach should be taken for 3 months". Abstract: "The impact of OME in cases of children with a comorbidity is higher, so it requires immediate reaction, without watchful waiting." The message should say the 3-month observation applies to children without risk factors. |
| faringoamigdalitis_no_estreptococica | consenso_italiano_faringitis_2024 | OK | Rec. 8a (p. 8): "Antibiotic therapy is not recommended in patients without comorbidities with pharyngotonsillitis and a culture and/or molecular test positive for bacteria other than GABHS." Conclusions (p. 8): "the disease is generally self-limiting and that antibiotic therapy does not significantly reduce the duration of symptoms or the risk of complications." Remark: the source says "does not significantly reduce"; Rec. 8b (infectious disease counselling if antibiotics are considered) is not mentioned. |
| laringitis | finlandia_vri_2023 | OK | Table 3 (p. 253): "Laryngitis — No antibiotics". Abstract (p. 248): "laryngitis should be treated with oral corticosteroids, with adrenaline inhalations added in severe cases". |
| bronquitis_aguda | finlandia_vri_2023 | OK (wording) | Table 3: "Acute bronchitis (cough) — No antibiotics". p. 252: "Antitussive and beta-sympathomimetic drugs are ineffective in relieving acute coughs in children and antitussives may cause serious adverse events (strong)". The message says «broncodilatadores»; the source says "beta-sympathomimetic drugs" (narrower). |
| bronquiolitis | finlandia_vri_2023 | OK | Table 3: "Bronchiolitis — No antibiotics". |
| bronquitis_sibilante | finlandia_vri_2023 | OK | Table 3: "Wheezing bronchitis — No antibiotics". |
| gastroenteritis_aguda | francia_gastroenteritis_2017 | OK (wording) | p. S27: "most often due to viruses (notably Rotavirus and Norovirus)"; "usually self-limited regardless of the etiologic microorganism"; "there are very few situations where anti-infectives are indicated"; "only patients with proven shigellosis should systematically receive antibiotics"; "Antibiotics should also be prescribed in severe Campylobacter infections"; "Patients with typhoid fever … should also receive antibiotics"; "antibiotics are indicated in high-risk children". Remarks: «casi siempre es viral» is stronger than "most often"; the summary (p. S26) also lists *Vibrio cholerae* among the causes that warrant antibiotics, which the message omits. (The source prints "typhoid fever (Shigella typhi, Shigella paratyphi A, B, or C)" — an evident typo in the article for Salmonella; the CSV correctly follows Table 1.) |

---

## 3. Bibliographic data (`fuentes.csv`) against page 1 of each PDF

| id | Verdict | Check |
|---|---|---|
| consenso_italiano_oma_2025 | OK | "Antibiotic treatment of acute and recurrent otitis media in children: an Italian intersociety Consensus"; "Italian Journal of Pediatrics (2025) 51:50"; https://doi.org/10.1186/s13052-025-01894-z |
| sip_oma_2019 | OK | "Updated Guidelines for the Management of Acute Otitis Media in Children by the Italian Society of Pediatrics / Treatment"; "(Pediatr Infect Dis J 2019;38:S10–S21)"; "DOI: 10.1097/INF.0000000000002452" |
| aafp_oma_2019 | OK | "Otitis Media: Rapid Evidence Review"; "American Family Physician … Volume 100, Number 6 ◆ September 15, 2019"; pages 350–356 (PDF has 7 pages, last is 356). No DOI printed; `url` empty is consistent. |
| consenso_italiano_faringitis_2024 | OK | "Treatment of acute pharyngitis in children: an Italian intersociety consensus (SIPPS-SIP-SITIP-FIMP-SIAIP-SIMRI-FIMMG)"; "Italian Journal of Pediatrics (2024) 50:235"; https://doi.org/10.1186/s13052-024-01789-5 |
| idsa_faringitis_2012 | Could not verify | Not among the 31 PDFs. |
| finlandia_vri_2023 | OK | Title matches; "Acta Paediatrica. 2025;114:248–257"; "DOI: 10.1111/apa.17481". Remark: `anio` = 2025 is the volume year; the article is "© 2024" (accepted 22 October 2024) and the guideline is the 2023 update. Duodecim is named in the paper as the body that established the working group. |
| china_micoplasma_2023 | OK | "Evidence-based guideline for the diagnosis and treatment of Mycoplasma pneumoniae pneumonia in children (2023)"; "Pediatr Investig 2025 Mar; 9(1): 1–11"; "DOI: 10.1002/ped4.12469". Authorship line: "The Subspecialty Group of Respiratory, the Society of Pediatrics, Chinese Medical Association; China National Clinical Research Center of Respiratory Diseases; Editorial Board, Chinese Journal of Pediatrics" — `organizacion` names only the first. |
| francia_gastroenteritis_2017 | OK | "Antimicrobial treatment of diarrhea/acute gastroenteritis in children"; "Archives de Pédiatrie 2017;24:S26-S29"; "Groupe de Pathologie Infectieuse Pédiatrique de la Société Française de Pédiatrie (GPIP)". No DOI printed in the PDF. |
| codepeh_ome_2019 | OK | "Diagnosis and Treatment of Otitis Media With Effusion: CODEPEH Recommendations"; "Acta Otorrinolaringol Esp. 2019;70(1):36---46". No DOI printed in the PDF. |
| jfp_uso_racional_2016 | OK | "Antibiotic stewardship: The FP's role"; "THE JOURNAL OF FAMILY PRACTICE | DECEMBER 2016 | VOL 65, NO 12"; first page 876, last page 885. |

No bibliographic error found.

---

## 4. Paediatric dosing recommendations present in the 31 papers but not loaded in `esquemas.csv`

Only statements made as recommendations, guideline summaries or reference regimens are listed; trial arms are excluded unless they reproduce a stated standard regimen.

### 4.1 In the nine loaded sources

**consenso_italiano_oma_2025 (08)**
- Second choice after failure/relapse — Rec. 5, p. 8: "it is recommended to use the combination amoxicillin-clavulanate at a dose of 40 mg/kg/day of amoxicillin (0.5ml/kg/day) in 3 administrations, supplemented with a plain amoxicillin formulation (50 mg/kg/day or 1 ml/kg in 3 administrations) to ensure optimal doses of amoxicillin and clavulanic acid (<10 mg/Kg/day)." Same regimen for severe AOM with spontaneous perforation, Rec. 11, p. 11. (The loaded amoxicillin-clavulanate row is the US 14:1 regimen in two doses.)
- Recurrent AOM — Rec. 6, p. 8–9: "amoxicillin-clavulanate for 7—10 days is recommended as the first choice for episodes of AOM."
- Allergy — Rec. 8.1, p. 10: "alternative antibiotics (such as second/third-generation cephalosporins—cefuroxime or cefpodoxime proxetil)"; Rec. 8.2: "The use of macrolides (like clarithromycin or azytromicine) or clindamycin should be reserved for patients at high risk of allergic reactions." (no doses given).

**sip_oma_2019 (30)**
- Rec. 8, p. S16: "amoxicillin-clavulanic acid 80–90mg /kg/day (dose of amoxicillin) is recommended"; Table 4, p. S16: "Amoxicillin–acid clavulanic (80–90† mg/kg/day in 3 doses)"; Rec. 10, p. S16: "The division of amoxicillin or amoxicillin-clavulanic acid into 3 doses is recommended in all cases".
- Treatment failure, p. S17: "cefpodoxime proxetil 10mg/kg/day in 2 doses or cefuroxime axetil 30mg/kg/day in 2 doses"; "Ceftriaxone 50mg/kg/day as a single daily administration (intravenous or intramuscular) for 3 days was found to be effective in AOM episodes considered as treatment failure."
- Durations — Rec. 11, p. S17: "should be 10 days in children with risk factors for unfavorable evolution (<2 years old and/or with spontaneous otorrhea)"; Rec. 12: "may be reduced to 5 days in children with no risk factors".
- Analgesia (not an antibiotic), p. S11: "Systemic treatment with paracetamol 15 mg/kg/dose (up to 4 times daily) or with ibuprofen 10 mg/kg/dose (up to 3 times daily) by mouth is the treatment of choice."

**aafp_oma_2019 (21)** — Figure 2, p. 353
- "Cefdinir, 14 mg per kg per day in one dose or two divided doses"
- "Cefpodoxime, 10 mg per kg per day in two divided doses"
- "clindamycin, 30 to 40 mg per kg per day in three divided doses for three days, with or without a third-generation cephalosporin"
- Ceftriaxone as an initial option in the penicillin-allergy branch (same dose as row 4).

**consenso_italiano_faringitis_2024 (29)** (no doses given)
- Rec. 4b, p. 5: "a 3rd generation cephalosporin for 5 days".
- Rec. 5, p. 6 (relapsing GABHS, tonsillectomy candidates): "amoxicillin-clavulanic acid, clindamycin, or a combination of amoxicillin plus rifampicin in the last 4 days of treatment should be prescribed."

**finlandia_vri_2023 (31)** — Table 3, p. 253
- Outpatient CAP: "Mycoplasma pneumonia: Macrolide or doxycycline (children >8 years old) alone or combined with amoxicillin." Inpatients: "Mycoplasma pneumonia. Add macrolide or doxycycline (children >8 years old)".
- Footnote b: "Penicillin combined with clindamycin is recommended in Streptococcus pyogenes pneumonia." (no dose).
- Antiviral, not an antibiotic: "Oseltamivir (for 5 days) p.o. • <12 months 6 mg/kg/day. • Over 12 months and less than 15 kg 30 mg twice a day. • 16–23 kg 45 mg twice a day. • 24–40 kg 60 mg twice a day. • Over 40 kg 75 mg twice a day p.o."

**china_micoplasma_2023 (16)** — p. 7
- "Minocycline is initiated with a dosage of 4 mg/kg for the first dose, with a maximum dose of 200 mg, followed by 2 mg/(kg⋅dose) for subsequent doses, with a maximum dose of 100 mg, also administered every 12 h, either orally or via intravenous infusion."
- "Tetracycline is used according to the instructions: 6.25–12.5 mg/(kg⋅dose), with a maximum dose of 500 mg, once every 6 h, for a course of 10–14 days."
- "Levofloxacin is administered orally or intravenously, with 8–10 mg/(kg⋅dose) twice daily for children aged 6 months–<5 years; for children ≥ 5 years, the recommended dosage is 8–10 mg/(kg⋅dose), once daily, with a maximum of 750 mg/d."
- "Moxifloxacin … for children aged 3 months–<2 years, 6 mg/(kg⋅dose), for children aged 2–<6 years, 5 mg/(kg⋅dose), for children aged 6–<12 years, 4 mg/(kg⋅dose), administered once every 12 h, with a maximum of 200 mg per dose; for children aged 12–< 18 years, weighing less than 45 kg, 4 mg/(kg⋅dose), administered once every 12 h, with a maximum dose of 200 mg per administration; and for children weighing ≥ 45 kg, 400mg/dose, administered once a day."
- Rec. 5, p. 6: erythromycin or clarithromycin as alternatives to azithromycin ("the instructions are referred to").

**francia_gastroenteritis_2017 (09)**
- Yersinia, Table 1, p. S28: "Cotrimoxazole (oral) 30 mg/kg/day of sulfamethoxazole in 2 divided doses or Ceftriaxone (IV or IM) 50 mg/kg/day in once daily dose Treatment duration: 5 days"; alternatives "Doxycycline (oral) (after 8 years of age) 4 mg/kg in 2 divided doses (maximum 200 mg/day) Treatment duration: 5 days" and "Ciprofloxacin (oral) 20 to 30 mg/kg/ day in 2 divided doses (maximum 1500 mg/day) Treatment duration: 5 days".
- Campylobacter comment, p. S28: "Other macrolides than azithromycin could be used for a duration treatment of 5 days."
- Amoebiasis alternatives, Table 2, p. S29: "Tinidazole (oral) 50 mg/kg/day Treatment duration: 3–5 days or Ornidazole (oral) 30 mg/kg (child) Treatment duration: 7 days".
- Giardia alternatives, Table 2, p. S29: "Tinidazole (oral) 50–70 mg/kg/day in a single dose or Albendazole (oral) 400 mg/day Treatment duration: 3–5 days or Ornidazole (oral) 30 mg/kg/day Treatment duration: 5 days".

**jfp_uso_racional_2016 (05)**
- Table 1, p. 878: "Linezolid … Children: 10 mg/kg q12h x 5-10 d".
- Table 3, p. 881 (first-line therapy, children): acute otitis media "High-dose amoxicillin or amoxicillin-clavulanate* — Children: 5-7 d"; acute bacterial rhinosinusitis "Amoxicillin-clavulanate … Children: 10-14 d"; community-acquired pneumonia "Amoxicillin / Azithromycin … Children: 5-10 d". Footnote: "*High-dose amoxicillin: 1 g TID in adults and 80-90 mg/kg/d in 2 divided doses in children; high-dose amoxicillin-clavulanate: 2000/125 mg BID in adults and 90/6.4 mg/kg/d in 2 divided doses in children."

### 4.2 In the other papers

- **06 — Antibiotic therapy for children with acute otitis media (Can Fam Physician 2017)**, p. 685–686: "Amoxicillin is the drug of choice if antibiotics are required, given as 45 to 60 mg/kg per day in 3 divided doses; if a twice-daily dosing regimen is used, higher total daily doses of 75 to 90 mg/kg per day are required." … "Consider a second-generation (cefprozil or cefuroxime) or third-generation cephalosporin if the child has a nonsevere allergic reaction to amoxicillin or penicillin." … "Consider 10 days' duration of antibiotics in children younger than 2 years of age and those with recurrent AOM or AOM with perforated TMs; 5 days of antibiotics might be appropriate for children 2 years of age or older with uncomplicated disease."
- **28 — Treatment of Acute Otitis Media in the Pediatric Emergency Department** (Israeli guideline summary), PDF p. 4–5: "first-line therapy is amoxicillin at 60-80 mg/kg, divided into 2-3 daily doses, for 7 days" … "second-line antibiotic therapy (such as amoxicillin-clavulanic acid or cefuroxime) … Ceftriaxone is considered to be a third-line antibiotic therapy."
- **23 — Panel 7: Otitis Media Treatment and Complications**, Table 6 footnote, PDF p. 55: "High dose amoxicillin/amox-clav: 80-90mg/kg/d of amoxicillin; low dose amoxicillin: 40mg/kg/d of amoxicillin" (USA and South Korea: high dose; Japan and the Netherlands: low dose first).
- **25 — Quality of antibiotic prescribing for pediatric CAP in outpatient care** (guideline parameters used as the standard), PDF p. 3: "Amoxicillin [40-90 mg/Kg/day; max 4 g/day; x 5–7 days] was the primary treatment for outpatient pediatric CAP. Alternative first-line treatments included: Penicillin [25-50 mg/Kg/day; max 2 g/day; x 5–7 days]; Ampicillin [50-100 mg/Kg/day; max 2 g/day; x 5–7 days]; and Amoxicillin-clavulanate [90 mg/Kg/day; max 4 g/Kg/day; x 5–7 days]." (the "4 g/Kg/day" is printed so in the article).
- **19 — Multistep antimicrobial stewardship intervention … pneumonia**, PDF p. 5: "The CP suggested amoxicillin of 90 mg/kg/day divided every 8 hours as first-line treatment, in line with CAP guidelines [7]."; PDF p. 10: "the first-line therapy for CAP should be based on amoxicillin alone (90 mg/kg/die for seven days)".
- **10 — Appropriateness of antibiotic prescribing … emergency department** (Spanish), PDF p. 5: "La mayoría de los consensos recomiendan dosis de 80 mg/kg/día" (amoxicillin, CAP).
- **11 — Community-acquired bacterial pneumonia in children: an update**, PDF p. 5: "Emerging data on antibiotic duration for CAP uniformly support a shortened course of amoxicillin-based therapy (5 days) for otherwise healthy children with mild CAP treated outpatient." (duration only, no dose).
- **24 and 01 — Penicillin V trials (Sweden)**: weight-banded regimen whose 10-day arm is described as the currently recommended treatment. 24, PDF p. 3: "The dosages for children up to 40 kg were adjusted according to weight (10-20 kg: 250 mg per dose, 20-40 kg: 500 mg per dose, irrespective of treatment arm)."; 01, Table 1, PDF p. 3: ">40 kg 800 mg × 4 / 1000 mg × 3; 20–40 kg 500 mg × 4 / 500 mg × 3; 10–20 kg 250 mg × 4 / 250 mg × 3" (5-day / 10-day). Penicillin V is not in the knowledge base at all.
- **18 — Interventions for bacterial folliculitis and boils (Cochrane)**, Table 2 "Regimens and drug-drug interactions of systemic antibiotics", PDF p. 95–97 (reference regimens, not review conclusions): cefadroxil "Paediatric: 30 mg/kg orally once daily or in equally divided doses every 12 hours"; clindamycin "Paediatric: 8 to 16 mg/kg/d orally divided every 6 to 8 hours; 15 to 20 mg/kg/d IV or IM divided every 6 to 8 hours"; tetracyclines "Paediatric: (older than 8 years) 25 to 50 mg/kg orally in 4 equally divided doses"; trimethoprim-sulfamethoxazole "Paediatric: (older than 1 month) based on trimethoprim component: 8 to 12 mg/kg/d orally in 2 divided doses"; linezolid "Paediatric: (birth through 11 years) 10 mg/kg IV or orally every 12 hours". Note the oral clindamycin range (8–16 mg/kg/d) differs from the JFP range loaded in row 37 (10–20 mg/kg/d).
- **12 — Development of a mobile application for vancomycin dosing calculation**, PDF p. 3: "Table 2 Vancomycin dosing nomogram for pediatric patients according to total body weight and renal function" (intravenous loading and maintenance doses by weight band and creatinine clearance). Not transcribed here: the extracted table is scrambled and was not needed for any loaded row; it must be read from the PDF page if it is to be used.
- **20 — Optimizing antimicrobial therapy in children**, p. S94: oral step-down in osteomyelitis — "generally was 2-fold the usual dose (e.g., 100–150 mg/kg/day of cephalexin and 40 mg/kg/day of clindamycin) and drug was administered every 6 h. It is our practice to continue antibiotic therapy in acute uncomplicated osteomyelitis for 10–14 days after all of the following: defervescence, observable resolution of pain on weight-bearing or use, and fall of CRP to <1 mg/dL." Antiviral, p. S95: "Recommended acyclovir dose is 10–15 mg/kg/dose for HSV encephalitis in patients 3 months to 12 years".
- **26 — Mycobacterium abscessus otomastoiditis** (single-centre strategy, case-based), PDF p. 2: "intravenous imipenem-cilastatin (60/60 mg/kg/day in 2 to 4 doses) and tigecycline (2.4 mg/kg/day in 2 doses), oral treatment with azithromycin (10 mg/kg once daily)". Specialist regimen; listed for completeness only.
- **22 — Acute mastoiditis series**, PDF p. 2: describes local practice, not a recommendation ("first line with ceftriaxone (50–100 mg/kg, according to the clinical condition of the patient)").

Papers with trial arms only and no dosing recommendation: 03, 04, 07, 14, 15, 27. Paper 17 (flucytosine) gives adult-oriented dosing; paper 02 (JAMA patient page) and 13 (CODEPEH) give no doses.

---

## 5. Summary of what should change

1. Row 6 (OMA, claritromicina): correct the `nota` — SIP p. S16 states 2 daily administrations.
2. Row 3 (OMA, amoxicilina-clavulanato): the 7:1 / 10 mg/kg/day clavulanate sentence is from the Italian documents, not from AAFP.
3. Row 5 (OMA, cefuroxima): "alergia no grave" is from SIP Rec. 9, not AAFP; AAFP lists four equivalent options.
4. Row 7 (faringoamigdalitis, amoxicilina): `max_por_dia` = 3000 is not part of the recommendation and the source sentence concerns children under 40 kg.
5. Rows 15 and 23 (claritromicina, Finnish): interval not in the source — needs another reference.
6. Row 21 (micoplasma, doxiciclina): not "second-line" in the source; recommended for severe/refractory MPP at ≥8 years.
7. Row 37 (SARM, clindamicina): source names TMP/SMX or a long-acting tetracycline as first-line; clindamycin is "an option".
8. Rows 8–11 (IDSA sample rows): not verifiable with the available PDFs; row 10 does not match the regimen the Italian consensus recommends (3rd-generation cephalosporin, 5 days).
9. `pagina`: rows 2, 3, 5 should add p. 352 (duration text); row 27 should add p. S27.
10. Message `otitis_media_efusion`: add that the 3-month observation is for children without risk factors.
