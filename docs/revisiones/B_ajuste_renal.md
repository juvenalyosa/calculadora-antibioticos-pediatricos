# B. Renal dose adjustment in children — source review for 15 antibiotics

Date of review: 2026-11-17. Literature review only; no code.

## How to read this report

- Every number below is backed by a quote copied from a document that was downloaded and text-extracted during this review. Quotes are verbatim; table cells that were linearised by text extraction are joined with ` | `.
- "Population" says who the source wrote the recommendation for: **children** (with weight/age), **adults only**, or **unspecified**.
- "Units" matters: some sources use absolute mL/min, others mL/min/1.73 m². For children only the BSA-normalised value (e.g. bedside Schwartz) is meaningful; where a label says "mL/min" without normalisation that is flagged.
- Items that could not be opened are listed in section 17 as **not verified** and are not used for any number.

### Sources opened (abbreviations used in the tables)

| Abbrev. | Document | Issuing body | How fetched |
|---|---|---|---|
| EMA-A30 | Article 30 referral, Annex III (harmonised SmPC) for Amoxil, Augmentin, Rocephin, Zinnat, Zinacef, Ciprofloxacin Bayer | European Medicines Agency / European Commission | PDF downloaded, text extracted |
| EMA-A31 | Vancomycin Article 31 referral, Annex III | EMA | PDF downloaded |
| CIMA | Ficha técnica (HTML) | AEMPS (Spain) | `https://cima.aemps.es/cima/dochtml/ft/{n}/FT_{n}.html` |
| FDA | US prescribing information (SPL) | FDA / NLM DailyMed | `https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid={setid}` (XML via the DailyMed services API) |
| eMC | UK SmPC | MHRA-approved text hosted by eMC | `https://www.medicines.org.uk/emc/product/{id}/smpc/print` |
| KF | Kinderformularium (Dutch Paediatric Formulary) monograph, section "Nierfunctiestoornissen bij kinderen > 3 maanden" | NKFK (Nederlands Kenniscentrum voor Farmacotherapie bij Kinderen) | `https://www.kinderformularium.nl/geneesmiddel/{id}/{slug}` |
| AEP | Pediamécum monograph | Asociación Española de Pediatría, Comité de Medicamentos | `https://www.aeped.es/comite-medicamentos/pediamecum/{slug}` |
| WHO-MFC | WHO Model Formulary for Children 2010 | World Health Organization | `https://iris.who.int/handle/10665/44309` (PDF bitstream `https://iris.who.int/server/api/core/bitstreams/ce78cf9a-ab0f-446e-a602-8b7e858cce41/content`); page numbers below are PDF page numbers |
| LPCH | "Pediatric IV Antimicrobial Renal Dosing Recommendations (NOT Neonates)", revised 1/2024 | Stanford Medicine Children's Health antimicrobial stewardship (institutional guideline, IV drugs only) | `https://www.stanfordchildrens.org/content-public/pdf/antimicrobial-stewardship-program/pediatric-renal-antibiotic-dosing-recommendations.pdf` |

KF is the only source found that is written specifically for children, uses BSA-normalised GFR bands throughout, and covers all 15 drugs. Its bands are always "GFR … ml/min/1.73 m2" and apply to "kinderen > 3 maanden" (children older than 3 months).

General peer-reviewed caveats (open access, both opened):

- Schijvens AM, de Wildt SN, Schreuder MF. Pharmacokinetics in children with chronic kidney disease. Pediatr Nephrol 2020;35:1153–1172. https://pmc.ncbi.nlm.nih.gov/articles/PMC7248054 (full text read through the Europe PMC API). Quotes: "Consequently, current dosing guidance in pediatric patients with CKD in the Dutch Pediatric Formulary and other dosing guidelines, such as the online Pediatric Drug Handbook, is derived from adult data and often excludes young infants and neonates"; "In general, dose adjustment is unlikely to be required when < 30% of the dose is excreted by the kidney".
- Minotti C, et al. Anti-infective Medicines Use in Children and Neonates With Pre-existing Kidney Dysfunction: A Systematic Review. Front Pediatr 2022;10:868513. https://www.frontiersin.org/journals/pediatrics/articles/10.3389/fped.2022.868513/pdf. Quote: "Dosing recommendations for anti-infective medicines in children with pre-existing kidney dysfunction are derived from adult pharmacokinetics studies and adjusted to kidney function." This review has no drug-level data for the oral agents in this list.

Consequence for the calculator: almost every paediatric band below is an extrapolation from adult data. None of the sources covers neonates or infants under 3 months.

---

## 1. Amoxicillin (oral)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| > 30 | No adjustment | Adults and children ≥ 40 kg; children < 40 kg | GFR, mL/min (not normalised) | EMA-A30 Amoxil §4.2; CIMA 63354 |
| 10–30 | 15 mg/kg twice daily (max 500 mg twice daily) | Children < 40 kg | GFR, mL/min | EMA-A30; CIMA |
| 10–30 | max 500 mg twice daily | Adults and children ≥ 40 kg | GFR, mL/min | EMA-A30; CIMA |
| < 10 | 15 mg/kg as a single daily dose (max 500 mg) | Children < 40 kg | GFR, mL/min | EMA-A30; CIMA |
| < 10 | max 500 mg/day | Adults and children ≥ 40 kg | GFR, mL/min | EMA-A30; CIMA |
| 10–30 | 500 mg or 250 mg every 12 h | Adults and paediatric patients ≥ 3 months AND > 40 kg only | GFR, mL/min | FDA §2.5 |
| < 10 | 500 mg or 250 mg every 24 h | same | GFR, mL/min | FDA §2.5 |
| < 30 | Do not use the 875 mg dose | same | GFR, mL/min | FDA §2.5 |
| ≥ 10 | No adjustment | Children > 3 months | mL/min/1.73 m² | KF (changed 5 Feb 2026) |
| < 10 | No general advice can be given | Children > 3 months | mL/min/1.73 m² | KF |
| Severe | "reduce dose" (no number) | Children | not stated | WHO-MFC |

Quotes

- EMA-A30, Amoxil Annex III, SmPC §4.2 (PDF created 23 Jul 2015). https://www.ema.europa.eu/en/documents/referral/amoxil-article-30-referral-annex-iii_en.pdf
  "Renal impairment | GFR (ml/min) Adults and children ≥ 40 kg Children < 40 kg# | greater than 30 no adjustment necessary no adjustment necessary | 10 to 30 maximum 500 mg twice daily 15 mg/kg given twice daily (maximum 500 mg twice daily) | less than 10 maximum 500 mg/day. 15 mg/kg given as a single daily dose (maximum 500 mg) | # In the majority of cases, parenteral therapy is preferred."
  Haemodialysis: "15 mg/kg/day given as a single daily dose. Prior to haemodialysis one additional dose of 15 mg/kg should be administered. In order to restore circulating drug levels, another dose of 15 mg/kg should be administered after haemodialysis." Peritoneal dialysis: "Amoxicillin maximum 500 mg/day."
- CIMA, Amoxicilina Normon 250 mg/5 ml polvo para suspensión oral EFG, §4.2, revisión "Marzo 2026". https://cima.aemps.es/cima/dochtml/ft/63354/FT_63354.html
  "GFR (ml/min) | Adultos y niños ≥ 40 kg | Niños < 40 kg # | mayor de 30 | no es necesario ajuste | no es necesario ajuste | 10 a 30 | máximo 500 mg dos veces al día | 15 mg/kg administrado dos veces al día (má ximo 500 mg dos veces al día) | menor de 10 | máximo 500 mg/día | 15 mg/kg administrado como dosis única diaria (máximo 500 mg) | # En la mayoría de casos, se elegirá tratamiento parenteral."
- FDA, Amoxicillin (Chartwell RX; tablets, capsules, chewable, suspension), setid b07b5ac4-253e-4c83-91c3-3fdc46e91a0f, effective 2026-10-06. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=b07b5ac4-253e-4c83-91c3-3fdc46e91a0f
  "2.5 Dosage in Renal Impairment for Adults and Pediatric Patients Aged 3 Months and Older and Weight Greater than 40 kg | Patients with impaired renal function do not generally require a reduction in dose unless the impairment is severe. Renal impairment patients with a glomerular filtration rate of less than 30 mL/min should NOT receive the 875 mg dose." Table 2: "GFR 10 to 30 mL/min | 500 mg or 250 mg every 12 hours, depending on the severity of the infection | GFR less than 10 mL/min | 500 mg or 250 mg every 24 hours, depending on severity of the infection | Hemodialysis | 500 mg or 250 mg every 24 hours, depending on severity of the infection | Administer an additional dose both during and at the end of dialysis".
  §2.3 (patients < 12 weeks): "There are currently no dosing recommendations for pediatric patients with impaired renal function."
- KF, Amoxicilline. https://www.kinderformularium.nl/geneesmiddel/7/amoxicilline
  "Nierfunctiestoornissen bij kinderen > 3 maanden | GFR ≥10 ml/min/1.73m2: aanpassing van de dosering is niet nodig. | GFR <10 ml/min/1.73m2: een algemeen advies kan niet worden gegeven." Clinical note: "Gebruik langer dan 10 dagen of in doseringen > 150 mg/kg/dag kan bij nierfunctiestoornissen leiden tot toxische spiegels. Monitor in deze gevallen de bloedspiegels." Change log: "05 februari 2026 11:40: Dosisaanpassing bij nierfunctiestoornissen is niet meer nodig. Wel wordt TDM aanbevolen bij gebruik > 10 dagen en doseringen > 150 mg/kg/dag."
- AEP Pediamécum, Amoxicilina, "Fecha de actualización: Abril 2021". https://www.aeped.es/comite-medicamentos/pediamecum/amoxicilina
  "Si ClCr 10-30 ml/min, 15 mg/kg/día en dos dosis si menor de 40 kg y 500 mg máximo dos veces al día si niños de más de 40 kg. | Si ClCr <10 ml/min, 15 mg/kg/día en una dosis si menor de 40 kg y 500 mg/día máximo si niños de más de 40 kg."
- WHO-MFC 2010, Amoxicillin, PDF p. 98: "Renal impairment: Mild to moderate: risk of crystalluria with high doses. Severe: reduce dose; rashes more common and risk of crystalluria."

Confidence / caveats: High confidence that EMA/AEMPS give a paediatric mg/kg rule. Three real problems: (a) the Pediamécum wording "15 mg/kg/día en dos dosis" reads as 15 mg/kg per DAY split in two, which is half of the EMA "15 mg/kg given twice daily"; the EMA/CIMA text is the primary document and should prevail. (b) KF dropped the adjustment entirely for GFR ≥ 10 in February 2026. (c) The FDA label has no paediatric < 40 kg renal rule at all, and its Highlights section carries an apparent typo: "Reduce the dose in patients with severe renal impairment (GFR greater than 30 mL/min)" (full text §8.6 says "GFR less than 30 mL/min"). EMA/FDA bands are in absolute mL/min.

---

## 2. Amoxicillin–clavulanate (oral) — by ratio

| Ratio (examples) | Band | Change | Population | Units | Source |
|---|---|---|---|---|---|
| All | > 30 | No adjustment | all | CrCl, mL/min | EMA-A30 Augmentin; CIMA; FDA |
| 4:1 (500/125; 125/31.25 and 250/62.5 per 5 mL) | 10–30 | 15 mg/3.75 mg/kg twice daily (max 500 mg/125 mg twice daily) | Children < 40 kg | CrCl, mL/min | EMA-A30; CIMA 56683 |
| 4:1 | < 10 | 15 mg/3.75 mg/kg as a single daily dose (max 500 mg/125 mg) | Children < 40 kg | CrCl, mL/min | EMA-A30; CIMA 56683 |
| 4:1 | 10–30 / < 10 | 500 mg/125 mg twice daily / once daily | Adults and children ≥ 40 kg | CrCl, mL/min | EMA-A30; CIMA |
| 2:1 (250/125; 125/62.5 per 5 mL) | < 30 | Not recommended in children < 40 kg; use 4:1 | Children < 40 kg | CrCl, mL/min | EMA-A30 |
| 7:1 (875/125; 400/57 and 200/28.5 per 5 mL) | < 30 | Not recommended (avoid) | all | CrCl, mL/min | EMA-A30; CIMA 59518 |
| 8:1 (100/12.5 per mL; 500/62.5; 1000/125) | < 30 | Not recommended (avoid) | all | CrCl, mL/min | EMA-A30; CIMA 59051 |
| 14:1 (600/42.9 per 5 mL) | < 30 | Not recommended (avoid) | all | CrCl, mL/min | EMA-A30 |
| 14:1 (Augmentin ES-600, US) | any | No renal dosing section in the label | children | — | FDA ES-600 label |
| not ratio-specific (US) | < 30 | No 875 mg dose; 500 mg or 250 mg q12h (GFR 10–30) / q24h (< 10) | unspecified (mg doses = adult-type) | GFR, mL/min | FDA §2.4 |
| not ratio-specific | 10–30 | 100% of normal single dose, interval 12 h | Children > 3 months | mL/min/1.73 m² | KF |
| not ratio-specific | < 10 | 100% of normal single dose, interval 24 h | Children > 3 months | mL/min/1.73 m² | KF |

Quotes

- EMA-A30, Augmentin Annex III, SmPC §4.2 (PDF created 13 Nov 2009). https://www.ema.europa.eu/en/documents/referral/augmentin-article-30-annex-iii_en.pdf
  4:1 SmPC (500 mg/125 mg tablets; 125 mg/31.25 mg and 250 mg/62.5 mg suspension): "Dose adjustments are based on the maximum recommended level of amoxicillin. No adjustment in dose is required in patients with creatinine clearance (CrCl) greater than 30 ml/min." … "Children < 40 kg | CrCl: 10-30 ml/min 15 mg/3.75 mg/kg twice daily (maximum 500 mg/125 mg twice daily). | CrCl < 10 ml /min 15 mg/3.75 mg/kg as a single daily dose (maximum 500 mg/125 mg). | Haemodialysis 15 mg/3.75 mg/kg per day once daily. Prior to haemodialysis 15 mg/3.75 mg/kg. In order to restore circulating drug levels, 15 mg/3.75 mg per kg should be administered after haemodialysis."
  2:1 SmPC: "In children < 40 kg with creatinine clearance less than 30 ml/min, the use of Augmentin presentations with an amoxicillin to clavulanic acid ratio of 2:1 is not recommended, as no dose adjustments are available. In such patients, Augmentin formulations with an amoxicillin to clavulanic acid ratio of 4:1 are recommended."
  7:1 SmPC: "In patients with creatinine clearance less than 30 ml/min, the use of Augmentin presentations with an amoxicillin to clavulanic acid ratio of 7:1 is not recommended, as no recommendations for dose adjustments are available."
  8:1 SmPC: "In patients with creatinine clearance less than 30 ml/min, the use of Augmentin presentations with an amoxicillin to clavulanic acid ratio of 8:1 is not recommended, as no recommendations for dose adjustments are available."
  14:1 SmPC (600 mg/42.9 mg/5 ml): "In patients with creatinine clearance less than 30 ml/min, the use of Augmentin is not recommended, as no recommendations for dose adjustments are available."
  (Extraction note: in the adult heading the "≥" glyph was lost by the PDF text layer: "Adults and children  40 kg".)
- CIMA, Augmentine 500 mg/125 mg polvo para suspensión oral en sobres (4:1), §4.2, revisión "Mayo 2024". https://cima.aemps.es/cima/dochtml/ft/56683/FT_56683.html
  "Niños < 40 kg | CrCl: 10-30 ml/min | 15 mg/3,75 mg/kg dos veces al día (máximo 500 mg/125 mg dos veces al día). | CrCl < 10 ml /min | 15 mg/3,75 mg/kg como dosis única diaria (máximo 500 mg/125 mg)."
- CIMA, Augmentine 100 mg/ml + 12,5 mg/ml polvo para suspensión oral (8:1), revisión "Mayo 2024". https://cima.aemps.es/cima/dochtml/ft/59051/FT_59051.html
  "En pacientes con aclaramiento de creatinina menor de 30 ml/min, no se recomienda el uso de presentaciones en proporción 8:1, ya que no hay recomendaciones de ajuste de dosis disponibles."
- CIMA, Augmentine 875 mg/125 mg sobres (7:1), revisión "Mayo 2024". https://cima.aemps.es/cima/dochtml/ft/59518/FT_59518.html
  "En pacientes con aclaramiento de creatinina menor de 30 ml/min, no se recomienda el uso de las presentaciones de Augmentine de proporción 7:1, ya que no hay recomendaciones de ajustes de dosis disponibles."
- FDA, Augmentin for suspension (Allegis), setid 04a31a42-d79e-bbb4-e063-6394a90a63ee, effective 2026-10-26. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=04a31a42-d79e-bbb4-e063-6394a90a63ee
  "2.4 Patients with Renal Impairment | Patients with impaired renal function do not generally require a reduction in dose unless the impairment is severe. Renal impairment patients with a glomerular filtration rate (GFR) of less than 30 mL/min should NOT receive the 875 mg dose (based on the amoxicillin component) of AUGMENTIN." Table 4: "GFR 10 mL/min to 30 mL/min | 500 mg or 250 mg every 12 hours, depending on the severity of the infection | GFR less than 10 mL/min | 500 mg or 250 mg every 24 hours, depending on severity of the infection | Hemodialysis | 500 mg or 250 mg every 24 hours, depending on severity of the infection | Administer an additional dose both during and at the end of dialysis".
- FDA, Augmentin ES-600 (14:1), setid 59979859-8c9b-4173-a5e3-2e6312fd9d79, effective 2026-07-07: the label text contains no renal dosing section; the only renal statement is in Overdosage: "High blood levels may occur more readily in patients with impaired renal function because of decreased renal clearance of both amoxicillin and clavulanate." https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=59979859-8c9b-4173-a5e3-2e6312fd9d79
- KF, Amoxicilline + clavulaanzuur. https://www.kinderformularium.nl/geneesmiddel/307/amoxicilline-clavulaanzuur
  "GFR 50-80 ml/min/1.73 m2 | Aanpassing is niet nodig | GFR 30-50 ml/min/1.73 m2 | Aanpassing is niet nodig | GFR 10-30 ml/min/1.73 m2 | 100 % van normale keerdosis en interval tussen twee doseringen: 12 uur | GFR < 10 ml/min/1.73 m2 | 100 % van normale keerdosis en interval tussen twee doseringen: 24 uur"
- AEP Pediamécum, Amoxicilina-clavulánico, "Abril 2021". https://www.aeped.es/comite-medicamentos/pediamecum/amoxicilina-clavulanico
  "ClCr = 10-30 ml/min: 15/3,75 mg/kg (empleando sobres) cada 12 horas (en <12 años). | ClCr <10 ml/min: 15/3,75 mg/kg (empleando sobres) cada 24 horas (en <12 años)."
- WHO-MFC 2010, Amoxicillin + clavulanic acid, PDF p. 99: "Renal impairment: Risk of crystalluria with high doses (particularly during parenteral therapy); reduce dose if creatinine clearance less than 30 ml/minute."

Confidence / caveats: High. The key design fact is ratio-dependence: only the 4:1 ratio has a paediatric adjusted dose in the regulatory text; 7:1, 8:1 and 14:1 are "not recommended" below CrCl 30 (EMA), and the US 14:1 label is silent. KF gives an interval-only rule without mentioning ratio, which conflicts with EMA for 7:1/14:1 products. AEP defines the paediatric group by age (< 12 years), EMA by weight (< 40 kg).

---

## 3. Ceftriaxone (IM/IV)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| any, liver normal | No dose reduction | unspecified | — | EMA-A30 Rocephin; CIMA; FDA |
| CrCl < 10 | Dose not to exceed 2 g daily | unspecified (g cap is adult-type) | mL/min | EMA-A30; CIMA |
| GFR < 10 | max 2 g/day | Children > 3 months | mL/min/1.73 m² | KF |
| Severe | max 50 mg/kg daily (max 2 g daily) | Children | not stated | WHO-MFC; AEP |
| Renal + hepatic dysfunction | ≤ 2 g daily, monitor | unspecified | — | FDA |
| Dialysis | No supplementary dose | unspecified | — | EMA-A30; FDA |

Quotes

- EMA-A30, Rocephin Annex III, SmPC §4.2 (PDF created 7 May 2014). https://www.ema.europa.eu/en/documents/referral/rocephin-article-30-referral-annex-iii_en.pdf
  "In patients with impaired renal function, there is no need to reduce the dosage of ceftriaxone provided hepatic function is not impaired. Only in cases of preterminal renal failure (creatinine clearance < 10 ml/min) should the ceftriaxone dosage not exceed 2 g daily. In patients undergoing dialysis no additional supplementary dosing is required following the dialysis. Ceftriaxone is not removed by peritoneal- or haemodialysis."
- CIMA, Ceftriaxona Normon 1000 mg, §4.2, revisión "Marzo 2026". https://cima.aemps.es/cima/dochtml/ft/62635/FT_62635.html
  "En pacientes con insuficiencia renal no es necesario reducir la dosis de ceftriaxona si la función hepática no está alterada. Solo en la insuficiencia renal preterminal (aclaramiento de creatinina < 10 ml/min) no se debe sobrepasar la dosis de 2 g de ceftriaxona diarios."
- FDA, Ceftriaxone for injection (Hospira), setid 36c07ebf-b091-483e-b2c4-f0fc0c2b4186, effective 2026-11-12. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=36c07ebf-b091-483e-b2c4-f0fc0c2b4186
  Dosage and Administration: "No dosage adjustment is necessary for patients with impairment of renal or hepatic function". Precautions: "patients with renal failure normally require no adjustment in dosage when usual doses of ceftriaxone are administered. Dosage adjustments should not be necessary in patients with hepatic dysfunction; however, in patients with both hepatic dysfunction and significant renal disease, caution should be exercised and the ceftriaxone dosage should not exceed 2 g daily."
- KF, Ceftriaxon. https://www.kinderformularium.nl/geneesmiddel/129/ceftriaxon — "GFR ≥10 ml/min/1.73m2: aanpassing van de dosering is niet nodig. | GFR <10 ml/min/1.73m2: maximaal 2 g/dag."
- WHO-MFC 2010, Ceftriaxone, PDF p. 107: "Renal impairment: Severe: maximum 50 mg/kg daily (maximum 2 g daily); also monitor plasma concentration if both severe renal impairment and hepatic impairment."
- AEP Pediamécum, Ceftriaxona, "Noviembre 2020". https://www.aeped.es/comite-medicamentos/pediamecum/ceftriaxona — "Dosis máxima 50 mg/kg/día (máx. 2 g diarios) si insuficiencia renal grave. No dializable (0-5%)."
- LPCH 2024: "Ceftriaxone 50mg/kg q24h Meningitis: 50mg/kg q12h No adjustment necessary 2000 mg".

Confidence / caveats: High; sources agree. The only paediatric mg/kg expression of the cap (50 mg/kg/day, max 2 g) comes from WHO-MFC/AEP, not from the regulatory text.

---

## 4. Cefuroxime axetil (oral) and cefuroxime sodium (IV)

### 4a. Cefuroxime axetil (oral)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| ≥ 30 | No adjustment | unspecified ("standard individual dose") | mL/min/1.73 m² | EMA-A30 Zinnat; CIMA 62806 |
| 10–29 | Standard individual dose every 24 h | unspecified | mL/min/1.73 m² | EMA-A30; CIMA |
| < 10 | Standard individual dose every 48 h | unspecified | mL/min/1.73 m² | EMA-A30; CIMA |
| Haemodialysis | One further standard dose at end of each dialysis | unspecified | — | EMA-A30; CIMA; FDA |
| ≥ 30 / 10 to < 30 / < 10 | none / q24h / q48h | **Adults** (table title) | mL/min | FDA §2.5 |
| 10–30 / < 10 | 100% single dose q24h / q48h (oral) | Children > 3 months | mL/min/1.73 m² | KF |

Quotes

- EMA-A30, Zinnat Annex III, SmPC §4.2 (PDF created 6 Sep 2012). https://www.ema.europa.eu/en/documents/referral/zinnat-article-30-referral-annex-iii_en.pdf
  "The safety and efficacy of cefuroxime axetil in patients with renal failure have not been established. Cefuroxime is primarily excreted by the kidneys. In patients with markedly impaired renal function it is recommended that the dosage of cefuroxime should be reduced to compensate for its slower excretion." Table 5: "≥30 mL/min/1.73 m2 | 1.4–2.4 | no dose adjustment necessary (standard dose of 125 mg to 500 mg given twice daily) | 10-29 mL/min/1.73 m2 | 4.6 | standard individual dose given every 24 hours | <10 mL/min/1.73 m2 | 16.8 | standard individual dose given every 48 hours | Patients on haemodialysis | 2–4 | a further standard individual dose should be given at the end of each dialysis".
- CIMA, Zinnat 250 mg/5 ml granulado para suspensión oral, §4.2 Tabla 7, revisión "09/2023". https://cima.aemps.es/cima/dochtml/ft/62806/FT_62806.html
  "≥30 ml/min/1,73 m 2 | 1,4 - 2,4 | No es necesario un ajuste de dosis estándar de 125 mg a 500 mg administrada dos veces al día | 10-29 ml/min/1,73 m 2 | 4,6 | Dosis individual estándar admin istrada cada 24 horas | <10 ml/min/1,73 m 2 | 16,8 | Dosis individual estándar administrada cada 48 horas".
- FDA, Cefuroxime axetil tablets, setid 15cabe73-f39d-4798-a840-115fe94f8f96, effective 2026-10-14. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=15cabe73-f39d-4798-a840-115fe94f8f96
  "Table 4. Dosing in Adults with Renal Impairment | Creatinine Clearance (mL/min) | Recommended Dosage | ≥30 | No dosage adjustment | 10 to ˂30 | Standard individual dose given every 24 hours | ˂10 (without hemodialysis) | Standard individual dose given every 48 hours | Hemodialysis | A single additional standard dose should be given at the end of each dialysis". §8.6: "Reducing the dosage of cefuroxime axetil is recommended for adult patients with severe renal impairment (creatinine clearance <30 mL/min)".
- KF, Cefuroxim. https://www.kinderformularium.nl/geneesmiddel/130/cefuroxim
  "GFR 10-30 ml/min/1.73 m2 | 100 % van normale keerdosis en interval tussen twee doseringen: bij parenterale toediening: 12 uur; bij orale toediening: 24 uur | GFR < 10 ml/min/1.73 m2 | 100 % van normale keerdosis en interval tussen twee doseringen: bij parenterale toediening: 24 uur; bij orale toediening: 48 uur" (50–80 and 30–50: "Aanpassing is niet nodig").

Confidence / caveats: High; EMA, AEMPS, FDA and KF give the same interval rule. EMA states safety/efficacy in renal failure are not established; FDA restricts its table to adults. AEP Pediamécum (cefuroxima, Oct 2020) has no renal dosing rule.

### 4b. Cefuroxime sodium (IV/IM)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| > 20 | No reduction (750 mg–1.5 g three times daily) | unspecified; mg doses are adult | mL/min/1.73 m² | EMA-A30 Zinacef; CIMA 62908 |
| 10–20 | 750 mg twice daily | adult-type mg | mL/min/1.73 m² | EMA-A30; CIMA |
| < 10 | 750 mg once daily | adult-type mg | mL/min/1.73 m² | EMA-A30; CIMA |
| > 20 / 10–20 / < 10 | q8h / 750 mg q12h / 750 mg q24h | Adults; children: "frequency … consistent with the recommendations for adults" | mL/min | FDA |
| 10–30 / < 10 | 100% single dose q12h / q24h (parenteral) | Children > 3 months | mL/min/1.73 m² | KF |

Quotes

- EMA-A30, Zinacef Annex III, SmPC §4.2 Table 3 (PDF created 6 Sep 2012). https://www.ema.europa.eu/en/documents/referral/zinacef-article-30-referral-annex-iii_en.pdf
  "> 20 mL/min/1.73 m2 | 1.7–2.6 | It is not necessary to reduce the standard dose (750 mg to 1.5 g three times daily). | 10-20 mL/min/1.73 m2 | 4.3–6.5 | 750 mg twice daily | < 10 mL/min/1.73 m2 | 14.8–22.3 | 750 mg once daily | Patients on haemodialysis | 3.75 | A further 750 mg dose should be given intravenously or intramuscularly at the end of each dialysis".
- CIMA, Cefuroxima Normon 750 mg, §4.2 Tabla 3, revisión "Julio 2023". https://cima.aemps.es/cima/dochtml/ft/62908/FT_62908.html
  "> 20 ml/min/1,73 m 2 | 1,7-2,6 | No es necesario reducir la dosis estándar (750 mg a 1.500 mg tres veces al día) | 10-20 ml/min/1,73 m 2 | 4,3-6,5 | 750 mg dos veces al día | < 10 ml/min/1,73 m 2 | 14,8-22,3 | 750 mg una vez al día".
- FDA, Cefuroxime for injection (Sagent), setid 0a145288-733a-4966-b7ae-dc96eb103d8c, effective 2025-03-27. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=0a145288-733a-4966-b7ae-dc96eb103d8c
  "Table 2. Dosage of Cefuroxime for Injection in Adults with Reduced Renal Function | Creatinine Clearance (mL/min) | Dose | Frequency | > 20 | 750 mg to 1.5 grams | q8h | 10 to 20 | 750 mg | q12h | < 10 | 750 mg | q24h a". Paediatric: "In pediatric patients with renal insufficiency, the frequency of dosing should be modified consistent with the recommendations for adults."
- KF: quote in 4a.

Confidence / caveats: Medium-high. Regulatory texts give adult mg doses only; the only paediatric instruction is the FDA sentence (change the frequency as in adults) and KF. Threshold disagreement: regulatory texts start adjusting at CrCl ≤ 20, KF at GFR < 30.

---

## 5. Cefalexin (oral)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| ≥ 60 | No adjustment | Adults and ≥ 15 years | CrCl, mL/min | FDA §2.4 |
| 30–59 | No adjustment; max daily dose 1 g | Adults and ≥ 15 years | mL/min | FDA |
| 15–29 | 250 mg every 8 h or every 12 h | Adults and ≥ 15 years | mL/min | FDA |
| 5–14 (not on dialysis) | 250 mg every 24 h | Adults and ≥ 15 years | mL/min | FDA |
| 1–4 (not on dialysis) | 250 mg every 48 h or every 60 h | Adults and ≥ 15 years | mL/min | FDA |
| 30–50 | 100% single dose, interval 12 h | Children > 3 months | mL/min/1.73 m² | KF |
| 10–30 | 100% single dose, interval 12 h | Children > 3 months | mL/min/1.73 m² | KF |
| < 10 | 100% single dose, interval 24 h | Children > 3 months | mL/min/1.73 m² | KF |
| GFR < 10 or dialysis | max 500 mg/day | Adults (capsule SmPC) | mL/min | CIMA 51115; eMC |
| "markedly impaired" | "Reduce dosage" (no number) | unspecified | — | eMC 9145, 551 |
| < 50 | "Ajuste" (no number) | Children | mL/min/1.73 m² | AEP |
| moderate | "Reduce dose" (no number) | Children | — | WHO-MFC |

Quotes

- FDA, Cephalexin for oral suspension (Teva), setid 5858b90a-a80d-40b4-9275-b89e9121ea42, effective 2026-09-02. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=5858b90a-a80d-40b4-9275-b89e9121ea42
  "Table 2. Recommended Cephalexin for Oral Suspension Dosage for Adult Patients and Pediatric Patients (at Least 15 Years of Age) with Renal Impairment | Renal function (Creatinine Clearance) | Dosage Recommendation | Greater than or equal to 60 mL/min | No dosage adjustment | 30 mL/min to 59 mL/min | No dosage adjustment; maximum daily dose should not exceed 1g | 15 mL/min to 29 mL/min | 250 mg, every 8 hours or every 12 hours | 5 mL/min to 14 mL/min not yet on dialysis* | 250 mg, every 24 hours | 1 mL/min to 4 mL/min not yet on dialysis* | 250 mg, every 48 hours or every 60 hours | *There is insufficient information to make dose adjustment recommendations in patients on hemodialysis."
- KF, Cefalexine. https://www.kinderformularium.nl/geneesmiddel/582/cefalexine
  "GFR 50-80 ml/min/1.73 m2 | Aanpassing is niet nodig | GFR 30-50 ml/min/1.73 m2 | 100 % van normale keerdosis en interval tussen twee doseringen: 12 uur | GFR 10-30 ml/min/1.73 m2 | 100 % van normale keerdosis en interval tussen twee doseringen: 12 uur | GFR < 10 ml/min/1.73 m2 | 100 % van normale keerdosis en interval tussen twee doseringen: 24 uur". Stated basis: "NKFK Werkgroep nierfunctiestoornissen, Extrapolatie van KNMP risico analyse "Verminderde nierfunctie" voor volwassenen naar kinderen, 20 Dec 2021".
- CIMA, Kefloridina Forte 500 mg cápsulas, §4.2, revisión "Septiembre 2021". https://cima.aemps.es/cima/dochtml/ft/51115/FT_51115.html
  "La dosis debe reducirse a un máximo diario de 500 mg si la función renal está gravemente afectada (tasa de filtración glomerular <10ml/min). Lo mismo aplica a los pacientes que requieren diálisis debido a fallo renal, la dosis diaria no debe exceder 500 mg".
- eMC, Keflex Suspension 250 mg/5 ml (Flynn), revised 22/11/2023. https://www.medicines.org.uk/emc/product/9145/smpc/print — §4.2 "Reduce dosage if renal function is markedly impaired (see section 4.4)."; §4.4 "If dialysis is required for renal failure, the daily dose of cefalexin should not exceed 500mg." Same wording in eMC product 551 (revised 26/06/2024): https://www.medicines.org.uk/emc/product/551/smpc/print
- AEP Pediamécum, Cefalexina, "Octubre 2020". https://www.aeped.es/comite-medicamentos/pediamecum/cefalexina — "Insuficiencia renal: | Ajuste si ClCr <50 ml/min/1,73 m 2 ."
- WHO-MFC 2010, Cefalexin, PDF p. 105: "Renal impairment: Reduce dose in moderate impairment."

Confidence / caveats: Medium. No regulatory text gives a paediatric (< 15 years) rule; KF is the only numeric paediatric source and is an explicit adult-to-child extrapolation. Thresholds for starting adjustment differ widely: 60 (FDA dose cap), 50 (AEP, KF), "markedly impaired" (UK), 10 (Spain). Note the KF rule cannot shorten a q12h regimen at GFR 10–50; it only bites on q6–8h regimens.

---

## 6. Azithromycin (oral)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| 10–80 | No adjustment | unspecified | GFR, mL/min | CIMA 59615 |
| ≥ 10 | No adjustment | unspecified | GFR, mL/min | eMC 3006 |
| < 10 | Use with caution (no dose given) | unspecified | GFR, mL/min | CIMA; eMC |
| ≥ 10 | No adjustment | Children > 3 months | mL/min/1.73 m² | KF |
| < 10 | No general advice | Children > 3 months | mL/min/1.73 m² | KF |
| any | No adjustment necessary (IV) | Children | mL/min/1.73 m² | LPCH |

Quotes

- CIMA, Zitromax 200 mg/5 ml polvo para suspensión oral, §4.2, revisión "Junio 2023". https://cima.aemps.es/cima/dochtml/ft/59615/FT_59615.html
  "No es necesario un ajuste posológico en pacientes con una tasa de filtración glomerular (TFG) entre 10-80 ml/min. Se debe tener precaución cuando se administra azitromicina a pacientes con TFG < 10 ml/min." §4.4: "En pacientes con una tasa de filtración glomerular < 10 ml/min se ha observado un incremento del 33% de la exposición sistémica a la azitromicina".
- eMC, Zithromax 200 mg in 5 ml suspension, revised 07/2026. https://www.medicines.org.uk/emc/product/3006/smpc/print
  "No dose adjustment is necessary in patients with GFR ≥ 10 ml/min. In patients with GFR < 10 ml/min azithromycin should be administered with caution (see section 5.2)."
- FDA, Zithromax tablets and oral suspension (Pfizer), setid db52b91e-79f7-4cc1-9564-f2eee8e31c45, effective 2026-07-24. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=db52b91e-79f7-4cc1-9564-f2eee8e31c45
  §12.3 only (pharmacokinetics, adults): "mean C max and AUC 0–120 increased by 5.1% and 4.2%, respectively, in subjects with GFR 10 to 80 mL/min compared to subjects with GFR >80 mL/min. The mean C max and AUC 0–120 increased 61% and 35%, respectively, in subjects with GFR <10 mL/min compared to subjects with GFR >80 mL/min." The current label text has no explicit renal dosing instruction.
- KF, Azitromycine. https://www.kinderformularium.nl/geneesmiddel/309/azitromycine — "GFR ≥10 ml/min/1.73m2: aanpassing van de dosering is niet nodig. | GFR <10 ml/min/1.73m2: een algemeen advies kan niet worden gegeven."
- AEP Pediamécum, Azitromicina, "Abril 2021". https://www.aeped.es/comite-medicamentos/pediamecum/azitromicina — "No es necesario ajustar la dosis en pacientes con alteración de la función renal, aunque debe administrarse con precaución en pacientes con alteración de la función renal grave (tasa de filtración glomerular <10 ml/min)."
- WHO-MFC 2010, Azithromycin, PDF p. 116: "Renal impairment: Use with caution in patients with severe renal impairment."
- LPCH 2024: "Azithromycin 10mg/kg x1, then 5mg/kg q24h x 4 days No adjustment necessary 500 mg".

Confidence / caveats: High for "no adjustment at GFR ≥ 10". Below 10 no source gives a dose, only caution.

---

## 7. Clarithromycin (oral)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| < 30 | Halve the dose, e.g. 7.5 mg/kg once daily; max 14 days | Children 6 months–12 years | CrCl, mL/min | CIMA 66388 |
| < 30 | Halve the dose to 7.5 mg/kg per day; not beyond 14 days | Children | mL/min/1.73 m² | eMC 12806 |
| < 30 | 250 mg once daily, or 250 mg twice daily in severe infections; max 14 days | Adults/adolescents | mL/min | CIMA 60515, 66388 |
| < 30 | Reduce dosage by 50% | unspecified | CLcr, mL/min | FDA §2.6 |
| 30–60 + atazanavir/ritonavir | Reduce by 50% | unspecified | mL/min | FDA |
| < 30 + atazanavir/ritonavir | Reduce by 75% | unspecified | mL/min | FDA |
| 10–30 | 100% single dose, interval 24 h | Children > 3 months | mL/min/1.73 m² | KF |
| < 10 | No general recommendation | Children > 3 months | mL/min/1.73 m² | KF |
| 10–29 / < 10 (alternative, secondary) | 4 mg/kg q12h / 4 mg/kg q24h | Children | mL/min/1.73 m² | AEP citing "otras fuentes" |

Quotes

- CIMA, Claritromicina Sandoz 25 mg/ml granulado para suspensión oral, §4.2, revisión "05/2024". https://cima.aemps.es/cima/dochtml/ft/66388/FT_66388.html
  Paediatric paragraph (after "Niños de 6 meses a 12 años"): "En caso de insuficiencia renal, especialmente si el aclaramiento de creatinina es <30 ml/min, se debe reducir la dosis a la mitad, por ejemplo 7,5 mg/kg una vez al día, y la duración del tratamiento no debe exceder de 14 días." Adult paragraph: "Con tasas de aclaramiento de creatinina inferiores a 30 ml/min, se debe reducir la dosis a 250 mg diariamente o, en las infecciones más graves, a 250 mg dos veces al día. La duración del tratamiento no debe exceder los 14 días en estos pacientes."
- eMC, Clarithromycin 125 mg/5 ml granules for oral suspension (Ranbaxy/Sun), revised 24/07/2026. https://www.medicines.org.uk/emc/product/12806/smpc/print
  "Renal Impairment | In children with creatinine clearance less than 30 ml/min/1.73 m 2 , the dosage of clarithromycin should be reduced by half to 7.5 mg/kg per day. Dosage should not be continued beyond 14 days in these patients."
- CIMA, Klacid 500 mg comprimidos, §4.2, revisión "Junio 2024". https://cima.aemps.es/cima/dochtml/ft/60515/FT_60515.html
  "En pacientes con insuficiencia renal y un aclaramiento de creatinina menor de 30 ml/minuto la dosis de claritromicina debe reducirse a la mitad , es decir, 250 mg una vez al día, ó 250 mg dos veces al día en infecciones más graves. En estos pacientes el tratamiento debe ser interrumpido a los 14 días."
- FDA, Clarithromycin for oral suspension (Sandoz), setid 22457862-0f88-4be8-b507-1c8f264269f2, effective 2025-03-25. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=22457862-0f88-4be8-b507-1c8f264269f2
  "Table 2. Clarithromycin Dosage Adjustments in Patients with Renal Impairment | Patients with severe renal impairment (CL cr of <30 mL/min) | Reduce the dosage of Clarithromycin by 50% | Patients with moderate renal impairment (CL cr of 30 to 60 mL/min) taking concomitant atazanavir or ritonavir-containing regimens | Reduce the dosage of Clarithromycin by 50% | Patients with severe renal impairment (CL cr of <30 mL/min) taking concomitant atazanavir or ritonavir-containing regimens | Reduce the dosage of Clarithromycin by 75%".
- KF, Claritromycine. https://www.kinderformularium.nl/geneesmiddel/311/claritromycine
  "GFR 50-80 ml/min/1.73 m2 | Geen dosisaanpassing nodig | GFR 30-50 ml/min/1.73 m2 | Geen dosisaanpassing nodig | GFR 10-30 ml/min/1.73 m2 | 100 % van normale keerdosis en interval tussen twee doseringen: 24 uur" … "GFR < 10 ml/min/1.73 m2 | Een algemene aanbeveling kan niet gegeven worden." Change log: "20 mei 2020 15:04: Dosis aanpassingen bij nierfunctiestoornissen aangepast op basis van SmPC".
- AEP Pediamécum, Claritromicina, "Septiembre 2024". https://www.aeped.es/comite-medicamentos/pediamecum/claritromicina
  "Insuficiencia renal: si el aclaramiento de creatinina es <30 ml/minuto, ampliar el intervalo a cada 24 horas (la mitad de la dosis total diaria). Otras fuentes específicas recomiendan: si el aclaramiento de creatinina es 10-29 ml/minuto/1,73 m 2 , 4 mg/kg/12 horas, y si el aclaramiento es <10 ml/minuto/1,73 m 2 , 4 mg/kg/24 horas."

Confidence / caveats: High for the single threshold CrCl < 30 → half the daily dose (7.5 mg/kg once daily) with a 14-day limit; EU, UK, US and KF agree. No EMA Article 30 harmonised text for Klacid was found (the EMA referral URL returned 404), so EU evidence is national SmPCs. The "4 mg/kg" alternative is quoted second-hand by AEP (its reference list names Aronoff 2007, which was not opened).

---

## 8. Doxycycline (oral)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| any | No dose adjustment | unspecified | — | CIMA 46554; FDA (no accumulation) |
| ≥ 10 | No adjustment | Children > 3 months | mL/min/1.73 m² | KF |
| < 10 | No general advice | Children > 3 months | mL/min/1.73 m² | KF |

Quotes

- CIMA, Vibracina 10 mg/ml suspensión oral, §4.2 and §4.4, revisión "Julio 2026". https://cima.aemps.es/cima/dochtml/ft/46554/FT_46554.html
  "Doxiciclina no necesita ajuste de dosis en pacientes con alteración de la función renal (ver sección 4.4), a diferencia de lo que ocurre con otras tetraciclinas." and "A diferencia de lo que ocurre con otras tetraciclinas, doxiciclina no necesita ajuste de dosis en pacientes con alteración de la función renal debido a que se elimina por el tracto gastrointestinal."
- FDA, Doxycycline (Chartwell; tablets, capsules, powder for suspension), setid 3e7cf78f-3b48-4614-9fa4-c6d1e0c05a35, effective 2026-10-16. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=3e7cf78f-3b48-4614-9fa4-c6d1e0c05a35
  "Studies to date have indicated that administration of doxycycline at the usual recommended doses does not lead to excessive accumulation of doxycycline in patients with renal impairment." and "Studies have shown no significant difference in serum half-life of doxycycline (range 18–22 hours) in individuals with normal and severely impaired renal function. Hemodialysis does not alter serum half-life."
- KF, Doxycycline. https://www.kinderformularium.nl/geneesmiddel/315/doxycycline — "GFR ≥10 ml/min/1.73m2: aanpassing van de dosering is niet nodig. | GFR <10 ml/min/1.73m2: een algemeen advies kan niet worden gegeven."
- AEP Pediamécum, Doxiciclina, "Diciembre 2020". https://www.aeped.es/comite-medicamentos/pediamecum/doxiciclina — "Insuﬁciencia renal: no es necesario un ajuste de dosis. Controlar la función renal; posible aumento de BUN."
- WHO-MFC 2010, Doxycycline: antibacterial monograph (PDF p. 122) "Renal impairment: Use with caution; avoid excessive doses."; antimalarial monographs (PDF pp. 233 and 240) "Renal impairment: Dose reduction not necessary."

Confidence / caveats: High for "no adjustment". WHO-MFC is internally inconsistent between its two doxycycline monographs.

---

## 9. Benzylpenicillin / penicillin G (IV)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| "severe" only | Adjust | unspecified | — | FDA |
| CrCl > 10, uraemic | Full loading dose, then half the loading dose every 4–5 h | unspecified | mL/min/1.73 m² | FDA; CIMA 33580 |
| CrCl < 10 | Full loading dose, then half the loading dose every 8–10 h | unspecified | mL/min/1.73 m² | FDA; CIMA 33580 |
| 100–60 | 18–60 mg/kg daily in 4–6 single doses | Infants ≥ 1 month and children ≤ 12 years | CrCl "mL/min" (table header) | eMC 13988 |
| 50–10 | 12–36 mg/kg daily in 2–3 single doses | same | mL/min | eMC 13988 |
| < 10 | 6–24 mg/kg daily in 2 single doses | same | mL/min | eMC 13988 |
| 10–50 | Normal dose every 8–12 h | same | mL/min/1.73 m² (text) | eMC 13988 |
| < 10 | Normal dose every 12 h | same | mL/min/1.73 m² | eMC 13988 |
| 30–80 | No adjustment | Children > 3 months | mL/min/1.73 m² | KF |
| < 30 | No general advice; individual assessment | Children > 3 months | mL/min/1.73 m² | KF |
| Severe | "maximum 6 g daily" | not stated (g cap is adult-type) | — | WHO-MFC |

Quotes

- FDA, Pfizerpen (penicillin G potassium), setid efef4846-9497-41af-8dd6-518f86eca7f2, effective 2026-11-04. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=efef4846-9497-41af-8dd6-518f86eca7f2
  "Renal Impairment | Penicillin G is relatively nontoxic, and dosage adjustments are generally required only in cases of severe renal impairment. The recommended dosage regimens are as follows: | Creatinine clearance less than 10 mL/min/1.73 m 2 ; administer a full loading dose (see recommended dosages in the tables above) followed by one-half of the loading dose every 8–10 hours. | Uremic patients with a creatinine clearance greater than 10 mL/min/1.73 m 2 ; administer a full loading dose (see recommended dosages in the tables above) followed by one-half of the loading dose every 4–5 hours. Additional dosage modification should be made in patients with hepatic disease and renal impairment."
- CIMA, Penilevel 1.000.000 U.I., §4.2, revisión "Junio 2025". https://cima.aemps.es/cima/dochtml/ft/33580/FT_33580.html
  "Aclaramiento de creatinina < 10 ml/min/1,73 m2: administrar dosis de carga completa, seguida de media dosis de carga cada 8-10h. | Aclaramiento de creatinina > 10 ml/min/1,73 m2: administrar dosis de carga completa, seguida de media dosis de carga cada 4-5h."
- eMC, Benzylpenicillin Sodium 600 mg powder for solution for injection/infusion, revised 12/01/2025. https://www.medicines.org.uk/emc/product/13988/smpc/print
  "Benzylpenicillin dosage for infants (aged 1 month and older) and children (up to 12 years of age) based on creatinine clearance | Creatinine clearance in mL/min | 100-60 | 50-10 | <10 | Serum creatinine in mg % | 0.8-1.5 | 1.5-8.0 | 15 | Benzylpenicillin (daily dose) | 18-60 mg/kg | 12-36 mg/kg | 6-24 mg/kg | Dosing interval | in 4-6 single doses | in 2-3 single doses | in 2 single doses"
  "Infants (aged 1 month and older) and children (up to 12 years of age): If renal function is moderately-to-severely impaired (glomerular filtration rate = 10–50 mL/minute/1.73 m 2 ), the normal dose is administered every 8 – 12 hours. In very severe cases of impaired renal function or renal failure (glomerular filtration rate <10 mL/minute/1.73 m 2 ), the normal dose is administered every 12 hours" and "Pre-term and newborn infants (up to 4 weeks of age): Benzylpenicillin is not suitable for the treatment of pre-term and newborn infants with impaired renal function."
- KF, Benzylpenicilline. https://www.kinderformularium.nl/geneesmiddel/330/benzylpenicilline
  "GFR 50-80 ml/min/1.73 m2 | Aanpassing niet nodig | GFR 30-50 ml/min/1.73 m2 | Aanpassing niet nodig | GFR 10-30 ml/min/1.73 m2 | een algemeen advies kan niet worden gegeven. Per patiënt moet een afweging worden gemaakt tussen het beoogde effect, de risico's van bijwerkingen bij overdosering en therapiefalen bij onderdosering." (same text for GFR < 10).
- WHO-MFC 2010, Benzylpenicillin, PDF p. 103: "Renal impairment: Severe: maximum 6 g daily; neurotoxicity (high doses may cause convulsions)."

Confidence / caveats: Medium. The sources use three incompatible schemes (FDA/AEMPS loading-dose scheme pivoting at 10; UK paediatric daily-dose table pivoting at 60 and 10; KF no advice below 30). The UK table is in mg (600 mg = 1 million units per that product) and its header says "mL/min" while the accompanying sentence says "mL/minute/1.73 m²". The UK table and sentence are not obviously equivalent to each other (reduced daily dose vs normal dose at longer interval). No EMA harmonised text exists for this drug.

---

## 10. Benzathine benzylpenicillin (IM)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| 100–60 | 100% of normal dose, single administration | Adults, adolescents and children | CrCl, mL/min | CIMA 55835 |
| 50–10 | 75%, single administration | same | mL/min | CIMA 55835 |
| < 10 | 20–50% (max 1–3 million IU/day), in 2–3 administrations | same | mL/min | CIMA 55835 |
| 30–80 | No adjustment | Children > 3 months | mL/min/1.73 m² | KF |
| < 30 | No general advice | Children > 3 months | mL/min/1.73 m² | KF |
| any | No dosing rule; excretion "considerably delayed" | unspecified | — | FDA Bicillin L-A |

Quotes

- CIMA, Benzetacil 1.200.000 UI, §4.2, revisión "Junio 2023". https://cima.aemps.es/cima/dochtml/ft/55835/FT_55835.html
  "En pacientes con insuficiencia renal, deberá ajustarse la dosis en función del grado de severidad de insuficiencia renal. | Dosis para adultos, adolescentes y niños según el aclaramiento de creatinina | Aclaramiento creatinina en ml/min | 100-60 | 50-10 | <10 | Creatinina sérica en mg % | 0,8-1,5 | 1,5-8 | 15 | Proporción de la dosis diaria normal de Benzetacil | 100% | 75% | 20-50% (1-3 mill de UI/ día máximo | Intervalo de dosis | En 1 única administración | En 1 única administración | En 2-3 únicas administraciones". Dialysis: "No hay datos disponibles sobre la influencia de la diálisis en los niveles plasmáticos de bencil penicilina. Por lo tanto, la decisión de tratar a los pacientes en diálisis con Benzetacil debe tomarse caso por caso."
- KF, Benzathinebenzylpenicilline. https://www.kinderformularium.nl/geneesmiddel/341/benzathinebenzylpenicilline
  "GFR 50-80 ml/min/1.73 m2 | Aanpassing niet nodig | GFR 30-50 ml/min/1.73 m2 | Aanpassing niet nodig | GFR 10-30 ml/min/1.73 m2 | een algemeen advies kan niet worden gegeven. …" (same for < 10).
- FDA, Bicillin L-A, setid 012d46f1-d0a0-4676-a879-cd320297ab16, effective 2026-07-21. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=012d46f1-d0a0-4676-a879-cd320297ab16
  "With normal kidney function, the drug is excreted rapidly by tubular excretion. In neonates and young infants and in individuals with impaired kidney function, excretion is considerably delayed." Geriatric use: "This drug is known to be substantially excreted by the kidney, and the risk of toxic reactions to this drug may be greater in patients with impaired renal function." No dosing table.
- WHO-MFC 2010, Benzathine benzylpenicillin, PDF p. 102: "Renal impairment: Severe: neurotoxicity; high doses may cause convulsions."

Confidence / caveats: Low-medium. Only one source (the Spanish SmPC) gives numbers, and its last column ("daily dose", "2–3 administrations", "1–3 mill UI/día") is hard to apply to a single-shot depot product; it looks like text written for a daily-dosed penicillin. FDA gives no adjustment and KF declines to give advice below GFR 30. Treat as "no validated paediatric rule".

---

## 11. Ciprofloxacin (oral and IV)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| > 60 | Usual dose | Adult-type mg doses; children "not studied" | mL/min/1.73 m² | EMA-A30; CIMA |
| 30–60 | Oral 250–500 mg q12h; IV 200–400 mg q12h | adult-type | mL/min/1.73 m² | EMA-A30; CIMA |
| < 30 | Oral 250–500 mg q24h; IV 200–400 mg q24h | adult-type | mL/min/1.73 m² | EMA-A30; CIMA |
| > 50 | Usual dose | Adults | mL/min | FDA oral |
| 30–50 | 250–500 mg q12h | Adults | mL/min | FDA oral |
| 5–29 | Oral 250–500 mg q18h; IV 200–400 mg q18–24h | Adults | mL/min | FDA |
| HD / PD | 250–500 mg q24h after dialysis | Adults | — | FDA; EMA |
| < 50 | "No information is available" | Children | mL/min/1.73 m² | FDA |
| 10–30 | 100% single dose, interval 24 h | Children > 3 months | mL/min/1.73 m² | KF |
| < 10 | 100% single dose, interval 24 h | Children > 3 months | mL/min/1.73 m² | KF |
| 10–29; < 10 | q24h; q24h (IV) | Children | mL/min/1.73 m² | LPCH |
| 10–50 / < 10 | 7.5 mg/kg q12h / 5 mg/kg q12h | "datos en adultos" | mL/min | AEP |

Quotes

- EMA-A30, Ciprofloxacin Bayer Annexes I–III, SmPC §4.2 (CHMP opinion 24/07/2008, EC decision 07/10/2008 per the EMA referral page). https://www.ema.europa.eu/en/documents/referral/ciprofloxacin-bayer-article-30-referral-annex-i-ii-iii_en.pdf
  Oral: "Creatinine Clearance [mL/min/1.73 m²] | Serum Creatinine [µmol/L] | Oral Dose [mg] | > 60 | < 124 | See Usual Dosage. | 30-60 | 124 to 168 | 250-500 mg every 12 h | < 30 | > 169 | 250-500 mg every 24 h | Patients on haemodialysis | > 169 | 250-500 mg every 24 h (after dialysis) | Patients on peritoneal dialysis | > 169 | 250-500 mg every 24 h". IV: "> 60 | < 124 | See Usual Dosage. | 30-60 | 124 to 168 | 200-400 mg every 12 h | < 30 | > 169 | 200-400 mg every 24 h". Both: "Dosing in children with impaired renal and/or hepatic function has not been studied."
- CIMA, Ciprofloxacino Normon 250 mg comprimidos (revisión "Diciembre 2024") https://cima.aemps.es/cima/dochtml/ft/62299/FT_62299.html and Ciprofloxacino Normon 2 mg/ml solución para perfusión (revisión "Septiembre 2026") https://cima.aemps.es/cima/dochtml/ft/62646/FT_62646.html — same tables; "No se ha estudiado la dosificación en niños con insuficiencia de la función renal y/o hepática."
- FDA, Cipro tablets/oral suspension (Bayer), setid 888dc7f9-ad9c-4c00-8d50-8ddfd9bd27c0, effective 2026-03-02. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=888dc7f9-ad9c-4c00-8d50-8ddfd9bd27c0
  "Table 4: Recommended Starting and Maintenance Doses for Adult Patients with Impaired Renal Function | Creatinine Clearance (mL/min) | Dose | > 50 | See Usual Dosage. | 30–50 | 250–500 mg every 12 hours | 5–29 | 250–500 mg every 18 hours | Patients on hemodialysis or Peritoneal dialysis | 250–500 mg every 24 hours (after dialysis)". Paediatric: "Pediatric patients with moderate to severe renal insufficiency were excluded from the clinical trial of cUTI and pyelonephritis. No information is available on dosing adjustments necessary for pediatric patients with moderate to severe renal insufficiency (that is, creatinine clearance of < 50 mL/min/1.73m 2 )."
- FDA, Ciprofloxacin injection (Sagent), setid f406e796-17d9-4465-b8a7-00d966a4ba74, effective 2026-08-04. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=f406e796-17d9-4465-b8a7-00d966a4ba74 — "Creatinine Clearance (mL/min) | Dose | >30| See Usual Dosage.| 5 to 29| 200 to 400 mg every 18 to 24 hours"; same paediatric "No information is available" sentence.
- KF, Ciprofloxacine. https://www.kinderformularium.nl/geneesmiddel/310/ciprofloxacine
  "GFR 50-80 ml/min/1.73 m2 | Aanpassing niet nodig. | GFR 30-50 ml/min/1.73 m2 | Aanpassing niet nodig. | GFR 10-30 ml/min/1.73 m2 | 100 % van normale keerdosis en interval tussen twee doseringen: 24 uur | GFR < 10 ml/min/1.73 m2 | 100 % van normale keerdosis en interval tussen twee doseringen: 24 uur". Change log: "04 januari 2021 14:35: Het advies bij nierfunctiestoornissen is opnieuw beoordeeld en aangepast."
- LPCH 2024 (IV): "Ciprofloxacin 10-15mg/kg q12h or 10mg/kg q8h CKRT: 10-15 mg/kg q12h | CrCl 10-29: q24h | q24h | q24h | 400 mg".
- AEP Pediamécum, Ciprofloxacino, "Octubre 2020". https://www.aeped.es/comite-medicamentos/pediamecum/ciprofloxacino
  "Insuficiencia renal (datos en adultos): no es preciso modificar las dosis cuando el aclaramiento de creatinina es mayor a 50 ml/min, entre 10-50 ml/min: 7,5 mg/kg/12 horas, <10 ml/min: 5 mg/kg/12 horas."
- WHO-MFC 2010, Ciprofloxacin, PDF p. 119–120: "Renal impairment: Reduce dose if CrCl < 30 ml/minute."

Confidence / caveats: Medium. Both regulators state that paediatric renal dosing has not been studied; the mg tables are adult. The two paediatric sources (KF, LPCH) agree on "same dose every 24 h below 30". EMA and FDA disagree on bands (EMA starts at ≤ 60 and gives q24h below 30; FDA starts at ≤ 50 and gives q18h for 5–29). AEP uses a dose-reduction scheme with q12h retained, which differs from every other source.

---

## 12. Metronidazole (oral)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| any | No dose reduction needed | unspecified | — | eMC 101002; WHO-MFC ("not usually necessary") |
| ESRD | No specific reduction; monitor for adverse events (metabolite accumulation) | unspecified | — | FDA |
| Haemodialysis | Re-dose / supplement after session | unspecified | — | FDA; eMC |
| ≥ 10 | No adjustment | Children > 3 months | mL/min/1.73 m² | KF |
| < 10 | No general advice | Children > 3 months | mL/min/1.73 m² | KF |
| < 10, not on dialysis | Give 50% of the dose | children's formulary, but "No existen recomendaciones específicas en pacientes pediátricos" | mL/min | AEP |
| unspecified | "Puede recomendarse la reducción de la dosis" (no number) | unspecified | — | CIMA 47656 |

Quotes

- FDA, Flagyl capsules (Pfizer), setid a2883ca1-5a9a-4259-9d80-46ab67274384, effective 2026-05-22. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=a2883ca1-5a9a-4259-9d80-46ab67274384
  "Decreased renal function does not alter the single-dose pharmacokinetics of metronidazole." Precautions: "Patients with end-stage renal disease may excrete metronidazole and metabolites slowly in the urine, resulting in significant accumulation of metronidazole metabolites. Monitoring for metronidazole associated adverse events is recommended". Dosage: "Hemodialysis removes significant amounts of metronidazole and its metabolites from systemic circulation. … If the administration of metronidazole cannot be separated from a hemodialysis session, supplementation of metronidazole dosage following the hemodialysis session should be considered, depending on the patient's clinical situation".
- eMC, Metronidazole 200 mg/5 ml Oral Suspension, revised 20/06/2023. https://www.medicines.org.uk/emc/product/101002/smpc/print
  "The elimination half-life of metronidazole remains unchanged in the presence of renal failure. The dosage of metronidazole therefore needs no reduction. Such patients however, retain the metabolites of metronidazole. The clinical significance of this is not known at present." and "Metronidazole should therefore, be re-administered immediately after haemodialysis." and "No routine adjustment in the dosage of Metronidazole need be made in patients with renal failure undergoing intermittent peritoneal dialysis (IPD) or continuous ambulatory peritoneal dialysis (CAPD)."
- CIMA, Flagyl 125 mg/5 ml suspensión oral, revisión "Junio 2025". https://cima.aemps.es/cima/dochtml/ft/47656/FT_47656.html
  "Puede recomendarse la reducción de la dosis de metronidazol en pacientes con insuficiencia renal, cuando no estén sometidos a diálisis y la monitorización de los niveles séricos de los metabolitos".
- KF, Metronidazol. https://www.kinderformularium.nl/geneesmiddel/100/metronidazol — "GFR ≥10 ml/min/1.73m2: aanpassing van de dosering is niet nodig. | GFR <10 ml/min/1.73m2: een algemeen advies kan niet worden gegeven."
- AEP Pediamécum, Metronidazol, "Diciembre 2020". https://www.aeped.es/comite-medicamentos/pediamecum/metronidazol
  "No existen recomendaciones específicas en pacientes pediátricos con insuficiencia hepática y/o renal." and "…puede recomendarse la reducción de la dosis de metronidazol en pacientes renales (administrar 50% de la dosis cuando el aclaramiento sea <10 ml/min), cuando no estén sometidos a diálisis."
- WHO-MFC 2010, Metronidazole, PDF p. 127: "Renal impairment: Metabolites may accumulate in severe impairment possibly causing adverse effects. Dose adjustment is not usually necessary."
- LPCH 2024 (IV): "Metronidazole 10 mg/kg q8h … No adjustment necessary … No adjustment per manufacturer. Consider alternative dosing or antibiotic if >1-2 weeks".

Confidence / caveats: High for "no adjustment at GFR ≥ 10". Below 10 the sources split: UK/FDA/WHO no reduction, AEP 50%, KF no advice, Spanish SmPC "may be recommended" without a number.

---

## 13. Vancomycin, ORAL, for C. difficile

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| any | Dose adjustment unlikely (very low absorption), unless substantial absorption from inflamed bowel | ≥ 12 years (capsules) | — | EMA-A31 §4.2 |
| ≥ 10 | No adjustment | Children > 3 months | mL/min/1.73 m² | KF (oral monograph) |
| < 10 | No general advice | Children > 3 months | mL/min/1.73 m² | KF |
| any | No dose rule; monitor serum concentrations in renal insufficiency and/or colitis | Adults and children < 18 years | — | FDA Vancocin, Firvanq |

Quotes

- EMA-A31, Vancomycin Annex III, capsule SmPC §4.2 (PDF created 17 Oct 2017). https://www.ema.europa.eu/en/documents/referral/vancomycin-article-31-referral-annex-iii_en.pdf
  "Renal impairment | Due to the very low systemic absorption, dose adjustment is unlikely, unless substantial oral absorption may occur in case of inflammatory intestinal disorders or Clostridium difficile-induced pseudomembranous colitis (see section 4.4)."
- CIMA, Vancomicina Normon 500 mg polvo (IV product authorised for oral use), revisión "Enero 2026". https://cima.aemps.es/cima/dochtml/ft/85652/FT_85652.html
  Oral dose: "Recién nacidos, lactantes y niños menores de 12 años | La dosis recomendada de vancomicina es de 10 mg/kg por vía oral cada 6 horas durante 10 días. La dosis diaria máxima no debe exceder de 2 g." §4.4: "La absorción puede estar aumentada en pacientes con trastornos inflamatorios de la mucosa intestinal o con colitis pseudomembranosa inducida por Clostridioides difficile. Estos pacientes pueden estar en riesgo para el desarrollo de reacciones adversas, especialmente si presentan una insuficiencia renal concomitante." This SmPC has no renal adjustment statement for the oral route; its GFR-based rules are for IV use.
- FDA, Vancocin capsules, setid a078d9c2-f89c-4f9f-8ded-60ffb2983c3f, effective 2025-12-22. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=a078d9c2-f89c-4f9f-8ded-60ffb2983c3f
  "…monitoring of serum concentrations of vancomycin may be appropriate in some instances, e.g., in patients with renal insufficiency and/or colitis or in those receiving concomitant therapy with an aminoglycoside antibiotic." Same wording in Firvanq oral solution, setid 5aca5508-b577-446c-9980-ab4c7582b4b9, effective 2026-07-14: https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=5aca5508-b577-446c-9980-ab4c7582b4b9
- KF, Vancomycine oraal. https://www.kinderformularium.nl/geneesmiddel/15825/vancomycine-oraal — "GFR ≥10 ml/min/1.73m2: aanpassing van de dosering is niet nodig. | GFR <10 ml/min/1.73m2: een algemeen advies kan niet worden gegeven."

Confidence / caveats: High for "no routine adjustment for the oral route". Do NOT import interval rules from IV-vancomycin texts: AEP Pediamécum (vancomicina, Mar 2022) "cuando el aclaramiento es de 25-50 ml/min, el intervalo de dosiﬁcación debe ser 24 horas; si el aclaramiento de creatinina es ≤25 ml/min el intervalo interdosis debe ser de 48 horas o mayor" and WHO-MFC "Increase dose interval or reduce dose, or both, in renal impairment" are written for systemic (IV) therapy.

---

## 14. Trimethoprim–sulfamethoxazole (co-trimoxazole)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| > 30 | Usual regimen | unspecified (US); adults and > 12 years (ES, UK) | CrCl, mL/min | FDA; CIMA 48671; eMC 6994 |
| 15–30 | Half the usual regimen | same | mL/min | FDA; CIMA; eMC |
| < 15 | Use not recommended | same | mL/min | FDA; CIMA; eMC |
| ≤ 12 years | "No information is available" | Children ≤ 12 years | — | CIMA; eMC |
| > 30 | No adjustment | Children | mL/min/1.73 m² | AEP (2026) |
| 15–30 | 50% of usual daily dose (see quote for interval per regimen) | Children | mL/min/1.73 m² | AEP |
| < 15 | Not recommended; if used 25–50% of usual daily dose | Children | mL/min/1.73 m² | AEP |
| 10–30 (treatment) | 100% single dose, interval 24 h | Children > 3 months | mL/min/1.73 m² | KF |
| 10–30 (UTI/RTI prophylaxis) | 50% single dose, interval 24 h | Children > 3 months | mL/min/1.73 m² | KF |
| 10–30 (PJP prophylaxis) | No adjustment | Children > 3 months | mL/min/1.73 m² | KF |
| < 10 | Contraindicated | Children > 3 months | mL/min/1.73 m² | KF |
| 15–30; < 10 | decrease dose 50%; not recommended (IV) | Children | mL/min/1.73 m² | LPCH |

Quotes

- FDA, Bactrim / Bactrim DS (Sun), setid f59d0c04-9c66-4d53-a0e1-cb55570deb62, effective 2024-12-30. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=f59d0c04-9c66-4d53-a0e1-cb55570deb62
  "For Patients with Impaired Renal Function | When renal function is impaired, a reduced dosage should be employed using the following table: | Creatinine Clearance (mL/min) | Recommended Dosage Regimen | Above 30 | Usual standard regimen | 15–30 | ½ the usual regimen | Below 15 | Use not recommended". The same three rows ("Above 30", "15–30", "Below 15") are present in the oral suspension label, setid 5ff03694-787a-409d-a95e-8422aa27499c: https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=5ff03694-787a-409d-a95e-8422aa27499c
- CIMA, Septrin Pediátrico 8 mg/40 mg/ml suspensión oral, §4.2, revisión "Julio 2026". https://cima.aemps.es/cima/dochtml/ft/48671/FT_48671.html
  "Pacientes con insuficiencia renal : | Adultos y niños mayores de 12 años (no se dispone de información para niños menores de 12 años): | Aclaramiento de creatinina (ml/min) Dosis recomendada | > 30 Dosis estándar | 15-30 La mitad de la dosis estándar | < 15 No se recomienda".
- eMC, Co-Trimoxazole 40 mg/200 mg per 5 ml Paediatric Suspension, revised September 2026. https://www.medicines.org.uk/emc/product/6994/smpc/print
  "Adults and children over 12 years: | Creatinine Clearance (ml/min) | Recommended Dosage | >30 | 10 ml every 12 hours | 15 to 30 | 5 ml every 12 hours | <15 | Not recommended | No information is available for children aged 12 years and under with renal failure."
- AEP Pediamécum, Cotrimoxazol, "Septiembre 2026". https://www.aeped.es/comite-medicamentos/pediamecum/cotrimoxazol
  Column headers: "Si la dosis habitual es de 5 a 12 mg trimetoprim/kg/día, cada 12 o cada 24 horas | Si la dosis habitual es de 15 a 20 mg trimetoprim/kg/día, cada 6 o cada 8 horas | Si la dosis habitual es de 80 a 160 mg trimetoprim /dosis , 3 dosis/semana". Rows: ">30 mL/minuto/1,73 m 2 | No es necesario ajustar la dosis." (×3); "De 15 a 30 mL/minuto/1,73 m 2 | Usar con precaución y monitorización adecuada. Administrar el 50% de la dosis diaria habitual cada 12 o cada 24 horas. | Usar con precaución y monitorización adecuada. Administrar el 50% de la dosis diaria habitual cada 8 o cada 12 horas. | Usar con precaución y monitorización adecuada. 80 mg trimetoprim cada 24 horas o 3 dosis/semana."; "<15 mL/minuto/1,73 m 2 | Uso no recomendado; si se usa, vigilar estrechamente. Administrar el 25 o el 50% de la dosis diaria habitual cada 12 o cada 24 horas. | (same) | Uso no recomendado; si se usa, vigilar estrechamente. 80 mg trimetoprim, 3 dosis/semana." Stated basis: "Datos basados en estudios farmacocinéticos, estudios pediátricos (limitados), estudios en adultos y opinión de expertos." and "Adaptado de: UpToDate (Pediatric drug information)".
- KF, Cotrimoxazol. https://www.kinderformularium.nl/geneesmiddel/314/cotrimoxazol
  "Infecties, infecties bij CF (normale tot zeer hoge dosering), behandeling Pneumocystis Jiroveci Pneumonie: | GFR 50-80 ml/min/1.73 m2: Geen aanpassing nodig | GFR 30-50 ml/min/1.73 m2: Geen aanpassing nodig | GFR 10-30 ml/min/1.73 m2: 100% van normale keerdosis en interval tussen twee doseringen: 24 uur | GFR < 10 ml/min/1.73 m2 : Gecontraïndiceerd". "Profylaxe van recidiverende urineweginfecties/luchtweginfecties: … GFR 10-30 ml/min/1.73 m2: 50% van normale keerdosis en interval tussen twee doseringen: 24 uur | GFR < 10 ml/min/1.73 m2: Gecontraïndiceerd". "Profylaxe Pneumocystis Jiroveci Pneumonie: … GFR 10-30 ml/min/1.73 m2: Geen aanpassing nodig | GFR < 10 ml/min/1.73 m2: Gecontraïndiceerd".
- LPCH 2024 (IV): "Sulfamethoxazole/ trimethoprim (TMP) 3-6mg/kg TMP q12h Serious infection/PJP: 5mg/kg TMP q6-8h | CrCl 15-30: decrease dose 50% | Not recommended | Not recommended | 320 mg TMP".
- WHO-MFC 2010, Sulfamethoxazole + trimethoprim, PDF p. 129–130: "Renal impairment: Severe impairment: avoid use. Moderate impairment: use half normal dose. Plasma monitoring may be required with high doses in renal impairment; seek expert advice."

Confidence / caveats: High for the shape (> 30 normal, 15–30 half, < 15 avoid); every source except KF uses these bands. Two caveats: (a) the EU/UK SmPCs explicitly say the table is for patients over 12 years and that no information exists for younger children; the paediatric application rests on AEP (adapted from UpToDate, not opened directly), LPCH and WHO-MFC. (b) KF places the lower cut-off at 10, not 15, and treats 10–30 as one band.

---

## 15. Clindamycin (oral)

| Band | Change | Population | Units | Source |
|---|---|---|---|---|
| any | No dose modification | unspecified | — | FDA; CIMA 49152; LPCH |
| ≥ 10 | No adjustment | Children > 3 months | mL/min/1.73 m² | KF |
| < 10 | No general advice | Children > 3 months | mL/min/1.73 m² | KF |
| Severe | "reduce dose" (no number) | Children | — | WHO-MFC |

Quotes

- FDA, Cleocin Pediatric (clindamycin palmitate), setid 1ef87b6c-9d32-44ac-8c55-8bf01424cc45, effective 2026-06-01. https://dailymed.nlm.nih.gov/dailymed/drugInfo.cfm?setid=1ef87b6c-9d32-44ac-8c55-8bf01424cc45
  "The elimination half-life of clindamycin is increased slightly in patients with markedly reduced renal or hepatic function. Hemodialysis and peritoneal dialysis are not effective in removing clindamycin from the serum. Dosage schedules do not need to be modified in patients with renal or hepatic disease." and "Clindamycin dosage modification is not necessary in patients with renal disease." Cleocin HCl capsules, setid df9a2a41-b132-4f43-8940-b2d773b1369a (effective 2026-09-16): "Dosage schedules do not need to be modified in patients with renal disease." and "Clindamycin dosage modification is not necessary in patients with renal disease."
- CIMA, Dalacin 150 mg cápsulas duras, §4.2, revisión "Julio 2026". https://cima.aemps.es/cima/dochtml/ft/49152/FT_49152.html
  "Pacientes con insuficiencia renal | En pacientes con insuficiencia renal no se precisa ajuste de dosis." §5.2: "No hay que adaptar las dosis en pacientes con alteraciones de la función renal moderadas o de moderadas a graves."
- KF, Clindamycine. https://www.kinderformularium.nl/geneesmiddel/312/clindamycine — "GFR ≥10 ml/min/1.73m2: aanpassing van de dosering is niet nodig. | GFR <10 ml/min/1.73m2: een algemeen advies kan niet worden gegeven."
- LPCH 2024 (IV): "Clindamycin 7-13mg/kg q8h No adjustment necessary 900 mg".
- AEP Pediamécum, Clindamicina, "Noviembre 2020". https://www.aeped.es/comite-medicamentos/pediamecum/clindamicina — "En general no se requiere ajuste de dosis excepto en casos de deterioro grave de la función renal o hepática."
- WHO-MFC 2010, Clindamycin, PDF p. 133: "Renal impairment: Severe impairment: reduce dose."

Confidence / caveats: High for "no adjustment". WHO-MFC 2010 and AEP mention reduction in severe impairment without a number; the current regulatory texts say none is needed.

---

## 16. Disagreements between sources (consolidated)

1. **Amoxicillin, children < 40 kg.** EMA/AEMPS: 15 mg/kg twice daily at GFR 10–30, once daily below 10. Kinderformularium (since 5 Feb 2026): no adjustment at GFR ≥ 10. FDA: no paediatric < 40 kg rule ("no dosing recommendations for pediatric patients with impaired renal function"). AEP Pediamécum words the EMA rule as "15 mg/kg/día en dos dosis", which is half the EMA dose if read literally.
2. **Amoxicillin, FDA label internal error.** Highlights say "severe renal impairment (GFR greater than 30 mL/min)"; body says "less than 30".
3. **Amoxicillin–clavulanate.** EMA: paediatric adjusted dose exists only for 4:1; 7:1, 8:1, 14:1 "not recommended" below CrCl 30; 2:1 not recommended in children < 40 kg below 30. Kinderformularium: full single dose q12h (10–30) / q24h (< 10) with no ratio restriction. FDA: mg-only table; ES-600 (14:1) label has no renal section. AEP uses "< 12 años" where EMA uses "< 40 kg".
4. **Cefuroxime sodium.** Regulatory texts adjust from CrCl ≤ 20; Kinderformularium from GFR < 30. Regulatory doses are adult mg.
5. **Cefalexin.** Start of adjustment: FDA 30–59 (cap 1 g/day) and < 30 (fixed 250 mg doses, ≥ 15 years only); Kinderformularium < 50; AEP < 50; UK "markedly impaired"; Spain < 10 (max 500 mg/day).
6. **Clarithromycin.** Mainstream: < 30 → half dose. AEP also quotes an alternative (10–29: 4 mg/kg q12h; < 10: 4 mg/kg q24h). FDA adds protease-inhibitor rules (50% at 30–60; 75% below 30).
7. **Doxycycline.** WHO-MFC antibacterial monograph "Use with caution; avoid excessive doses" vs its antimalarial monograph and all others "no adjustment".
8. **Benzylpenicillin.** FDA/AEMPS loading-dose scheme pivoting at CrCl 10; UK paediatric table pivoting at 60 and 10 (and its own sentence gives a different, interval-only rule); Kinderformularium no advice below 30; WHO "maximum 6 g daily".
9. **Benzathine benzylpenicillin.** AEMPS gives a percentage table; FDA gives nothing; Kinderformularium no advice below 30.
10. **Ciprofloxacin.** EMA bands > 60 / 30–60 / < 30 (per 1.73 m²; q12h then q24h); FDA bands > 50 / 30–50 / 5–29 (mL/min; q12h then q18h). AEP reduces the mg/kg dose and keeps q12h; Kinderformularium and LPCH keep the dose and go to q24h below 30. Both regulators: paediatric renal dosing not studied.
11. **Metronidazole below GFR 10.** UK/FDA/WHO: no reduction; AEP: 50%; Kinderformularium: no advice.
12. **Co-trimoxazole.** Lower cut-off 15 (FDA, EU, UK, AEP, LPCH) vs 10 (Kinderformularium). EU/UK state no information for children ≤ 12 years.
13. **Clindamycin.** FDA/AEMPS/LPCH: none. WHO-MFC 2010 and AEP: reduce in severe impairment.
14. **Units.** EMA amoxicillin, amoxicillin–clavulanate, ceftriaxone, FDA tables and Spanish clarithromycin/co-trimoxazole use absolute mL/min; EMA cefuroxime and ciprofloxacin, FDA penicillin G, UK clarithromycin paediatric, Kinderformularium, AEP co-trimoxazole and LPCH use mL/min/1.73 m².

## 17. Not verified (could not be opened; not used for any number)

- **BNF for Children** (bnfc.nice.org.uk): HTTP 403 from this location on both direct download and the fetch tool. Not verified.
- **NICE** guidance: not consulted (BNFc is the NICE-hosted source and was blocked).
- **Daschner M. Drug dosage in children with reduced renal function. Pediatr Nephrol 2005;20:1675–1686** (PMID 16133064): only the abstract was readable (Europe PMC; not open access; Springer returned a login redirect). Its dosing tables are not verified.
- **Aronoff GR et al., Drug Prescribing in Renal Failure, 5th ed. 2007**, including its online paediatric table (kdpnet.kdp.louisville.edu): connection refused. Only cited second-hand by AEP and LPCH.
- **UpToDate / Lexicomp paediatric drug information**: not accessible; the AEP co-trimoxazole table is an adaptation of it.
- **KDIGO**: no paediatric antibiotic dosing table was located; not used.
- **EMA Article 30 referral for Klacid (clarithromycin)**: the EMA referral page returned 404; I could not confirm that such a harmonised text exists. Clarithromycin EU evidence is therefore national SmPCs (Spain, UK).
- **CIMA ficha técnica for Cefalexina Normon 500 mg (nº 54302)**: 404; Kefloridina Forte (nº 51115) was used instead.
- **EC decision dates of the EMA Article 30 referrals** other than ciprofloxacin: not captured; the dates given for those documents are PDF creation dates.
- **AEP Pediamécum pages for penicillin G, benzathine penicillin and cefuroxime axetil**: page addresses not found (cefuroxime page opened but has no renal rule).
- Table-layout caveat: quotes from tables (EMA PDFs, CIMA/eMC HTML, Benzetacil, UK benzylpenicillin, LPCH) were reconstructed from linearised text; cell-to-column assignment was inferred from order. The LPCH and Benzetacil tables are the most exposed to this.

## 18. Summary table

| Drug | Adjustment needed? | Bands (paediatric where available) | Best source | Verified |
|---|---|---|---|---|
| Amoxicillin (oral) | Only severe (GFR < 30) per EMA; none ≥ 10 per KF | EMA: > 30 none; 10–30: 15 mg/kg BID (max 500 mg BID); < 10: 15 mg/kg once daily (max 500 mg); children < 40 kg; mL/min | EMA Art. 30 Amoxil SmPC §4.2 (= AEMPS) | Yes |
| Amoxicillin–clavulanate | Only severe (CrCl < 30), ratio-dependent | 4:1: 10–30: 15/3.75 mg/kg BID; < 10: 15/3.75 mg/kg once daily (max 500/125). 7:1, 8:1, 14:1: avoid below 30. 2:1: avoid in < 40 kg below 30 | EMA Art. 30 Augmentin SmPC §4.2 (= AEMPS) | Yes |
| Ceftriaxone (IM/IV) | No, except cap in severe | CrCl < 10: max 2 g/day (WHO/AEP: 50 mg/kg/day, max 2 g); no post-dialysis dose | EMA Art. 30 Rocephin §4.2; KF | Yes |
| Cefuroxime axetil (oral) | Yes, CrCl < 30 | ≥ 30 none; 10–29: usual single dose q24h; < 10: q48h; mL/min/1.73 m² | EMA Art. 30 Zinnat §4.2 (= AEMPS, FDA, KF) | Yes |
| Cefuroxime sodium (IV) | Yes, CrCl ≤ 20 (KF: < 30) | Adult mg: 10–20: 750 mg BID; < 10: 750 mg once daily. Paediatric: KF 10–30 q12h; < 10 q24h | EMA Art. 30 Zinacef §4.2 + KF for children | Yes (paediatric rule = KF/FDA sentence only) |
| Cefalexin | Yes, but no paediatric regulatory rule | KF: 30–50 and 10–30: single dose q12h; < 10: q24h. FDA (≥ 15 y): fixed 250 mg schedule below 30 | Kinderformularium; FDA for ≥ 15 y | Yes (sources disagree on threshold) |
| Azithromycin | No (caution GFR < 10) | ≥ 10 none; < 10 caution, no dose given | AEMPS Zitromax §4.2; UK SmPC; KF | Yes |
| Clarithromycin | Only severe (CrCl < 30) | < 30: half dose, 7.5 mg/kg once daily, max 14 days; < 10: no advice (KF) | AEMPS paediatric suspension §4.2; UK paediatric SmPC | Yes |
| Doxycycline | No | none (KF: no advice < 10) | AEMPS Vibracina §4.2; FDA | Yes |
| Benzylpenicillin (IV) | Only severe; schemes differ | FDA/AEMPS: CrCl < 10: loading dose then half q8–10h; > 10 uraemic: half q4–5h. UK children: 50–10: 12–36 mg/kg/day in 2–3 doses; < 10: 6–24 mg/kg/day in 2 doses | FDA Pfizerpen / AEMPS Penilevel; UK SmPC for children | Yes (no consensus) |
| Benzathine benzylpenicillin (IM) | Unclear | AEMPS: 60–100: 100%; 10–50: 75%; < 10: 20–50%. FDA none; KF no advice < 30 | AEMPS Benzetacil §4.2 (single source) | Yes, but low confidence |
| Ciprofloxacin (oral/IV) | Yes, CrCl < 30 in children's sources | KF/LPCH: 10–30 and < 10: usual single dose q24h. EMA adult: 30–60 q12h (250–500 mg oral / 200–400 mg IV); < 30 q24h | Kinderformularium for children; EMA Art. 30 for bands | Yes (regulators: paediatric "not studied") |
| Metronidazole | No (uncertain < 10) | ≥ 10 none; < 10: none (UK/FDA) vs 50% (AEP); re-dose after haemodialysis | UK SmPC; FDA; KF | Yes |
| Vancomycin, oral (C. difficile) | No | none; monitor serum level if colitis + renal impairment | EMA Art. 31 vancomycin §4.2; KF oral | Yes |
| Co-trimoxazole | Yes | > 30 usual; 15–30 half; < 15 avoid (KF: < 10 contraindicated) | FDA Bactrim; AEMPS Septrin (> 12 y); AEP 2026 for children | Yes (EU: no data ≤ 12 y) |
| Clindamycin | No | none (KF: no advice < 10) | FDA Cleocin Pediatric; AEMPS Dalacin §4.2 | Yes |
