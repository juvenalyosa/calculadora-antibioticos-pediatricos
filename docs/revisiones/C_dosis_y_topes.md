# C — Doses and maximums: sources that fill the gaps in the knowledge base

Search date: 2026-11-17. Scope: literature review only, no code.

## How to read the "verified" column

| Code | Meaning |
|---|---|
| **RAW** | I downloaded the page or PDF and read the extracted text myself. Quotes are copied from that text. |
| **TOOL** | The site refused direct download (HTTP 403). The page was read through the fetch tool, whose reader returned short exact excerpts. I did not see the raw page. Numbers were identical in two separate passes. Treat as verified with that reservation. |
| **NO** | Could not open the document. Nothing is reported from memory. |

Every quote is in its original language. "Max" fields say "not stated" when the source gives no figure; nothing has been inferred unless explicitly labelled "inference".

Sources opened (short names used below):

| Short name | Document | URL | Verified |
|---|---|---|---|
| IDSA-GAS-2012 | Shulman ST et al. Clinical Practice Guideline for the Diagnosis and Management of Group A Streptococcal Pharyngitis: 2012 Update by the IDSA. Clin Infect Dis 2012;55(10):e86–e102 | https://pmc.ncbi.nlm.nih.gov/articles/PMC7108032/ | RAW |
| IDSA-GAS-2025 | IDSA Clinical Practice Guideline Update on GAS Pharyngitis, Part 1 (risk assessment). Published 14 Oct 2025; Clin Infect Dis 2026;82(Suppl 3):i97–i104 | https://www.idsociety.org/practice-guideline/streptococcal-pharyngitis2/ | RAW |
| CDC-GAS | CDC, Clinical Guidance for Group A Streptococcal Pharyngitis (page date shown: Nov. 18, 2025) | https://www.cdc.gov/group-a-strep/hcp/clinical-guidance/strep-throat.html | TOOL |
| NICE-NG84 | NICE guideline NG84, Sore throat (acute): antimicrobial prescribing. Published 26 January 2018. Section 1.3, Tables 1–2 | https://www.nice.org.uk/guidance/ng84/chapter/Recommendations | RAW |
| AAP-AOM-2013 | Lieberthal AS et al. The Diagnosis and Management of Acute Otitis Media. Pediatrics 2013;131(3):e964–e999. Table 5 (p. e983), "Duration of Therapy" (p. e984–e985) | https://www.aepap.org/sites/default/files/oma_aap_2013.pdf (copy of the journal PDF hosted by a paediatric society; the publisher page https://publications.aap.org/pediatrics/article/131/3/e964/30912 returned 403) | RAW (mirror copy) |
| PIDS/IDSA-CAP-2011 | Bradley JS et al. The Management of Community-Acquired Pneumonia in Infants and Children Older Than 3 Months of Age. Clin Infect Dis 2011;53(7):e25–e76. Tables 5 and 7 | https://pmc.ncbi.nlm.nih.gov/articles/PMC7107838/ | RAW |
| IDSA/SHEA-CDI-2017 | McDonald LC et al. Clinical Practice Guidelines for Clostridium difficile Infection in Adults and Children: 2017 Update. Clin Infect Dis 2018;66(7):e1–e48. Table 2 | https://pmc.ncbi.nlm.nih.gov/articles/PMC6018983/ | RAW |
| IDSA-SSTI-2014 | Stevens DL et al. Practice Guidelines for the Diagnosis and Management of Skin and Soft Tissue Infections: 2014 Update. Clin Infect Dis 2014;59(2):e10–e52. Table 2 | https://academic.oup.com/cid/article/59/2/e10/2895845 (recommendation text also read RAW at https://www.idsociety.org/practice-guideline/skin-and-soft-tissue-infections/, which does not reproduce Table 2) | TOOL (table) / RAW (recommendation text) |
| IDSA-MRSA-2011 | Liu C et al. Clinical Practice Guidelines by the IDSA for the Treatment of MRSA Infections in Adults and Children. Clin Infect Dis 2011;52(3):e18–e55. Table 3 | https://academic.oup.com/cid/article/52/3/e18/306145 | TOOL |
| WHO-2014 | Revised WHO classification and treatment of childhood pneumonia at health facilities: evidence summaries. WHO, 2014 | https://iris.who.int/handle/10665/137319 (file: https://iris.who.int/server/api/core/bitstreams/38cf9b2a-5d7d-49de-a27b-46d5ac4fc387/content) | RAW |
| WHO-AWaRe-2022 | The WHO AWaRe (Access, Watch, Reserve) antibiotic book. WHO, 2022 | https://iris.who.int/handle/10665/365237 (file: https://iris.who.int/server/api/core/bitstreams/61517f8b-3a34-413c-ae36-67a059ebe485/content) | RAW |
| AEMPS fichas técnicas | Spanish regulator product information, section 4.2 | https://cima.aemps.es/cima/dochtml/ft/{n}/FT_{n}.html (n given per item) | RAW |
| DailyMed | US FDA labels (SPL) | https://dailymed.nlm.nih.gov/dailymed/services/v2/spls/{setid}.xml (setid given per item) | RAW |

Not opened: AAP Red Book (subscription), BNF for Children (403 from both direct download and fetch tool), EMA/eMC SmPCs (not attempted; the AEMPS sheets were used instead), any IDSA "Part 2" treatment update for pharyngitis (none found).

---

## Gap 1 — Group A streptococcal pharyngitis (children)

### 1.1 IDSA 2012, Table 2 "Antibiotic Regimens Recommended for Group A Streptococcal Pharyngitis" — RAW

| Drug / route | Dose | Interval | Max per dose | Max per day | Duration | Age / weight limits | Verbatim quote (dose cell; duration cell) |
|---|---|---|---|---|---|---|---|
| Benzathine penicillin G, IM | 600 000 U or 1 200 000 U | single dose | — | — | 1 dose | <27 kg / ≥27 kg | "<27 kg: 600 000 U; ≥27 kg: 1 200 000 U"; "1 dose" |
| Penicillin V, oral | 250 mg | 2 or 3 times daily (children) | 250 mg (children); 500 mg (adolescents/adults) | not stated | 10 d | "Children" vs "adolescents and adults" (no age cut-off given) | "Children: 250 mg twice daily or 3 times daily; adolescents and adults: 250 mg 4 times daily or 500 mg twice daily"; "10 d" |
| Amoxicillin, oral | 50 mg/kg once daily, or 25 mg/kg twice daily | q24h or q12h | 1000 mg (once daily) / 500 mg (twice daily) | 1000 mg | 10 d | — | "50 mg/kg once daily (max = 1000 mg); alternate: 25 mg/kg (max = 500 mg) twice daily"; "10 d" |
| Cephalexin, oral (penicillin allergy) | 20 mg/kg/dose | twice daily | 500 mg | 1000 mg (derived: 2 × 500) | 10 d | footnote b | "20 mg/kg/dose twice daily (max = 500 mg/dose)"; "10 d" |
| Cefadroxil, oral (penicillin allergy) | 30 mg/kg | once daily | 1 g | 1 g | 10 d | footnote b | "30 mg/kg once daily (max = 1 g)"; "10 d" |
| Clindamycin, oral (penicillin allergy) | 7 mg/kg/dose | 3 times daily | 300 mg | 900 mg (derived) | 10 d | — | "7 mg/kg/dose 3 times daily (max = 300 mg/dose)"; "10 d" |
| Azithromycin, oral (penicillin allergy) | 12 mg/kg | once daily | 500 mg | 500 mg | 5 d | footnote c | "12 mg/kg once daily (max = 500 mg)"; "5 d" |
| Clarithromycin, oral (penicillin allergy) | 7.5 mg/kg/dose | twice daily | 250 mg | 500 mg (derived) | 10 d | footnote c | "7.5 mg/kg/dose twice daily (max = 250 mg/dose)"; "10 d" |

Footnotes, verbatim: "b Avoid in individuals with immediate type hypersensitivity to penicillin." "c Resistance of GAS to these agents is well-known and varies geographically and temporally."

Recommendation 9, verbatim: "Treatment of GAS pharyngitis in penicillin-allergic individuals should include a first generation cephalosporin (for those not anaphylactically sensitive) for 10 days, clindamycin or clarithromycin for 10 days, or azithromycin for 5 days (strong, moderate)."

Body text, verbatim: "Intramuscular benzathine penicillin G therapy is preferred for patients deemed unlikely to complete a full 10-day course of oral therapy."

Chronic carrier table in the same guideline (useful only if the calculator covers carriers), verbatim cells: clindamycin "20–30 mg/kg/d in 3 doses (max = 300 mg/dose)" 10 d; "Penicillin V: 50 mg/kg/d in 4 doses × 10 d (max = 2000 mg/d); rifampin: 20 mg/kg/d in 1 dose × last 4 d of treatment (max = 600 mg/d)"; amoxicillin–clavulanic acid "40 mg amoxicillin/kg/d in 3 doses (max = 2000 mg amoxicillin/d)" 10 d.

### 1.2 Newer IDSA update — RAW

Only Part 1 exists and it contains no dosing. Verbatim from the IDSA page: "This is Part 1 of an update to the 2012 IDSA Guideline on Diagnosis and Management of Group A Streptococcal Pharyngitis." and "Here we include recommendations on risk assessment for GAS using clinical scoring systems in children and adults. Subsequent updates will address these other areas of uncertainty." Two web searches found no published treatment part. IDSA 2012 Table 2 therefore remains the current IDSA dosing source.

### 1.3 CDC clinical guidance page — TOOL

Identical to IDSA 2012 except azithromycin. Excerpts returned:

- "250 mg twice daily or 3 times daily for 10 days" (penicillin V, children)
- "50 mg/kg once daily (maximum = 1000 mg) for 10 days"; "25 mg/kg (maximum = 500 mg) twice daily for 10 days" (amoxicillin)
- "Weight less than 27 kg" "1 dose (600,000 U)"; "Weight 27 kg or more" "1 dose (1,200,000 U)" (benzathine penicillin G)
- "20 mg/kg/dose twice daily (maximum = 500 mg/dose) for 10 days" (cephalexin)
- "30 mg/kg once daily (maximum = 1 g) for 10 days" (cefadroxil)
- "7 mg/kg/dose 3 times daily (maximum = 300 mg/dose) for 10 days" (clindamycin)
- "12 mg/kg once (maximum = 500 mg), then 6 mg/kg (max=250 mg) once daily for the next 4 days" (azithromycin)
- "7.5 mg/kg/dose twice daily (maximum = 250 mg/dose) for 10 days" (clarithromycin)

### 1.4 NICE NG84, Table 2 "Antibiotics for children and young people under 18 years" — RAW

| Drug | Dose | Interval | Duration | Verbatim |
|---|---|---|---|---|
| Phenoxymethylpenicillin (first choice) | by age band | 4 times or twice a day | 5 to 10 days | "1 month to 11 months, 62.5 mg four times a day or 125 mg twice a day for 5 to 10 days" / "1 year to 5 years, 125 mg four times a day or 250 mg twice a day for 5 to 10 days" / "6 years to 11 years, 250 mg four times a day or 500 mg twice a day for 5 to 10 days" / "12 years to 17 years, 500 mg four times a day or 1,000 mg twice a day for 5 to 10 days" / "Five days may be enough for symptomatic cure; but a 10-day course may increase the chance of microbiological cure" |
| Clarithromycin (penicillin allergy) | by weight band | twice a day | 5 days | "1 month to 11 years: Under 8 kg, 7.5 mg/kg twice a day for 5 days / 8 kg to 11 kg, 62.5 mg twice a day for 5 days / 12 kg to 19 kg, 125 mg twice a day for 5 days / 20 kg to 29 kg, 187.5 mg twice a day for 5 days / 30 kg to 40 kg, 250 mg twice a day for 5 days / 12 years to 17 years, 250 mg to 500 mg twice a day for 5 days" |
| Erythromycin (penicillin allergy in pregnancy) | — | — | 5 days | "8 years to 17 years, 250 mg to 500 mg four times a day or 500 mg to 1,000 mg twice a day for 5 days" |

NICE lists no benzathine penicillin, amoxicillin, cefalexin, cefadroxil, clindamycin or azithromycin for sore throat.

### 1.5 WHO AWaRe 2022, Table 6.5 (printed p. 58–59) — RAW

"Empiric antibiotic treatment in patients with a high likelihood of Streptococcus pyogenes pharyngitis", Children column, verbatim:

- First choice: "Amoxicillin (oral): 80–90 mg/kg/day / Oral weight bands: / 3–6 kg: 250 mg given every 12 hours / 6–< 10 kg: 375 mg given every 12 hours / 10–< 15 kg: 500 mg given every 12 hours / 15–< 20 kg: 750 mg given every 12 hours / ≥ 20 kg: 500 mg given every 8 hours or 1 g given every 12 hours" OR "Phenoxymethylpenicillin (oral): 10–15 mg/kg/dose (16 000–24 000 IU/kg/dosea) given every 6 to 8 hours". Duration: "5b or 10c days depending on the local prevalence or previous history of rheumatic fever".
- Second choice: "Cefalexin (oral): 25 mg/kg/dose given every 12 hours / Oral weight bands: / 3–< 6 kg: 125 mg given every 12 hours / 6–< 10 kg: 250 mg given every 12 hours / 10–< 15 kg: 375 mg given every 12 hours / 15–< 20 kg 500 mg given every 12 hours / 20–< 30 kg: 625 mg given every 12 hours / ≥ 30 kg: use adult dose" (adult dose in the same table: "500 mg given every 8 hours") OR "Clarithromycin d (oral): 7.5 mg/kg/dose given every 12 hours". Duration: "5 days".
- Footnotes: "b In settings with a low prevalence of rheumatic fever or in patients with no history of rheumatic fever or rheumatic heart disease." "c In settings with a high prevalence of rheumatic fever or in patients with a history of rheumatic fever or rheumatic heart disease and who are aged between 3 and 21 years."

### 1.6 Red Book — NO

Not opened (subscription). The CDC page above is the nearest open source; I cannot confirm that its azithromycin regimen is the Red Book's.

### 1.7 Differences between sources (pharyngitis)

| Point | IDSA 2012 | CDC page | NICE NG84 | WHO AWaRe 2022 |
|---|---|---|---|---|
| Amoxicillin | 50 mg/kg once daily, max 1000 mg; or 25 mg/kg bid, max 500 mg/dose; 10 d | same | not listed | 80–90 mg/kg/day in weight bands; 5 or 10 d |
| Penicillin V (children) | 250 mg bid–tid, 10 d | same | age bands, 5–10 d | 10–15 mg/kg/dose q6–8h, 5 or 10 d |
| Azithromycin | 12 mg/kg once daily, max 500 mg, 5 d | 12 mg/kg day 1 (max 500 mg), then 6 mg/kg (max 250 mg) for 4 days | not listed | not listed as an option (footnote d mentions it only as an alternative when clarithromycin is unavailable) |
| Clarithromycin | 7.5 mg/kg/dose bid, max 250 mg/dose, 10 d | same | weight bands, 5 d | 7.5 mg/kg/dose q12h, 5 d |
| Cefalexin | 20 mg/kg/dose bid, max 500 mg/dose, 10 d | same | not listed | 25 mg/kg/dose q12h, weight bands, 5 d |
| Benzathine penicillin G | <27 kg 600 000 U; ≥27 kg 1 200 000 U | same | not listed | not listed for pharyngitis |

---

## Gap 2 — Acute otitis media (children)

### 2.1 AAP 2013, Table 5 — RAW (mirror copy of the journal PDF)

Title, verbatim: "TABLE 5 Recommended Antibiotics for (Initial or Delayed) Treatment and for Patients Who Have Failed Initial Antibiotic Treatment".

The cell strings below are verbatim. The PDF text extraction flattens the columns, so the assignment of cells to columns is my reading of the layout; it matches the running text of the guideline.

| Column | Cells (verbatim) |
|---|---|
| Initial treatment — Recommended first-line | "Amoxicillin (80–90 mg/ kg per day in 2 divided doses)" or "Amoxicillin-clavulanatea (90 mg/kg per day of amoxicillin, with 6.4 mg/kg per day of clavulanate [amoxicillin to clavulanate ratio, 14:1] in 2 divided doses)" |
| Initial treatment — Alternative (if penicillin allergy) | "Cefdinir (14 mg/kg per day in 1 or 2 doses)"; "Cefuroxime (30 mg/kg per day in 2 divided doses)"; "Cefpodoxime (10 mg/kg per day in 2 divided doses)"; "Ceftriaxone (50 mg IM or IV per day for 1 or 3 d)" |
| After 48–72 h of failure — Recommended first-line | "Amoxicillin-clavulanatea (90 mg/kg per day of amoxicillin, with 6.4 mg/kg per day of clavulanate in 2 divided doses)" or "Ceftriaxone (50 mg IM or IV for 3 d)" |
| After 48–72 h of failure — Alternative | "Ceftriaxone, 3 d"; "Clindamycin (30–40 mg/kg per day in 3 divided doses), with or without third-generation cephalosporin"; "Failure of second antibiotic": "Clindamycin (30–40 mg/kg per day in 3 divided doses) plus third-generation cephalosporin"; "Tympanocentesisb"; "Consult specialistb" |

Footnotes, verbatim: "a May be considered in patients who have received amoxicillin in the previous 30 d or who have the otitis-conjunctivitis syndrome." "b Perform tympanocentesis/drainage if skilled in the procedure, or seek a consultation from an otolaryngologist for tympanocentesis/drainage. If the tympanocentesis reveals multidrug-resistant bacteria, seek an infectious disease specialist consultation."

Note on ceftriaxone: the table prints "50 mg" without "/kg". The running text on the same page gives the per-kg dose: "Alternative initial antibiotics include cefdinir (14 mg/kg per day in 1 or 2 doses), cefuroxime (30 mg/kg per day in 2 divided doses), cefpodoxime (10 mg/kg per day in 2 divided doses), or ceftriaxone (50 mg/kg, administered intramuscularly)."

Maximum doses: Table 5 states none for any drug.

Macrolides, verbatim: "Macrolides, such as erythromycin and azithromycin, have limited efficacy against both H influenzae and S pneumoniae." Clarithromycin does not appear in Table 5. Clindamycin, verbatim: "Clindamycin alone (30–40 mg/kg per day in 3 divided doses) may be used for suspected penicillin-resistant S pneumoniae; however, the drug will likely not be effective for the multidrug-resistant serotypes."

Duration by age, verbatim ("Duration of Therapy"): "Thus, for children younger than 2 years and children with severe symptoms, a standard 10-day course is recommended. A 7-day course of oral antibiotic appears to be equally effective in children 2 to 5 years of age with mild or moderate AOM. For children 6 years and older with mild to moderate symptoms, a 5- to 7-day course is adequate treatment."

### 2.2 WHO AWaRe 2022, Table 5.3 (printed p. 44–45) — RAW

"Empiric antibiotic treatment for acute bacterial otitis media", Children column, verbatim:

- First choice: "Amoxicillin (oral): 80–90 mg/kg/day / Oral weight bands: / 3–6 kg: 250 mg given every 12 hours / 6–< 10 kg: 375 mg given every 12 hours / 10–< 15 kg: 500 mg given every 12 hours / 15–< 20 kg: 750 mg given every 12 hours / ≥ 20 kg: 500 mg given every 8 hours or 1 g given every 12 hours". Duration "5 days".
- Second choice: "Amoxicillin+clavulanic acid a (oral): 80–90 mg/kg/day of amoxicillin component / Oral weight bands: / 3–< 6 kg: 250 mg of amoxicillin/dose given every 12 hours / 6–< 10 kg: 375 mg of amoxicillin/dose given every 12 hours / 10–< 15 kg: 500 mg of amoxicillin/dose given every 12 hours / 15–< 20 kg: 750 mg of amoxicillin/dose given every 12 hours / ≥ 20 kg: 500 mg of amoxicillin/dose given every 8 hours or 1 g of amoxicillin/dose given every 12 hours". Duration "5 days".

### 2.3 Base versus AAP 2013

| Base entry | AAP 2013 | Result |
|---|---|---|
| Amoxicillin 80–90 mg/kg/day | "Amoxicillin (80–90 mg/ kg per day in 2 divided doses)" | agrees; AAP specifies 2 doses |
| Amoxicillin-clavulanate 90/6.4 mg/kg/day in 2 doses | same, ratio 14:1 | agrees |
| Ceftriaxone 50 mg/kg/day 1–3 days (max 1 g/day) | initial "for 1 or 3 d"; after failure "for 3 d"; no maximum in AAP | regimen agrees; the 1 g cap is not from AAP (see 3.11 for the US label source) |
| Cefuroxime 30 mg/kg/day in 2 doses | "Cefuroxime (30 mg/kg per day in 2 divided doses)" | agrees |
| Clarithromycin 15 mg/kg/day | not in Table 5; macrolides described as having limited efficacy | not supported by AAP 2013 |
| (missing) | cefdinir 14 mg/kg/day in 1–2 doses; cefpodoxime 10 mg/kg/day in 2 doses; clindamycin 30–40 mg/kg/day in 3 doses | to add if wanted |
| (missing) duration | <2 y or severe: 10 d; 2–5 y mild/moderate: 7 d; ≥6 y mild/moderate: 5–7 d | to add |

---

## Gap 3 — Maximum doses

### 3.1 Oral amoxicillin, 80–90 mg/kg/day

| Source | Dose / interval | Max per dose | Max per day | Limits | Verbatim | Verified |
|---|---|---|---|---|---|---|
| PIDS/IDSA-CAP-2011, Table 7 (outpatient, ≥5 years) | 90 mg/kg/day in 2 doses | not stated | 4 g | ≥5 years | "Oral amoxicillin (90 mg/kg/day in 2 dosesb to a maximum of 4 g/dayc)"; footnote "c Not evaluated prospectively for safety." | RAW |
| PIDS/IDSA-CAP-2011, Table 5 footnote | — | — | adult dose | all oral agents | "Doses for oral therapy should not exceed adult doses." | RAW |
| WHO-AWaRe-2022, Table 50.1 (p. 640) | 80–90 mg/kg/day, weight bands q12h | 1 g (top band) | "1.5 g but can be higher" | top band ≥20 kg | "Amoxicillin Oral: 80–90 mg/kg/day / Up to a maximum daily dose of 1.5 g but can be higher for serious bacterial infections / Weight bands: 3–<6 kg: 250 mg given every 12 hours / 6–<10 kg: 375 mg given every 12 hours / 10–<15 kg: 500 mg given every 12 hours / 15–<20 kg: 750mg given every 12 hours / ≥ 20 kg: 500 mg given every 8 hours or 1 g given every 12 hours" | RAW |
| AEMPS, Amoxicilina Normon 250 mg/5 ml (n=63354, text revised March 2026) | children <40 kg: 20–90 mg/kg/day (otitis, CAP), 40–90 mg/kg/day (streptococcal pharyngitis) | not stated | not stated for <40 kg | ≥40 kg take adult dose | "De 20 a 90 mg/kg/día en dosis divididas*." / "*Se deben considerar pautas posológicas de dos veces al día cuando la dosis está en el rango superior." / "Los niños que pesen 40 kg o más deben tomar la dosis de adultos." Adult otitis/pharyngitis dose: "500 mg cada 8 horas, de 750 mg a 1 g cada 12 horas. Para infecciones graves, de 750 mg a 1 g cada 8 horas, durante 10 días." Adult CAP: "De 500 mg a 1 g cada 8 horas." | RAW |
| AAP-AOM-2013 | 80–90 mg/kg/day in 2 doses | not stated | not stated | — | see 2.1 | RAW |

Note: the WHO book's "1.5 g" daily ceiling and its own top weight band ("1 g given every 12 hours" = 2 g/day) do not match each other. Both are quoted as printed.

### 3.2 Oral amoxicillin, 40–45 mg/kg/day

| Source | Dose / interval | Max | Limits | Verbatim | Verified |
|---|---|---|---|---|---|
| DailyMed, amoxicillin tablets/suspension/capsules (setid b07b5ac4-253e-4c83-91c3-3fdc46e91a0f) | <40 kg, lower respiratory tract: 45 mg/kg/day q12h or 40 mg/kg/day q8h | no mg cap stated for <40 kg; above 40 kg the fixed dose applies: 875 mg q12h or 500 mg q8h | ≥3 months; 40 kg threshold | Column headers: "Recommended Dosage for Adults and Pediatric Patients Aged 3 Months and Older and Weight Greater than 40 kg" / "Recommended Dosage for Pediatric Patients Aged 3 Months and Older and Weight Less than 40 kg". Lower respiratory tract row: "875 mg every 12 hours or 500 mg every 8 hours" / "45 mg/kg/day in divided doses every 12 hours or 40 mg/kg/day in divided doses every 8 hours" | RAW |
| PIDS/IDSA-CAP-2011, Table 5 | "45 mg/kg/day in 3 doses" (alternative to 90 mg/kg/day in 2 doses for pneumococcus with penicillin MIC ≤2.0 μg/mL) | "Doses for oral therapy should not exceed adult doses." | — | "Preferred: amoxicillin (90 mg/kg/day in 2 doses or 45 mg/kg/day in 3 doses)" | RAW |

### 3.3 Amoxicillin-clavulanate 90 mg/kg/day (14:1)

| Source | Dose / interval | Max per dose | Max per day | Limits | Verbatim | Verified |
|---|---|---|---|---|---|---|
| PIDS/IDSA-CAP-2011, Table 7 (≥5 years) | 90 mg/kg/day (amoxicillin) in 2 doses | 2000 mg | 4000 mg | ≥5 years | "oral amoxicillin clavulanate (amoxicillin component, 90 mg/kg/day in 2 dosesb to a maximum dose of 4000 mg/day, eg, one 2000-mg tablet twice dailyb)" | RAW |
| DailyMed, Augmentin ES-600 (setid 59979859-8c9b-4173-a5e3-2e6312fd9d79) | 90 mg/kg/day q12h, 10 days | not stated | not stated | 3 months–12 years, ≤40 kg only | "Based on the amoxicillin component (600 mg/5 mL), the recommended dose of AUGMENTIN ES-600 is 90 mg/kg/day divided every 12 hours, administered for 10 days ... This dose provides 6.4 mg/kg/day of the clavulanic acid component." / "Pediatric patients weighing greater than 40 kg:Experience with AUGMENTIN ES-600 in this group is not available." | RAW |
| WHO-AWaRe-2022, Table 50.1 (p. 641) | 80–90 mg/kg/day (amoxicillin), weight bands | 1 g amoxicillin (top band) | not stated | ≥20 kg top band | "≥ 20 kg: 500 mg of amoxicillin/dose given every 8 hours or 1 g of amoxicillin/dose given every 12 hours" | RAW |
| AEMPS, Augmentine 100 mg/ml + 12,5 mg/ml (n=59051, rev. May 2024) — this is the 8:1 product, not 14:1 | 40/5 to 80/10 mg/kg/day in 3 doses | — | 3000 mg/375 mg | <40 kg | "40 mg/5 mg/kg/día a 80 mg/10 mg/kg/día (sin sobrepasar 3 000 mg/375 mg al día) dividido en tres veces al día, dependiendo de la gravedad de la infección." | RAW |

### 3.4 Oral cefuroxime axetil 30 mg/kg/day

| Source | Dose / interval | Max per dose | Max per day | Duration | Limits | Verbatim | Verified |
|---|---|---|---|---|---|---|---|
| DailyMed, cefuroxime axetil for oral suspension (setid 135e2dfc-eb47-4d04-a903-a081d36c267e; label dated 2013) | AOM: 30 mg/kg/day divided b.i.d. | 500 mg (derived) | 1,000 mg | 10 days | 3 months–12 years | Table: "Acute otitis media | 30 mg/kg/day divided b.i.d. | 1,000 mg | 10"; "Pharyngitis/tonsillitis | 20 mg/kg/day divided b.i.d. | 500 mg | 10"; "Impetigo | 30 mg/kg/day divided b.i.d. | 1,000 mg | 10" (column header "Daily Maximum Dose") | RAW |
| AEMPS, Zinnat 250 mg/5 ml granulado (n=62806, rev. 09/2023) | AOM: 15 mg/kg twice daily | 250 mg | 500 mg (derived) | not stated for AOM | 3 months up; <40 kg | "Otitis media aguda | 15 mg/kg dos veces al día, hasta un máximo de 250 mg dos veces al día"; "Amigdalitis aguda y faringitis, | 10 mg/kg dos veces al día, hasta un máximo de 250 mg dos veces al día"; "No hay experiencia del uso de Zinnat en niños menores de 3 meses." | RAW |

The two regulators disagree on the ceiling for the same 30 mg/kg/day regimen: 1000 mg/day (US) versus 250 mg twice daily (Spain).

### 3.5 Clarithromycin 15 mg/kg/day — number of daily doses and maximum

All sources give two daily doses.

| Source | Dose / interval | Max per dose | Max per day | Duration | Verbatim | Verified |
|---|---|---|---|---|---|---|
| PIDS/IDSA-CAP-2011, Table 7 | 15 mg/kg/day in 2 doses | 500 mg (derived) | 1 g | 7–14 days | "oral clarithromycin (15 mg/kg/day in 2 doses to a maximum of 1 g/day)"; "oral clarithromycin (15 mg/kg/day in 2 doses for 7-14 days)" | RAW |
| WHO-AWaRe-2022, Table 50.1 (p. 646) | 7.5 mg/kg/dose q12h | — | 1 g | — | "Clarithromycin Oral: 7.5 mg/kg/dose given every 12 hours / Up to a maximum daily dose of 1 g" | RAW |
| DailyMed, clarithromycin for suspension (setid 22457862-0f88-4be8-b507-1c8f264269f2) | 15 mg/kg/day divided q12h | adult dose | adult dose | 10 days | "The recommended daily dosage is 15 mg/kg/day divided every 12 hours for 10 days (up to the adult dose)." | RAW |
| AEMPS, Claritromicina Sandoz 25 mg/ml (n=66388, rev. 05/2024) | 7.5 mg/kg twice daily | 250 mg in top band (10 ml of 25 mg/ml) | not stated | 5–10 days | "Niños de 6 meses a 12 años: La dosis recomendada es de 7,5 mg/kg dos veces al día." Bands: "8 – 11 kg | 1 – 2 años | 2,5 ml dos veces al día | 12 – 19 kg | 2 – 4 años | 5,0 ml dos veces al día | 20 – 29 kg | 4 – 8 años | 7,5 ml dos veces al día | 30 – 40 kg | 8 – 12 años | 10,0 ml dos veces aldía". "La duración habitual del tratamiento en niños menores de 12 años es de 5 a 10 días." | RAW |
| IDSA-GAS-2012 (pharyngitis only) | 7.5 mg/kg/dose bid | 250 mg | 500 mg (derived) | 10 d | "7.5 mg/kg/dose twice daily (max = 250 mg/dose)" | RAW |

### 3.6 IV ciprofloxacin

| Source | Indication | Dose / interval | Max per dose | Max per day | Verbatim | Verified |
|---|---|---|---|---|---|---|
| AEMPS, Ciprofloxacino Normon 2 mg/ml perfusión (n=62646, rev. September 2026) | "Otras infecciones graves" | 10 mg/kg three times daily | 400 mg | 1200 mg (derived) | "Otras infecciones graves. | 10 mg/kg de peso corporal tres veces al día, con un máximo de 400 mg por dosis." | RAW |
| same | complicated UTI / pyelonephritis | 6–10 mg/kg three times daily | 400 mg | — | "6 mg/kg de peso corporal a 10 mg/kg de peso corporal, tres veces al día, con un máximo de 400 mg por dosis." "10 a 21 días." | RAW |
| same | cystic fibrosis | 10 mg/kg three times daily | 400 mg | — | "10 mg/kg de peso corporal, tres veces al día, con un máximo de 400 mg por dosis." "10 a 14 días." | RAW |
| DailyMed, ciprofloxacin injection (setid f406e796-17d9-4465-b8a7-00d966a4ba74), Table 3 | complicated UTI / pyelonephritis, 1–17 years | 6–10 mg/kg every 8 hours | 400 mg | — | "6 mg/kg to 10 mg/kg (maximum 400 mg per dose; not to be exceeded even in patients weighing more than 51 kg) | Every 8 hours | 10 to 21 days" | RAW |
| WHO-AWaRe-2022, Table 50.1 (p. 646) | — | (oral 15 mg/kg/dose q12h) | — | 1.2 g IV; 1.5 g oral | "Up to a maximum daily dose of 1.5 g (oral), or 1.2 g (IV)" | RAW |
| PIDS/IDSA-CAP-2011, Table 5 (H. influenzae, alternative) | pneumonia | 30 mg/kg/day every 12 hours | not stated | not stated | "intravenous ciprofloxacin (30 mg/kg/day every 12 hours)" | RAW |

None of these sources gives 20 mg/kg/day IV as a standard regimen; the range found is 18–30 mg/kg/day in 3 doses (labels) or 30 mg/kg/day in 2 doses (PIDS/IDSA).

### 3.7 C. difficile — oral vancomycin and metronidazole

IDSA/SHEA 2017, Table 2 "Recommendations for the Treatment of Clostridium difficile Infection in Children" — RAW. Columns: Clinical Definition / Recommended Treatment / Pediatric Dose / Maximum Dose / Strength.

| Clinical definition | Treatment (verbatim) | Pediatric dose (verbatim) | Maximum dose (verbatim) | Strength / quality |
|---|---|---|---|---|
| Initial episode, non-severe | "Metronidazole × 10 days (PO), OR Vancomycin × 10 days (PO)" | "7.5 mg/kg/dose tid or qid" / "10 mg/kg/dose qid" | "500 mg tid or qid" / "125 mg qid" | Weak/Low; Weak/Low |
| Initial episode, severe/fulminant | "Vancomycin × 10 days (PO or PR) with or without metronidazole × 10 days (IV)a" | "10 mg/kg/dose qid" / "10 mg/kg/dose tid" | "500 mg qid" / "500 mg tid" | Strong/Moderate; Weak/Low |
| First recurrence, non-severe | "Metronidazole × 10 days (PO), OR Vancomycin × 10 days (PO)" | "7.5 mg/kg/dose tid or qid" / "10 mg/kg/dose qid" | "500 mg tid or qid" / "125 mg qid" | Weak/Low |
| Second or subsequent recurrence | "Vancomycin in a tapered and pulsed regimenb, OR Vancomycin for 10 days followed by rifaximinc for 20 days, OR Fecal microbiota transplantation" | "10 mg/kg/dose qid" / "Vancomycin: 10 mg/kg/dose qid; rifaximin: no pediatric dosing" | "125 mg qid" / "Vancomycin: 500 mg qid; rifaximin: 400 mg tid" | Weak/Low; Weak/Low; Weak/Very low |

Footnote b, verbatim: "Tapered and pulsed regimen: vancomycin 10 mg/kg with max of 125 mg 4 times per day for 10–14 days, then 10 mg/kg with max of 125 mg 2 times per day for a week, then 10 mg/kg with max of 125 mg once per day for a week, and then 10 mg/kg with max of 125 mg every 2 or 3 days for 2–8 weeks."

Recommendations, verbatim: "Either metronidazole or vancomycin is recommended for the treatment of children with an initial episode or first recurrence of nonsevere CDI (see Pediatric treatment section for dosing) (weak recommendation, low quality of evidence)". "For children with an initial episode of severe CDI, oral vancomycin is recommended over metronidazole (strong recommendation, moderate quality of evidence)."

Other sources for oral vancomycin:

| Source | Dose / interval | Max per dose | Max per day | Duration | Verbatim | Verified |
|---|---|---|---|---|---|---|
| DailyMed, Vancocin capsules (setid a078d9c2-f89c-4f9f-8ded-60ffb2983c3f) and Firvanq (setid 5aca5508-b577-446c-9980-ab4c7582b4b9) | 40 mg/kg/day in 3 or 4 divided doses | not stated | 2 g | 7–10 days | "Pediatric Patients (less than 18 years of age): 40 mg/kg in 3 or 4 divided doses for 7 to 10 days. The total daily dosage should not exceed 2 g." | RAW |
| AEMPS, Vancomicina Normon 1000 mg (n=85651, rev. January 2026), oral use | 10 mg/kg every 6 hours | not stated | 2 g | 10 days | "Recién nacidos, lactantes y niños menores de 12 años / La dosis recomendada de vancomicina es de 10 mg/kg por vía oral cada 6 horas durante 10 días. La dosis diaria máxima no debe exceder de 2 g." | RAW |
| WHO-AWaRe-2022, Table 33.3 (p. 473) and Table 50.1 (p. 652) | 5–10 mg/kg/dose every 6 hours (second choice) | not stated | 2 g | 10 days | "Vancomycin (oral): • Neonates: 5–10 mg/kg/dose given every 6 hours • Children: 5–10 mg/kg/dose given every 6 hours"; "Oral: 5–10 mg/kg/dose given every 6 hours / Up to a maximum daily dose of 2 g (oral)" | RAW |
| WHO-AWaRe-2022, Table 33.3, metronidazole (first choice) | 7.5 mg/kg/dose every 8 hours | adult dose 500 mg | — | 10 days | "Metronidazole (oral): • Neonates: 7.5 mg/kg/dose given every 12 hours • Children: 7.5 mg/kg/dose given every 8 hours / Oral weight bands: / 3–< 6 kg: 30 mg given every 8 hours / 6–< 10 kg: 50 mg given every 8 hours / 10–< 15 kg: 100 mg given every 8 hours / 15–< 20 kg: 150 mg given every 8 hours / 20–< 30 kg: 200 mg given every 8 hours / ≥ 30 kg: use adult dose"; adult: "Metronidazole (oral): 500 mg given every 8 hours" | RAW |

### 3.8 Metronidazole for giardiasis

| Source | Dose / interval | Max | Duration | Limits | Verbatim | Verified |
|---|---|---|---|---|---|---|
| AEMPS, Flagyl 125 mg/5 ml suspensión oral (n=47656, rev. June 2025) | 15–40 mg/kg/day in 2–3 doses; or fixed doses by age | no mg ceiling stated for the mg/kg regimen | 3 days (age-band single daily doses); 5 or 7–10 days for the >10 years alternatives | age bands from 1 year | "Lambliasis (Giardiasis): / Mayores de 10 años: 2.000 mg una vez al día durante 3 días, ó 400 mg tres veces al día durante 5 días, ó 500 mg dos veces al día durante 7-10 días. / Niños de 7 a 10 años: 1.000 mg una vez al día durante 3 días. / Niños de 3 a 7 años: de 600 a 800 mg una vez al día durante 3 días. / Niños de 1 a 3 años: 500 mg una vez al día durante 3 días. / O bien, expresado en mg por kilogramo de peso corporal: / 15-40 mg/kg/día divididos en 2-3 dosis." | RAW |
| CDC giardiasis clinical care page (https://www.cdc.gov/giardia/hcp/clinical-care/index.html) | no doses given | — | — | — | page names metronidazole, tinidazole and nitazoxanide but gives no mg/kg figures | TOOL |

No source opened states a per-dose or per-day maximum in mg for the 30–40 mg/kg/day giardiasis regimen. Inference, not a quote: the only ceilings available are the fixed regimens for patients over 10 years in the same sheet (2000 mg once daily, 400 mg three times daily, or 500 mg twice daily).

### 3.9 Trimethoprim-sulfamethoxazole for skin infection

| Source | Dose / interval | Max per dose | Max per day | Limits | Verbatim | Verified |
|---|---|---|---|---|---|---|
| IDSA-SSTI-2014, Table 2, MRSA SSTI row | 8–12 mg/kg/day (trimethoprim) in 2 divided doses orally | not stated for children; adult "1–2 double-strength tablets bid po" | not stated | "Doses listed are not appropriate for neonates." | Children: "8–12 mg/kg/d (based on trimethoprim component) in either 4 divided doses IV or 2 divided doses po"; Adults: "1–2 double-strength tablets bid po"; comment: "Bactericidal; limited published efficacy data" | TOOL |
| IDSA-MRSA-2011, Table 3, purulent cellulitis row | trimethoprim 4–6 mg/kg/dose every 12 h | not stated for children; adult "1–2 DS tab PO BID" | not stated | not recommended under 2 months (per first-pass summary; exact wording not captured) | "Trimethoprim 4–6 mg/kg/dose, sulfamethoxazole 20–30 mg/kg/dose PO every 12 h" | TOOL |
| DailyMed, sulfamethoxazole and trimethoprim oral suspension (setid 5ff03694-787a-409d-a95e-8422aa27499c) — UTI/otitis dosing, not skin | 8 mg/kg trimethoprim per 24 h in 2 doses | table tops out at 40 kg = 20 mL (160 mg trimethoprim) every 12 hours | — | ≥2 months | "The recommended dose for children with urinary tract infections or acute otitis media is 40 mg/kg sulfamethoxazole and 8 mg/kg trimethoprim per 24 hours, given in two divided doses every 12 hours for 10 days." Table rows: "22 | 10 | 1 (5 mL)", "44 | 20 | 2 (10 mL)", "66 | 30 | 3 (15 mL)", "88 | 40 | 4 (20 mL)" | RAW |
| AEMPS, Septrin Pediátrico 8 mg/40 mg/ml (n=48671, rev. July 2026) — standard dose | about 6 mg trimethoprim/kg/24 h, age bands | 160 mg trimethoprim (>12 years) | — | from 6 weeks | "Lactantes y niños menores de 12 años (las dosis se aproximan a 6 mg de trimetoprima/ 30 mg de sulfametoxazol/kg/24 horas)"; "6 a 12 años: 80 mg de trimetroprima/ 400 mg sulfametoxazol/ 12 horas"; "Adultos y niños mayores de 12 años: 160 mg de trimetoprima/800 mg de sulfametoxazol/12 horas" | RAW |
| WHO-AWaRe-2022, Table 50.1 (p. 651) — UTI/diarrhoea | 20 mg/kg SMX + 4 mg/kg TMP every 12 hours | — | 1200 mg of sulfamethoxazole component | ≥30 kg adult dose | "Oral: 20 mg/kg of sulfamethoxazole + 4 mg/kg of trimethoprim given every 12 hours / Up to a maximum daily dose of 1200 mg of sulfamethoxazole component / Weight bands: 3–< 6 kg: 100 mg + 20 mg given every 12 hours / 6–< 10 kg: 200 mg + 40 mg given every 12 hours / 10–< 30 kg: 400 mg + 80 mg given every 12 hours / ≥ 30 kg: use adult dose" | RAW |

No opened source states an explicit paediatric mg ceiling for the 8–12 mg/kg/day skin regimen. The adult dose in the same IDSA rows (1–2 double-strength tablets twice daily, i.e. 160–320 mg trimethoprim per dose) is the only stated upper reference.

### 3.10 Oral clindamycin

| Source | Indication | Dose / interval | Max per dose | Max per day | Verbatim | Verified |
|---|---|---|---|---|---|---|
| IDSA-SSTI-2014, Table 2 | impetigo | 20 mg/kg/day in 3 doses | adult "300–400 mg qid po" | not stated | "20 mg/kg/d in 3 divided doses po" | TOOL |
| IDSA-SSTI-2014, Table 2 | MSSA SSTI | 25–30 mg/kg/day in 3 doses orally | adult "300–450 mg qid po" | not stated | "25–40 mg/kg/d in 3 divided doses IV or 25–30 mg/kg/d in 3 divided doses po" | TOOL |
| IDSA-SSTI-2014, Table 2 | MRSA SSTI | 30–40 mg/kg/day in 3 doses orally | adult "300–450 mg qid po" | not stated | "25–40 mg/kg/d in 3 divided doses IV or 30–40 mg/kg/d in 3 divided doses po" | TOOL |
| IDSA-MRSA-2011, Table 3, purulent cellulitis | MRSA SSTI | 10–13 mg/kg/dose every 6–8 h | adult "300–450 mg PO TID" | 40 mg/kg/day | "10–13 mg/kg/dose PO every 6–8 h, not to exceed 40 mg/kg/day" | TOOL |
| IDSA-GAS-2012, Table 2 | pharyngitis | 7 mg/kg/dose 3 times daily | 300 mg | 900 mg (derived) | "7 mg/kg/dose 3 times daily (max = 300 mg/dose)" | RAW |
| AAP-AOM-2013, Table 5 | otitis after failure | 30–40 mg/kg/day in 3 doses | not stated | not stated | "Clindamycin (30–40 mg/kg per day in 3 divided doses)" | RAW |
| PIDS/IDSA-CAP-2011, Table 5 | pneumonia, oral step-down | 30–40 mg/kg/day in 3 or 4 doses | "should not exceed adult doses" | — | "oral clindamycina (30–40 mg/kg/day in 3 or 4 doses)" | RAW |
| DailyMed, clindamycin HCl capsules (setid 9b9f898f-6a91-44f8-9508-f9ddfaad27bc) | label | 8–16 mg/kg/day; more severe 16–20 mg/kg/day; 3–4 doses | adult 450 mg every 6 hours | adult 1800 mg (derived) | "Serious infections- 8 to 16 mg/kg/day (4 to 8 mg/lb/day) divided into three or four equal doses. More severe infections- 16 to 20 mg/kg/day (8 to 10 mg/lb/day) divided into three or four equal doses." Adults: "More severe infections- 300 to 450 mg every 6 hours." | RAW |
| DailyMed, Cleocin Pediatric (palmitate) (setid 1ef87b6c-9d32-44ac-8c55-8bf01424cc45) | label | 8–25 mg/kg/day in 3–4 doses | not stated | not stated | "Serious infections: 8–12 mg/kg/day ... Severe infections: 13–16 mg/kg/day ... More severe infections: 17–25 mg/kg/day (8.5–12.5 mg/lb/day) divided into 3 or 4 equal doses." | RAW |
| AEMPS, Dalacin 150 mg cápsulas (n=49152, rev. July 2026) | label | 8–25 mg/kg/day in 3 or 4 doses | — | adult 1800 mg | "La dosis recomendada en niños mayores de 1 mes es de 8 a 25 mg/kg/día dividido en 3 o 4 dosis iguales." Adults: "La dosis recomendada en adultos es de 600 a 1.800 mg al día divididos en 2, 3 o 4 dosis iguales" | RAW |

### 3.11 Ceftriaxone ceiling in otitis (source for the base's "max 1 g")

| Source | Verbatim | Verified |
|---|---|---|
| DailyMed, ceftriaxone for injection (setid 96f3fc1e-84f4-4e9b-91ac-19a1692f227a) | "For the treatment of acute bacterial otitis media, a single intramuscular dose of 50 mg/kg (not to exceed 1 gram) is recommended" | RAW |
| AEMPS, Ceftriaxona Normon 1000 mg (n=62635, rev. March 2026) | "Para el tratamiento inicial de la otitis media aguda, se puede administrar una dosis intramuscular única de 50 mg/kg de ceftriaxona. Datos existentes muy limitados sugieren que, si el niño está gravemente enfermo o una terapia inicial ha fracasado, ceftriaxona puede ser eficaz administrada por vía intramuscular en una dosis de 50 mg/kg diarios durante 3 días." (no mg ceiling stated for otitis) | RAW |

### 3.12 WHO AWaRe 2022 weight-band tables (candidate alternative dosing method) — RAW

All bands are verbatim from the Children column of the named table. Printed page numbers.

**Otitis media — Table 5.3 (p. 44–45).** See 2.2. Duration 5 days.

**Pharyngitis — Table 6.5 (p. 58–59).** See 1.5. Duration 5 or 10 days (cefalexin, clarithromycin: 5 days).

**Mild pneumonia — Table 12.5 (p. 161)**, "Empiric antibiotic treatment for mild cases of community-acquired pneumonia in children":
"Amoxicillin (oral): 80–90 mg/kg/day / Oral weight bands: / 3–< 6 kg: 250 mg given every 12 hours / 6–< 10 kg: 375 mg given every 12 hours / 10–< 15 kg: 500 mg given every 12 hours / 15–< 20 kg: 750 mg given every 12 hours / ≥ 20 kg: 500 mg given every 8 hours or 1 g given every 12 hours". Duration: "3 days for children in areas of low HIV prevalence and no chest indrawing / 5 days if the child has chest indrawing or lives in region of higher HIV prevalence".

**Severe pneumonia — Table 27.6 (p. 379)** (no weight bands):
"Amoxicillin OR Ampicillin (IV/IM): 50 mg/kg/dose given every 12 hours (first week of life) / 50 mg/kg/dose given every 8 hours (>first week of life) AND Gentamicin (IV/IM): • Neonates: 5 mg/kg/dose given once a day • Children: 7.5 mg/kg/dose given once a day / Amoxicillin/ampicillin can be replaced by Benzylpenicillin (IV): 50 000 IU/kg/dose (30 mg/kg/dose) given every 8 hours / If no clinical response to ampicillin AND gentamicin after 48–72 hours, change to: Cefotaxime (IV/IM): 50mg/kg/dose given every 8 hours OR Ceftriaxone (IV/IM): 80 mg/kg/dose given once a day". Duration: "5 days (consider longer treatment if the patient is not clinically stable at day 5)".

**Acute infectious diarrhoea — Table 14.6 (p. 181–183):**
- First choice: "Ciprofloxacin a (oral): 15 mg/kg/dose given every 12 hours / Oral weight bands: / 3–< 6 kg: 50 mg given every 12 hours / 6–< 10 kg: 100 mg given every 12 hours / 10–< 15 kg: 150 mg given every 12 hours / 15–< 20 kg: 200 mg given every 12 hours / 20–< 30 kg: 300 mg given every 12 hours / ≥ 30 kg: use adult dose" (adult: "500 mg given every 12 hours"). Duration "3 days".
- Second choice: "Azithromycin c (oral): 10 mg/kg/dose given once a day" (4 days); "Cefixime d (oral): 10 mg/kg/dose given once a day" (5 days in children); "Sulfamethoxazole+trimethoprim d,e (oral): 20 mg/kg + 4 mg/kg given every 12 hours / Oral weight bands (mg of the sulfamethoxazole/trimethoprim component): / 3–< 6 kg: 100 mg/20 mg given every 12 hours / 6–< 10 kg: 200 mg/40 mg given every 12 hours / 10–< 15 kg: 400 mg/80 mg given every 12 hours / 15–< 20 kg: 400 mg/80 mg given every 12 hours / 20–< 30 kg: 400 mg/80 mg given every 12 hours / ≥ 30 kg: use adult dose" (5 days); "Ceftriaxone d (IV/IM): 80 mg/kg/dose given once a day" (3 days).
- Scope note, verbatim: "Only certain cases of diarrhoeal disease benefit from antibiotic treatment, namely patients with significant acute bloody diarrhoea and/or who are severely immunocompromised."

**Cholera — Table 14.7 (p. 183):** "Azithromycin a (oral): 20 mg/kg, single dose"; second choice "Ciprofloxacin (oral): 15 mg/kg, single dose" OR "Doxycycline (oral) • < 45 kg (< 12 years): 2 to 4 mg/kg, single dose • > 45 kg (> 12 years): 300 mg, single dose".

**Mild skin infection (impetigo, erysipelas, cellulitis) — Table 16.4 (p. 204–205):**
- "Amoxicillin+clavulanic acid b (oral): 80–90 mg/kg/day of amoxicillin component / Oral weight bands: / 3–< 6 kg: 250 mg of amoxicillin/dose given every 12 hours / 6–< 10 kg: 375 mg of amoxicillin/dose given every 12 hours / 10–< 15 kg: 500 mg of amoxicillin/dose given every 12 hours / 15–< 20 kg: 750 mg of amoxicillin/dose given every 12 hours / ≥ 20kg: 500 mg of amoxicillin/dose given every 8 hours or 1 g of amoxicillin/dose given every 12 hours"
- "Cefalexin (oral): 25 mg/kg/dose given every 12 hours / Oral weight bands: / 3–< 6 kg: 125 mg given every 12 hours / 6–< 10 kg: 250 mg given every 12 hours / 10–< 15 kg: 375 mg given every 12 hours / 15–< 20 kg: 500 mg given every 12 hours / 20–< 30 kg: 625 mg given every 12 hours / ≥ 30 kg: use adult dose" (adult: "500 mg given every 8 hours")
- "Cloxacillin a,c (oral): 15 mg/kg/dose given every 6 hours / Oral weight bands: / 3–< 6 kg: 62.5 mg given every 6 hours / 6–< 10 kg: 125 mg given every 6 hours / 10–< 15 kg: 250 mg given every 6 hours / 15–< 20 kg: 375 mg given every 6 hours / ≥ 20 kg: 500 mg given every 6 hours"
- Duration: "5 daysd" with "d The optimal duration of antibiotic treatment is not known (169); duration is often individualized based on clinical response."
- On MRSA cover, verbatim: "In these cases, the literature suggests using clindamycin or sulfamethoxazole+trimethoprim; these options are however not currently listed in the EML and EMLc for this indication."

**C. difficile — Table 33.3 (p. 473).** See 3.7.

**Daily maximums in Table 50.1 "Dosing guidance – Children" (p. 640–652):** amoxicillin oral "Up to a maximum daily dose of 1.5 g but can be higher for serious bacterial infections"; amoxicillin IV "Up to a maximum daily dose of 6 g (serious bacterial infections)"; benzylpenicillin "Up to a maximum daily dose of 6 million IU (3.6 g)"; ceftriaxone "Up to a maximum daily dose of 3 g"; ciprofloxacin "Up to a maximum daily dose of 1.5 g (oral), or 1.2 g (IV)"; clarithromycin "Up to a maximum daily dose of 1 g"; clindamycin "Maximum daily dose: see adult dosing"; cefalexin "Maximum daily dose: use adult dosing"; metronidazole "Up to a maximum daily dose of 1 g" (standard dose); sulfamethoxazole+trimethoprim "Up to a maximum daily dose of 1200 mg of sulfamethoxazole component"; vancomycin "Up to a maximum daily dose of 2 g (IV)" and "Up to a maximum daily dose of 2 g (oral)".

---

## Gap 4 — Pneumonia

### 4.1 PIDS/IDSA 2011, Table 7 "Empiric Therapy for Pediatric Community-Acquired Pneumonia (CAP)" — RAW

| Site of care | Presumed bacterial pneumonia (verbatim) | Presumed atypical pneumonia (verbatim) |
|---|---|---|
| Outpatient, <5 years old (preschool) | "Amoxicillin, oral (90 mg/kg/day in 2 dosesb) Alternative: oral amoxicillin clavulanate (amoxicillin component, 90 mg/kg/day in 2 dosesb)" | "Azithromycin oral (10 mg/kg on day 1, followed by 5 mg/kg/day once daily on days 2–5); Alternatives: oral clarithromycin (15 mg/kg/day in 2 doses for 7-14 days) or oral erythromycin (40 mg/kg/day in 4 doses)" |
| Outpatient, ≥5 years old | "Oral amoxicillin (90 mg/kg/day in 2 dosesb to a maximum of 4 g/dayc); for children with presumed bacterial CAP who do not have clinical, laboratory, or radiographic evidence that distinguishes bacterial CAP from atypical CAP, a macrolide can be added to a β-lactam antibiotic for empiric therapy; alternative: oral amoxicillin clavulanate (amoxicillin component, 90 mg/kg/day in 2 dosesb to a maximum dose of 4000 mg/day, eg, one 2000-mg tablet twice dailyb)" | "Oral azithromycin (10 mg/kg on day 1, followed by 5 mg/kg/day once daily on days 2–5 to a maximum of 500 mg on day 1, followed by 250 mg on days 2–5); alternatives: oral clarithromycin (15 mg/kg/day in 2 doses to a maximum of 1 g/day); erythromycin, doxycycline for children >7 years old" |
| Inpatient (all ages)d, fully immunized, minimal local penicillin resistance | "Ampicillin or penicillin G; alternatives: ceftriaxone or cefotaxime; addition of vancomycin or clindamycin for suspected CA-MRSA" | "Azithromycin (in addition to β-lactam, if diagnosis of atypical pneumonia is in doubt); alternatives: clarithromycin or erythromycin; doxycycline for children >7 years old; levofloxacin for children who have reached growth maturity, or who cannot tolerate macrolides" |
| Inpatient, not fully immunized or significant local penicillin resistance | "Ceftriaxone or cefotaxime; addition of vancomycin or clindamycin for suspected CA-MRSA; alternative: levofloxacin; addition of vancomycin or clindamycin for suspected CA-MRSA" | "Azithromycin (in addition to β-lactam, if diagnosis in doubt); alternatives: clarithromycin or erythromycin; doxycycline for children >7 years old; levofloxacin for children who have reached growth maturity or who cannot tolerate macrolides" |

Footnotes, verbatim: "b See text for discussion of dosage recommendations based on local susceptibility data. Twice daily dosing of amoxicillin or amoxicillin clavulanate may be effective for pneumococci that are susceptible to penicillin." "c Not evaluated prospectively for safety." "d See Table 5 for dosages."

Inpatient doses are in Table 5 "Selection of Antimicrobial Therapy for Specific Pathogens" — RAW:

| Pathogen | Parenteral (verbatim) | Oral (verbatim) |
|---|---|---|
| S. pneumoniae, penicillin MIC ≤2.0 μg/mL | "Preferred: ampicillin (150–200 mg/kg/day every 6 hours) or penicillin (200 000–250 000 U/kg/day every 4–6 h); Alternatives: ceftriaxone (50–100 mg/kg/day every 12–24 hours) (preferred for parenteral outpatient therapy) or cefotaxime (150 mg/kg/day every 8 hours); may also be effective: clindamycin (40 mg/kg/day every 6–8 hours) or vancomycin (40–60 mg/kg/day every 6–8 hours)" | "Preferred: amoxicillin (90 mg/kg/day in 2 doses or 45 mg/kg/day in 3 doses); Alternatives: second- or third-generation cephalosporin (cefpodoxime, cefuroxime, cefprozil)..." |
| S. pneumoniae, penicillin MIC ≥4.0 μg/mL | "Preferred: ceftriaxone (100 mg/kg/day every 12–24 hours); Alternatives: ampicillin (300–400 mg/kg/day every 6 hours)..." | "Preferred: oral levofloxacin ... or oral linezolid ...; Alternative: oral clindamycina (30–40 mg/kg/day in 3 doses)" |
| Group A Streptococcus | "Preferred: intravenous penicillin (100 000–250 000 U/kg/day every 4–6 hours) or ampicillin (200 mg/kg/day every 6 hours); Alternatives: ceftriaxone (50–100 mg/kg/day every 12–24 hours) or cefotaxime (150 mg/kg/day every 8 hours)..." | "Preferred: amoxicillin (50–75 mg/kg/day in 2 doses), or penicillin V (50–75 mg/kg/day in 3 or 4 doses); Alternative: oral clindamycina (40 mg/kg/day in 3 doses)" |
| H. influenzae | "Preferred: intravenous ampicillin (150-200 mg/kg/day every 6 hours) if β-lactamase negative, ceftriaxone (50–100 mg/kg/day every 12-24 hours) if β-lactamase producing, or cefotaxime (150 mg/kg/day every 8 hours); Alternatives: intravenous ciprofloxacin (30 mg/kg/day every 12 hours)..." | "Preferred: amoxicillin (75-100 mg/kg/day in 3 doses) if β-lactamase negative) or amoxicillin clavulanate (amoxicillin component, 45 mg/kg/day in 3 doses or 90 mg/kg/day in 2 doses) if β-lactamase producing" |
| M. pneumoniae / C. pneumoniae | "Preferred: intravenous azithromycin (10 mg/kg on days 1 and 2 of therapy; transition to oral therapy if possible)" | "Preferred: azithromycin (10 mg/kg on day 1, followed by 5 mg/kg/day once daily on days 2–5); Alternatives: clarithromycin (15 mg/kg/day in 2 doses) or oral erythromycin (40 mg/kg/day in 4 doses)..." |

Table 5 footnote, verbatim: "Doses for oral therapy should not exceed adult doses."

Duration, recommendation 54, verbatim: "Treatment courses of 10 days have been best studied, although shorter courses may be just as effective, particularly for more mild disease managed on an outpatient basis. (strong recommendation; moderate-quality evidence)"

History of the 40–45 mg/kg/day dose, verbatim from the evidence text: "At the time of initial pediatric registration trials for amoxicillin in the early 1970s, the vast majority of isolates were highly susceptible, and "standard" dosage therapy (40–45 mg/kg/day divided into 3 equal doses) appeared to be uniformly successful."

IV cefuroxime does not appear with a dose in Table 5 or Table 7; cefuroxime is named only as an oral alternative.

### 4.2 WHO 2014 revised classification and treatment — RAW

Recommendations (printed p. 2), verbatim:

- "Recommendation 1 / Children with fast breathing pneumonia with no chest indrawing or general danger sign should be treated with oral amoxicillin: at least 40mg/kg/dose twice daily (80mg/kg/day) for five days. In areas with low HIV prevalence, give amoxicillin for three days."
- "Recommendation 2 / Children age 2–59 months with chest indrawing pneumonia should be treated with oral amoxicillin: at least 40mg/kg/dose twice daily for five days."
- "Recommendation 3 / Children aged 2–59 months with severe pneumonia should be treated with parenteral ampicillin (or penicillin) and gentamicin as a first-line treatment. — Ampicillin: 50 mg/kg, or benzyl penicillin: 50 000 units per kg IM/IV every 6 hours for at least five days — Gentamicin: 7.5 mg/kg IM/IV once a day for at least five days / Ceftriaxone should be used as a second-line treatment in children with severe pneumonia having failed on the first-line treatment."

Table "Doses of amoxicillin for children 2–59 months of age with pneumonia" (printed p. 19), column "DOSAGE OF AMOXICILLIN DISPERSIBLE TABLETS (250 mg)", verbatim:

| Tool | Category | Age / weight | Dose |
|---|---|---|---|
| iCCM tool for community health workers | Fast breathing pneumonia | "2 months up to 12 months (4–<10 kg)" | "1 tab twice a day x 5 days (10 tabs)" |
| | | "12 months up to 5 years (10–19 kg)" | "2 tabs twice a day x 5 days (20 tabs)" |
| IMCI tool for professional health workers at health facilities: revised | Fast breathing and chest indrawing pneumonia | "2 months up to 12 months (4–<10 kg)" | "1 tab twice a day x 5 days (10 tabs)" |
| | | "12 months up to 3 years (10–<14 kg)" | "2 tabs twice a day x 5 days (20 tabs)" |
| | | "3 years up to 5 years (14–19 kg)" | "3 tabs twice a day x 5 days (30 tabs)" |

### 4.3 Regulatory ceilings for the inpatient drugs — RAW

| Drug | Source | Verbatim |
|---|---|---|
| Cefuroxime IV, children <40 kg, CAP | AEMPS, Cefuroxima Normon 750 mg (n=62908, rev. July 2023), Tabla 2 | "Neumonía adquirida en la comunidad | 30 a 100 mg/kg/día (por vía intravenosa) administrada en 3 ó 4 dosis divididas; una dosis de 60 mg/kg/día es adecuada para la mayoría de las infecciones". Adults: "Neumonía adquirida en la comunidad ... 750 mg cada 8 horas"; "Infecciones graves | 750 mg cada 6 horas (por vía intravenosa) 1.500 mg cada 8 horas (por vía intravenosa)" |
| Ceftriaxone, children 15 days–12 years (<50 kg), CAP | AEMPS, Ceftriaxona Normon 1000 mg (n=62635, rev. March 2026) | "50-80 mg/kg | Una vez al día | ... Neumonía adquirida en la comunidad"; "Para niños que pesen 50 kg o más, se debe administrar la dosis habitual para adultos." |
| Ceftriaxone, paediatric, non-meningitis | DailyMed (setid 36c07ebf-b091-483e-b2c4-f0fc0c2b4186) | "For the treatment of serious miscellaneous infections other than meningitis, the recommended total daily dose is 50 to 75 mg/kg, given in divided doses every 12 hours. The total daily dose should not exceed 2 grams." / "In the treatment of meningitis ... a total daily dose of 100 mg/kg/day (not to exceed 4 grams daily) is recommended." |
| Ceftriaxone, WHO | WHO-AWaRe-2022, Table 50.1 (p. 645) | "Lower dose: 80 mg/kg/dose given once daily / Higher dose: 100 mg/kg/dose given once daily / Up to a maximum daily dose of 3 g" (lower dose for severe CAP; higher dose for meningitis) |
| Benzylpenicillin, WHO | WHO-AWaRe-2022, Table 50.1 (p. 643) | "Severe CAP/sepsis 50 000 IU/kg/dose (30 mg/kg/dose) given every 8 hours ... Up to a maximum daily dose of 6 million IU (3.6 g)" |

### 4.4 Finnish 2023 (base) versus PIDS/IDSA 2011 and WHO

| Base (Finnish 2023) | PIDS/IDSA 2011 | WHO 2014 / AWaRe 2022 |
|---|---|---|
| Amoxicillin 40–45 mg/kg/day in 3 doses × 5 days | 90 mg/kg/day in 2 doses empirically (Table 7); 45 mg/kg/day in 3 doses listed only as the alternative for penicillin-susceptible pneumococcus (Table 5). Duration: 10 days best studied, shorter may be as effective | At least 40 mg/kg/dose twice daily (80 mg/kg/day) × 5 days; 3 days for fast-breathing pneumonia in low-HIV settings |
| High dose 80–90 mg/kg/day, max 4 g/day | 90 mg/kg/day, max 4 g/day (≥5 years), flagged "Not evaluated prospectively for safety" | 80–90 mg/kg/day in weight bands; top band 500 mg q8h or 1 g q12h; stated ceiling 1.5 g/day "but can be higher" |
| Penicillin G 100 000–300 000 U/kg/day in 4–6 doses | 200 000–250 000 U/kg/day every 4–6 h (pneumococcus); 100 000–250 000 U/kg/day (group A strep). No mg/unit ceiling stated | WHO 2014: 50 000 U/kg every 6 hours (= 200 000 U/kg/day). AWaRe 2022: 50 000 IU/kg/dose every 8 hours (= 150 000 IU/kg/day), max 6 million IU/day |
| Cefuroxime IV 100–150 mg/kg/day | no IV cefuroxime dose given | not listed. Spanish product sheet: 30–100 mg/kg/day in 3–4 doses |
| Ceftriaxone 100 mg/kg/day | 50–100 mg/kg/day every 12–24 h; 100 mg/kg/day for penicillin-resistant pneumococcus | 80 mg/kg once daily (second line), max 3 g/day |

---

## Points where sources disagree with the base

1. **Pharyngitis, amoxicillin ceiling.** The base has 50 mg/kg/day in 2 doses with no cap. IDSA 2012 and CDC cap it: 25 mg/kg twice daily with max 500 mg per dose (1000 mg/day), or 50 mg/kg once daily with max 1000 mg. WHO uses a different dose altogether (80–90 mg/kg/day, weight bands).
2. **Pharyngitis, duration.** The base has 10 days. IDSA agrees (10 days; azithromycin 5). NICE gives 5–10 days for penicillin V and 5 days for clarithromycin. WHO gives 5 days where rheumatic fever risk is low and 10 days where it is high; cefalexin and clarithromycin always 5 days.
3. **Pharyngitis, azithromycin.** IDSA 2012: 12 mg/kg once daily for 5 days (max 500 mg). CDC: 12 mg/kg on day 1 (max 500 mg), then 6 mg/kg (max 250 mg) for 4 days. Pick one and cite it.
4. **Pharyngitis, clarithromycin ceiling.** IDSA caps at 250 mg per dose for pharyngitis, which is lower than the 1 g/day ceiling used for pneumonia. A single clarithromycin cap in the calculator would be wrong for one of the two indications.
5. **Otitis, clarithromycin.** The base has clarithromycin 15 mg/kg/day. AAP 2013 Table 5 does not list it and states macrolides have limited efficacy against the main pathogens.
6. **Otitis, ceftriaxone "max 1 g/day".** Not in AAP 2013. The US label supports "not to exceed 1 gram" for the single IM dose. The AAP table itself prints "50 mg" without "/kg" (the running text has 50 mg/kg).
7. **Otitis, duration.** AAP: 10 days under 2 years or severe; 7 days at 2–5 years; 5–7 days from 6 years. WHO: 5 days for all. The base should say which it follows.
8. **Cefuroxime axetil ceiling.** US label: 30 mg/kg/day, max 1000 mg/day. Spanish product sheet: 15 mg/kg twice daily, max 250 mg twice daily (500 mg/day). These cannot both be the cap.
9. **Amoxicillin high-dose ceiling.** 4 g/day (PIDS/IDSA, ≥5 years, "not evaluated prospectively for safety") versus WHO's top band of 1 g twice daily and a stated 1.5 g/day ceiling. The WHO book is internally inconsistent on this point.
10. **Amoxicillin-clavulanate 14:1.** The US label limits the 600 mg/42.9 mg per 5 mL product to children ≤40 kg and gives no data above that; PIDS/IDSA allows up to 4000 mg/day of amoxicillin component using 2000-mg tablets. The Spanish 8:1 suspension has a different regimen (up to 80/10 mg/kg/day in 3 doses, max 3000/375 mg/day) and must not be dosed at 90 mg/kg/day.
11. **IV ciprofloxacin.** The base has 20 mg/kg/day. Labels give 10 mg/kg three times daily (30 mg/kg/day) for severe infections and 6–10 mg/kg three times daily for complicated UTI, both with max 400 mg per dose; PIDS/IDSA gives 30 mg/kg/day in 2 doses. No opened source gives 20 mg/kg/day IV.
12. **Oral vancomycin for C. difficile.** The base's 40 mg/kg/day in 4 doses equals IDSA/SHEA's 10 mg/kg/dose four times daily, but the base lacks the ceilings: 125 mg per dose in non-severe disease, 500 mg per dose in severe/fulminant disease. Labels cap only at 2 g/day. IDSA/SHEA also offers metronidazole 7.5 mg/kg/dose three or four times daily (max 500 mg per dose) for non-severe disease; WHO lists metronidazole as first choice and vancomycin as second.
13. **Clindamycin.** The base has 10–20 mg/kg/day. That fits the US capsule label (8–20 mg/kg/day) but is below guideline doses: IDSA 2014 gives 20 mg/kg/day for impetigo, 25–30 for MSSA and 30–40 for MRSA skin infection; IDSA 2011 gives 10–13 mg/kg/dose every 6–8 h, not to exceed 40 mg/kg/day; AAP and PIDS/IDSA use 30–40 mg/kg/day. For pharyngitis IDSA uses 7 mg/kg/dose three times daily, max 300 mg per dose.
14. **Trimethoprim-sulfamethoxazole.** The base's 8–12 mg/kg/day of trimethoprim matches IDSA 2014 (2 oral doses) and IDSA 2011 (4–6 mg/kg/dose every 12 h). Neither states a paediatric mg ceiling; only the adult dose (1–2 double-strength tablets twice daily) is given. Regulatory sheets use a lower standard dose (6–8 mg/kg/day).
15. **Metronidazole for giardiasis.** The base's 30–40 mg/kg/day is the upper part of the Spanish sheet's 15–40 mg/kg/day in 2–3 doses. No mg ceiling is stated for the per-kg regimen.
16. **Pneumonia, amoxicillin dose.** The Finnish 40–45 mg/kg/day in 3 doses is half of what PIDS/IDSA and WHO recommend empirically (90 and ≥80 mg/kg/day, in 2 doses).
17. **Pneumonia, IV cefuroxime.** The Finnish 100–150 mg/kg/day exceeds the Spanish product sheet's range (30–100 mg/kg/day). PIDS/IDSA and WHO do not list IV cefuroxime.
18. **Pneumonia, ceftriaxone.** Finnish 100 mg/kg/day is the top of the PIDS/IDSA range (50–100) and above WHO's 80 mg/kg and the regulatory CAP range (50–80 mg/kg in Spain; 50–75 mg/kg, max 2 g, in the US label for non-meningeal infection). WHO caps at 3 g/day.
19. **Pneumonia, benzylpenicillin interval in WHO documents.** WHO 2014 says every 6 hours; the AWaRe book 2022, which cites the 2014 document, says every 8 hours.

## Not verified / limits of this review

- **AAP Red Book**: not opened (subscription). Nothing reported from it.
- **BNF for Children**: HTTP 403 on every attempt. Nothing reported from it.
- **EMA / eMC SmPCs**: not attempted; Spanish AEMPS sheets used instead.
- **AAP 2013 on the publisher's site**: 403. The table was read from a copy of the journal PDF hosted elsewhere; the footer of that copy reads "PEDIATRICS Volume 131, Number 3, March 2013 e983".
- **IDSA 2014 skin and IDSA 2011 MRSA tables, and the CDC pharyngitis page**: read only through the fetch tool (code TOOL). The streptococcal-skin-infection rows of the IDSA 2014 table (penicillin VK, cefalexin, clindamycin for children) could not be attributed to individual drugs and are not reported. The IDSA 2011 statement that TMP-SMX is not recommended under 2 months was seen only in a summary, not as an exact excerpt.
- **IDSA pharyngitis treatment update**: none found; only Part 1 (risk assessment, October 2025) exists as far as two searches and the IDSA page show.
- **Giardiasis**: one regulatory source only; no guideline with a mg ceiling was found.
- **Cefuroxime US label**: the suspension label retrieved is dated 2013 (older listing); current US tablets labels were not checked for the suspension table.
- Values marked "(derived)" are simple arithmetic from the quoted per-dose figure and interval, not statements in the source.
