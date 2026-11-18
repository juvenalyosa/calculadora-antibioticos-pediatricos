# F. INVIMA registry lookup — anti-infectives relevant to childhood infections (Colombia)

Date of retrieval: 2026-11-17. All figures below were computed from INVIMA open data downloaded during this lookup (nothing from memory). The earlier partial extracts in `dl/cum/` and `dl/cum_ren/` were **not** reused; everything was re-queried.

## 1. Source, date, method

| Dataset (datos.gov.co, Socrata) | id | Last data update (UTC) | Rows / registrations |
|---|---|---|---|
| Código Único de Medicamentos **Vigentes** | `i7cb-raxc` | 2026-11-16 12:13 | 156,281 rows / 9,500 expedientes |
| CUM **Vencidos** | `vwwf-4ftk` | 2026-11-16 12:18 | 156,805 rows / 16,683 expedientes |
| CUM **En trámite de renovación** | `vgr4-gemg` | 2026-11-16 12:18 | 115 rows / 18 expedientes |
| CUM **Otros estados** | `spzp-dfuc` | 2026-11-16 12:19 | 114,444 rows / 9,979 expedientes (Cancelado, Pérdida de fuerza ejecutoria, Negado, Desistido, Abandono, Suspendido, Revocado, export-only, and **"Temp. no comerc - Vigente"**) |

Query patterns (repeatable; only generic terms are sent):

```
# 1) ATC sweep, run on each of the 4 datasets, for each prefix
#    J01 P01 P02 P03 D06 S01 S02 S03 J02 J04 J05 A07A A01AB D01 D07C D10AF G01 R01AX
https://www.datos.gov.co/resource/<id>.json?$where=starts_with(atc,'J01')&$limit=50000&$offset=0&$order=:id

# 2) Name sweep (to catch mis-coded ATC), 68 Spanish stems, e.g. DICLOXACILIN, CEFIXIM, NITROFURANTOIN ...
https://www.datos.gov.co/resource/<id>.json?$where=upper(principioactivo) like '%STEM%' OR upper(producto) like '%STEM%' OR upper(descripcionatc) like '%STEM%'&$limit=50000

# 3) Whole-registry sweep of current oral liquid / paediatric solid forms (Vigentes only)
https://www.datos.gov.co/resource/i7cb-raxc.json?$where=viaadministracion='ORAL' AND formafarmaceutica in ('SUSPENSION ORAL','POLVO PARA RECONSTITUIR A SUSPENSION ORAL','POLVO PARA RECONSTITUIR A SOLUCION ORAL','SOLUCION ORAL','JARABE','SUSPENSIONES','ELIXIR','GRANULOS','POLVO','TABLETA DISPERSABLE','TABLETA MASTICABLE', ...)

# metadata (update date): https://www.datos.gov.co/api/views/<id>.json  -> rowsUpdatedAt
```

How the numbers were built:

- A dataset row is *registration (expediente) × CUM presentation × active ingredient × role (manufacturer/importer)*. **"n registrations" = distinct `expediente`**, i.e. distinct sanitary registrations, not pack sizes.
- **Registered now = yes** means at least one registration in the *Vigentes* dataset (or *En trámite de renovación*, which keeps the registration legally alive; only 3 registrations in scope are in that state). Where a registration is Vigente but all its CUM presentations are flagged `Inactivo`, the note says so.
- **TNC** = "Temporalmente no comercializado – Vigente" (in *Otros estados*): the registration is legally valid but the holder has declared the product is not being marketed. Reported separately; it is *not* counted as "registered now".
- **Lapsed** = found only as Vencido / Pérdida de fuerza ejecutoria / Cancelado / Negado / Desistido / Abandono. **Never found** = no row in any of the four datasets, by ATC or by name.
- Strength: `cantidad` + `unidadmedida` per `unidadreferencia`, normalised to mg per 5 mL for oral liquids and mg per vial/ampoule/tablet otherwise, then cross-checked against the product name. The registry often stores the salt weight, grams per bottle, or a malformed unit; in those cases the labelled strength in the product name was used.

Limits (read before using the tables):

1. **A registration does not prove the product is stocked or even marketed.** Many "Vigente" registrations are dormant; conversely TNC products can return.
2. ATC-based queries miss mis-coded products. The name sweep and the oral-liquid sweep were added for that reason. Mis-codings seen among current products: nitrofurantoin 100 mg tablets under V06DD01 (found only by name), furazolidone under G01AX06 and nifuroxazide + metronidazole under A01AB17/G01AF20 (found because G01 and A01AB were swept), amoxicillin-sulbactam under the amoxicillin-clavulanate code. The oral-liquid sweep found **no** current anti-infective oral liquid coded outside the swept groups.
3. Ingredient names in the registry are free text with many misspellings ("cefradilo", "piperaciclina", "nitaxozanida"). Counts for a substance may be off by one or two where a misspelling was not recognised.
4. `fechavencimiento` is empty for most current registrations (1,539 of the 2,107 current registrations in the swept groups; 402 carry the indefinite date 3000-01-01; none has a past date), so expiry dates are given only for lapsed products.
5. The name sweep of the *Otros estados* dataset — see section 13, item 6.
6. Registration numbers are quoted as in the registry except that stray spaces were removed (the registry writes e.g. `INVIMA 2008 M-011328 R1`); search by the numeric core if an exact match fails.

---

## 2. Penicillins

| Substance | Registered now? | n current (TNC) | Forms and strengths currently registered (n) | Example registration | Note |
|---|---|---|---|---|---|
| Phenoxymethylpenicillin (penicillin V) | **yes** | 1 | Powder for oral solution **250 mg/5 mL** (potassium salt) (1) | INVIMA 2021M-0010931-R1 (Penviotina) | Single product. Tablets (500 mg, 1.5 MUI): lapsed — 12 expired + 2 pérdida de fuerza ejecutoria. |
| Procaine penicillin | **yes** | 2 | Vial 400,000 UI (1); 800,000 UI (1) | INVIMA 2021M-006414-R2; INVIMA 2021M-011499-R2 | Fixed combinations procaine + penicillin G sodium/potassium (J01CE30): lapsed (9 expired, 4 cancelled; last 2019). |
| Dicloxacillin | **yes** | 25 (3) | Capsule 500 mg (15); powder for oral suspension **250 mg/5 mL** (10) | INVIMA 2014M-0014902 (caps); INVIMA 2016M-0005129-R1 (susp) | No current 250 mg capsule or 125 mg/5 mL. |
| Oxacillin | **yes** | 5 | Vial 1 g (5) | INVIMA 2020M-0019809 | Injectable only. |
| Cloxacillin | lapsed | 0 | — | — | 3 expired (inj 1 g, caps 500 mg), last 2018. |
| Flucloxacillin | never found | 0 | — | — | No row in any dataset. |
| Ampicillin, oral | **yes** | 24 oral of 30 (4) | Capsule 500 mg (12); tablet 1 g (6); tablet 500 mg (1); powder for oral suspension **250 mg/5 mL** (5) | INVIMA 2019M-0018917 (caps); INVIMA 2017M-0005497-R1 (susp) | Capsule 250 mg only TNC. Injectable (already in calculator): vial 500 mg (3), 1 g (3). |
| Ampicillin-sulbactam IV | **yes** | 9 (3) | Vial 1.5 g = 1 g + 0.5 g (8); vial 3 g = 2 g + 1 g (1) | INVIMA 2018M-0018572; INVIMA 2021M-0020226 (3 g) | |
| Sultamicillin (oral) | **yes** | 22 (3) | Powder for oral suspension **250 mg/5 mL** (5); tablet 375 mg (11); tablet 750 mg (6) | INVIMA 2016M-0016768 (susp); INVIMA 2018M-006229-R2 (750 mg) | |
| Amoxicillin-sulbactam, oral | **yes** | 4 (1) | Powder for oral suspension **amoxicillin 1000 mg + sulbactam 250 mg per 5 mL** (2); tablet 875 mg + 125 mg (2) | INVIMA 2019M-0009492-R1; INVIMA 2021M-0015448-R1 (susp); INVIMA 2021M-0004136-R2 (tabs) | Coded J01CR02, the same ATC as amoxicillin-clavulanate — easy to miss. |
| Amoxicillin-sulbactam, IV | lapsed | 0 | — | — | 1 expired (1 g + 0.5 g, 2018), 2 cancelled. |
| Amoxicillin-clavulanate, IV | **no current marketed registration** | 0 (1) | TNC only: vial 500 mg + 100 mg (1) | INVIMA 2021M-0020259 (TNC) | Lapsed: 10 expired + 4 other (1 g + 200 mg; 500 mg + 100 mg). The last two marketed registrations (INVIMA 2021M-0011301-R2, 500/100 mg, and INVIMA 2021M-0011374-R2, 1 g/200 mg) are listed as Vencido with expiry 2026-08-04, i.e. three months before this lookup — re-check, a renewal could still be recorded. |
| Piperacillin-tazobactam | **yes** | 15 (2) | Vial 4 g + 0.5 g (15) | INVIMA 2017M-0017589 | No 2 g + 0.25 g vial currently registered. |

## 3. Cephalosporins

| Substance | Registered now? | n current (TNC) | Forms and strengths currently registered (n) | Example registration | Note |
|---|---|---|---|---|---|
| Cefadroxil | **yes** | 11 (3) | Capsule 500 mg (6); powder for oral suspension **250 mg/5 mL** (2) and **500 mg/5 mL** (1); tablet 1 g (2, one prolonged-release) | INVIMA 2015M-0003515-R1 (caps); INVIMA 2021M-0014229-R1 (250/5); INVIMA 2023M-0006480-R2 (500/5) | 125 mg/5 mL only TNC. A 12th product (INVIMA 2020M-0019595, oral suspension 250 mg/5 mL, ATC J01DB05) lists its ingredient as "cefradilo monohidrato" — probably cefadroxil, see section 9. |
| Cefradine | **yes** | 18 (2) | Capsule 500 mg (11); tablet 500 mg (3); tablet 1 g (1); powder for oral suspension/solution **250 mg/5 mL** (2); vial 1 g (1) | INVIMA 2009M-009670-R2 (caps); INVIMA 2019M-0009641-R1 (susp) | Excludes the ambiguous product above. |
| Cefazolin | **yes** | 10 | Vial 1 g (10) | INVIMA 2013M-0014666 | |
| Cefaclor | lapsed | 0 | — | — | 21 expired (caps, suspensions 125/250/375 mg per 5 mL), last 2011. |
| Cefprozil | lapsed | 0 | — | — | 4 cancelled / pérdida de fuerza ejecutoria (susp 250 mg/5 mL, tabs 250/500 mg), last 2019. |
| Cefixime | lapsed | 0 | — | — | 24 expired; suspension 100 mg/5 mL last 2017, tablet 400 mg last 2021. |
| Ceftibuten | **yes** | 1 (2) | Powder for oral suspension **180 mg/5 mL** (1) | INVIMA 2019M-0019357 | TNC: a second suspension and capsule 400 mg. |
| Cefotaxime | **yes** | 2 | Vial 1 g (2) | INVIMA 2021M-0020201 | Vial 500 mg lapsed. |
| Ceftazidime | **yes** | 1 | Vial 1 g (1) | INVIMA 2020M-010595-R2 | Only one registration left (19 lapsed). Ceftazidime-avibactam: 2 current. |
| Cefepime | **yes** | 11 (2) | Vial 1 g (11) | INVIMA 2017M-0017875 | Vial 2 g lapsed. |

## 4. Macrolides and lincosamides

| Substance | Registered now? | n current (TNC) | Forms and strengths currently registered (n) | Example registration | Note |
|---|---|---|---|---|---|
| Erythromycin (systemic) | **yes** | 4 (2) | Oral suspension **250 mg/5 mL, estolate** (1); tablet 500 mg estolate (1); tablet 500 mg stearate (1); tablet 600 mg stearate (1) | INVIMA 2023M-008266-R4 (susp) | **No ethylsuccinate product is currently registered.** One suspension only (21 suspensions lapsed). TNC: 2 × stearate 500 mg tablets. Topical acne forms 2–4%: 5. |
| Spiramycin | **yes** | 1 | Tablet 3 MUI (1) | INVIMA 2015M-0004583-R1 | No liquid. |
| Clindamycin, injectable | **yes** | 7 (4) | Ampoule 600 mg/4 mL (6); 300 mg/2 mL (1) (150 mg/mL) | INVIMA 2017M-0006016-R1 | |
| Clindamycin, oral liquid | **never found** | 0 | — | — | No oral liquid in any dataset. Capsule 300 mg: 11 current (already in calculator). |
| Lincomycin | **yes** | 10 (2) | Ampoule 600 mg/2 mL (9); 300 mg/1 mL (1) | INVIMA 2015M-0016493 | Injectable only; no oral form current. |

## 5. Aminoglycosides (injectable)

| Substance | Registered now? | n current (TNC) | Forms and strengths currently registered (n) | Example registration | Note |
|---|---|---|---|---|---|
| Gentamicin | **yes** | 9 (1) | Ampoule 160 mg/2 mL (5); 80 mg/2 mL (3); 40 mg/1 mL (1) | INVIMA 2019M-009484-R3 | Paediatric 20 mg/2 mL and 120 mg/2 mL: lapsed. 147 injectable registrations have lapsed. |
| Amikacin | **yes** | 5 | Ampoule 500 mg/2 mL (4); 100 mg/2 mL (1) | INVIMA 2020M-0001551-R2; INVIMA 2022M-014889-R3 (100 mg) | 250 mg/2 mL and 1 g/4 mL: lapsed. |
| Tobramycin | lapsed (injectable) | 0 | — | — | Injectable 40 mg/mL expired 2007. Inhalation 300 mg (2 current) and ophthalmic forms exist (section 9). |

## 6. Quinolones and urinary agents

| Substance | Registered now? | n current (TNC) | Forms and strengths currently registered (n) | Example registration | Note |
|---|---|---|---|---|---|
| Nalidixic acid | lapsed | 0 | — | — | Suspension 250 mg/5 mL: negado / pérdida de fuerza ejecutoria (2022); tablet 500 mg last 2020. |
| Norfloxacin | **yes** | 11 (1) | Tablet/capsule 400 mg (11) | INVIMA 2016M-0005331-R1 | No liquid. |
| Levofloxacin | **yes** | 30 (3) | Tablet 500 mg (16); 750 mg (10); IV 500 mg/100 mL (3); ophthalmic 0.5% (1) | INVIMA 2018M-0014489-R1; INVIMA 2014M-0015067 (IV) | Oral liquid: never found. No 250 mg tablet current. |
| Moxifloxacin | **yes** | 19 (4) | Tablet 400 mg (10); IV 400 mg/250 mL (3); ophthalmic 0.5% (6) | INVIMA 2016M-0012877-R1; INVIMA 2019M-0000712-R2 (IV) | |
| Nitrofurantoin | **yes** | 6 | Capsule 100 mg (4; at least 3 state macrocrystals); tablet 100 mg (2) | INVIMA 2022M-0011106-R2; INVIMA 2020M-0008104-R1 (tablet, mis-coded V06DD01) | **No oral liquid and no 50 mg strength currently registered.** Suspension 25 mg/5 mL (Macrodantina infantil) and 50 mg capsules: pérdida de fuerza ejecutoria. |
| Fosfomycin | **yes** | 6 (4) | Oral sachet 3 g (4); capsule 500 mg (1); IV vial 4 g (1) | INVIMA 2015M-0003562-R1 (sachet); INVIMA 2019M-0015204-R1 (IV) | Paediatric 2 g sachet expired 2008. TNC: 4 more 3 g sachets. |

## 7. Other systemic agents

| Substance | Registered now? | n current (TNC) | Forms and strengths currently registered (n) | Example registration | Note |
|---|---|---|---|---|---|
| Vancomycin, injectable | **yes** | 11 (2) | Vial 500 mg (9); 1 g (2) | INVIMA 2010M-0010494; INVIMA 2023M-0018274-R1 (1 g) | Oral capsules 125 mg (1) / 250 mg (2) already in calculator. |
| Linezolid | **yes** | 17 (1) | IV 2 mg/mL, 600 mg/300 mL bag (11); tablet 600 mg (6) | INVIMA 2017M-0012334-R1 (IV); INVIMA 2019M-0013724-R1 (tabs) | **Oral suspension: lapsed** (100 mg/5 mL granules, pérdida de fuerza ejecutoria). |
| Metronidazole, IV | **yes** | 4 | Infusion 500 mg/100 mL (4) | INVIMA 2022M-0003878-R2 | |
| Rifampicin | **yes** | 2 (1) | Capsule 300 mg (2) | INVIMA 2008M-008802-R2 | **Oral liquid lapsed** (100 mg/5 mL, last 2012). IV 600 mg: TNC only. |
| Chloramphenicol | **yes** | 1 (1) | Capsule 250 mg (1) | INVIMA 2021M-0007778-R1 | Injectable 1 g: TNC only. Oral suspension lapsed (2019); ophthalmic lapsed (2015). |
| Tetracycline | lapsed | 0 | — | — | Tetracycline HCl capsules 250/500 mg: expired/cancelled. The two current J01AA "tetraciclina" hits are lymecycline 150/300 mg capsules. |
| Minocycline | **yes** | 2 | Capsule 100 mg (2) | INVIMA 2021M-0007991-R1 | |
| Meropenem | **yes** | 22 (3) | Vial 500 mg (10); 1 g (12) | INVIMA 2019M-0007641-R1 | |
| Ertapenem | **yes** | 6 | Vial 1 g (6) | INVIMA 2019M-0002061-R2 | |
| Colistin (colistimethate) | **yes** | 3 (3) | Vial 150 mg colistin base (3) | INVIMA 2020M-0019591 | TNC: 80 mg, 1 MUI, 2 MUI vials. Polymyxin B 500,000 UI vial: 9 current. |

## 8. Antiprotozoals and anthelmintics (P01, P02)

| Substance | Registered now? | n current (TNC) | Forms and strengths currently registered (n) | Example registration | Note |
|---|---|---|---|---|---|
| Tinidazole | **yes** | 14 (2) | Oral suspension **1 g/5 mL (200 mg/mL)** (2); tablet 500 mg (9); 1 g (3) | INVIMA 2007M-0007223 (susp) | |
| Secnidazole | **yes** | 27 (6) | Paediatric powder for oral suspension (2; 3 g and 3.33 g per bottle); tablet 500 mg (14); 1 g (11) | INVIMA 2022M-0005808-R2; INVIMA 2016M-0005413-R1 (powders) | mg per 5 mL not derivable from the registry (see section 11). Also combined with fluconazole or itraconazole (tablets). |
| Ornidazole | lapsed | 0 | — | — | Expired 2009. |
| Nitazoxanide | **yes** | 71 (6) | Powder/granules/suspension **100 mg/5 mL** (26); tablet 500 mg (42); soft capsule 500 mg (2); **dispersible tablet 200 mg** (1) | INVIMA 2019M-0003257-R2 (susp); INVIMA 2019M-0008733-R1 (dispersible) | Largest group in scope. |
| Albendazole | **yes** | 25 (5) | Oral suspension **100 mg/5 mL (2%)** (10); **200 mg/5 mL (4%, 400 mg/10 mL)** (4); tablet 200 mg (10); tablet 400 mg (1) | INVIMA 2010M-0010573; INVIMA 2020M-0007124-R1 (4%) | |
| Mebendazole | **yes** | 2 (1) | Oral suspension **100 mg/5 mL** (2) | INVIMA 2015M-0003809-R1 | Tablet 100 mg: TNC only (25 expired). |
| Pyrantel (pamoate) | **yes** | 10 | Oral suspension **250 mg/5 mL** (6); tablet 250 mg (3); soft capsule 250 mg (1) | INVIMA 2016M-003092-R4 | Plus pyrantel + oxantel: suspension 250 + 250 mg/5 mL (3) and tablet (1), e.g. INVIMA 2020M-0007003-R1. |
| Ivermectin | **yes** (oral liquid only) | 7 oral (5) | Oral solution/suspension **6 mg/mL (0.6% drops)** (7) | INVIMA 2015M-0016032 | **No tablet currently registered** (tablets 3/6 mg expired, denied or withdrawn; soft capsule 3 mg TNC). Topical 1% cream / 0.1% lotion: 3. |

## 9. Topical, ophthalmic and otic anti-infectives (D06, S01, S02, S03)

| Substance / product type | Registered now? | n current (TNC) | Forms and strengths currently registered (n) | Example registration | Note |
|---|---|---|---|---|---|
| Fusidic acid, skin | **yes** | 14 (3) | Cream 2% (13); ointment 2% (1) | INVIMA 2014M-0015168 | Also fusidic acid + betamethasone (1), + hydrocortisone (1). |
| Fusidic acid, eye | yes (dormant) | 1 | Viscous eye drops 1% (1) | INVIMA 2019M-010922-R3 | Vigente but all CUM presentations inactive. |
| Retapamulin | lapsed | 0 | — | — | Ointment 1%, expired 2018. |
| Silver sulfadiazine | **yes** | 11 (2) | Cream 1% (10 + 1 in renewal) | INVIMA 2016M-0011800-R1 | |
| Tobramycin, eye | **yes** | 4 (1) | Eye drops 0.3% (4) | INVIMA 2019M-0009892-R1 | Ointment lapsed (2017). Tobramycin + dexamethasone: drops 0.3%/0.1% (7), ointment (4). |
| Gentamicin, eye | **yes** | 12 (1) | Eye drops 0.3% (9); eye ointment 0.3% (2 + 1 in renewal) | INVIMA 2015M-0016112 | Skin cream 0.1%: 3. |
| Ciprofloxacin, eye | **yes** | 7 (5) | Eye drops 0.3% (7) | INVIMA 2019M-012285-R2 | Plain eye ointment: none. With dexamethasone: drops (4), ointment (1). |
| Moxifloxacin, eye | **yes** | 6 (1) | Eye drops 0.5% (6) | INVIMA 2014M-0003113-R1 | With dexamethasone: 3. |
| Sulfacetamide, eye | **yes** | 1 | Eye drops 10% (1) | INVIMA 2016M-0011463-R1 | 19 lapsed. |
| Chloramphenicol, eye | lapsed | 0 | — | — | Drops 0.5% expired 2015; ointment 2007. |
| Erythromycin eye ointment | lapsed | 0 | — | — | 0.5% ointment expired 2020. Alternative currently registered: oxytetracycline + polymyxin B eye ointment (3), e.g. INVIMA 2023M-0018510-R1. |
| Ciprofloxacin, ear, alone | **yes** | 1 (2) | Ear drops 0.3% (1) | INVIMA 2019M-013358-R2 | |
| Ciprofloxacin + corticosteroid, ear | **yes** | 6 | + hydrocortisone 1% (2); + dexamethasone 0.1% (2); + fluocinolone 0.025% (1); + betamethasone + lidocaine (1) | INVIMA 2023M-0000361-R3; INVIMA 2017M-0012007-R1; INVIMA 2019M-0015073-R1 | |
| Polymyxin-neomycin ear combinations | **yes** | 5 (1) | Neomycin + polymyxin B + betamethasone or dexamethasone + lidocaine, ear drops (3); neomycin + colistin + hydrocortisone, ear suspension (2) | INVIMA 2019M-0009902-R1; INVIMA 2016M-0011730-R1 | Eye: neomycin + polymyxin B + dexamethasone drops/ointment (≈20). |

## 10. Antivirals and antifungals (context)

| Substance | Registered now? | n current (TNC) | Forms and strengths currently registered (n) | Example registration | Note |
|---|---|---|---|---|---|
| Oseltamivir | **yes** | 1 (1) | Capsule 75 mg (1) | INVIMA 2017M-014651-R2 | **Suspension lapsed** (expired 2016); 30 mg and 45 mg capsules lapsed/denied. Only one marketed registration. |
| Aciclovir | **yes** | 52 total | Oral suspension **100 mg/5 mL** (1) and **200 mg/5 mL** (2); tablet 200 mg (8), 400 mg (3), 800 mg (13); IV vial 250 mg (1); skin 5% (22); eye ointment 3% (1); vaginal cream 5% (1) | INVIMA 2016M-003061-R2 (100/5); INVIMA 2025M-0021891 (200/5); INVIMA 2022M-0003840-R2 (IV) | |
| Nystatin | **yes** | 11 total | Oral suspension **100,000 UI/mL** (6); tablet 500,000 UI (2); topical/vaginal (3) | INVIMA 2016M-0005332-R1 | |
| Fluconazole | **yes** | 36 (2) | Powder for oral suspension **50 mg/5 mL** (2); capsule 150 mg (16), 200 mg (14); IV 200 mg/100 mL (4) | INVIMA 2016M-0017527 (susp) | No 50 mg or 100 mg capsule current. |

## 11. Re-confirmation: amoxicillin-clavulanate oral and amoxicillin suspensions

Amoxicillin-clavulanate, oral (52 current registrations; 7 TNC):

| Presentation | Ratio | n current | Example registration |
|---|---|---|---|
| Powder for suspension **250 mg + 62.5 mg per 5 mL** | 4:1 | 6 | INVIMA 2018M-0018551; INVIMA 2020M-010184-R2 |
| Powder for suspension **400 mg + 57 mg per 5 mL** | 7:1 | 12 | INVIMA 2018M-0018608; INVIMA 2021M-010635-R2 |
| Powder for suspension **600 mg + 42.9 mg per 5 mL** | 14:1 | 7 | INVIMA 2020M-0019588; INVIMA 2021M-0002672-R2 |
| Tablet **500 mg + 125 mg** | 4:1 | 11 | INVIMA 2020M-010253-R3 |
| Tablet **875 mg + 125 mg** | 7:1 | 16 | INVIMA 2017M-0017689 |
| Tablet 1000 mg + 62.5 mg (prolonged release) | 16:1 | **never found** | — |
| 125 mg + 31.25 mg per 5 mL; 200 mg + 28.5 mg per 5 mL; dispersible tablets | — | lapsed | — |

All three suspension ratios (4:1, 7:1, 14:1) therefore exist. The calculator currently covers only 14:1 (7 registrations); 7:1 is the most common (12).

Amoxicillin alone (72 current registrations): powder for oral suspension/solution **125 mg/5 mL (3), 250 mg/5 mL (25), 500 mg/5 mL (1), 750 mg/5 mL (2)**; capsule 500 mg (32); tablet 500 mg (1), 875 mg (5), 1 g (3). Examples: INVIMA 2020M-011372-R2 (125), INVIMA 2008M-011328-R1 (250), INVIMA 2019M-0019164 (500), INVIMA 2024M-0021243 (750). No 400 mg/5 mL or 1 g/5 mL amoxicillin-only suspension is registered.

## 12. Registered in Colombia with a paediatric oral form but NOT in the calculator

From the substances on the check-list:

1. **Amoxicillin-clavulanate 7:1 (400/57 mg per 5 mL)** — 12 registrations; and **4:1 (250/62.5 mg per 5 mL)** — 6.
2. **Dicloxacillin** suspension 250 mg/5 mL — 10 (plus 500 mg capsules, 15).
3. **Ampicillin** suspension 250 mg/5 mL — 5.
4. **Sultamicillin** suspension 250 mg/5 mL — 5.
5. **Amoxicillin-sulbactam** suspension 1000 + 250 mg per 5 mL — 2.
6. **Phenoxymethylpenicillin** oral solution 250 mg/5 mL — 1.
7. **Cefadroxil** suspension 250 mg/5 mL (2–3) and 500 mg/5 mL (1).
8. **Cefradine** suspension 250 mg/5 mL — 2.
9. **Ceftibuten** suspension 180 mg/5 mL — 1.
10. **Erythromycin estolate** suspension 250 mg/5 mL — 1.
11. **Nitazoxanide** suspension 100 mg/5 mL — 26; dispersible tablet 200 mg — 1.
12. **Albendazole** suspension 100 mg/5 mL — 10; 200 mg/5 mL — 4.
13. **Mebendazole** suspension 100 mg/5 mL — 2.
14. **Pyrantel pamoate** suspension 250 mg/5 mL — 6.
15. **Tinidazole** suspension 1 g/5 mL — 2.
16. **Secnidazole** paediatric powder for suspension — 2.
17. **Ivermectin** oral drops 6 mg/mL — 7.
18. **Aciclovir** suspension 100 and 200 mg/5 mL — 3; **nystatin** suspension 100,000 UI/mL — 6; **fluconazole** suspension 50 mg/5 mL — 2.

Substances that were **not on the check-list** but have a current oral liquid / paediatric form:

- Within J01: **none**. Every J01 oral liquid currently registered belongs to a substance already listed (the only other one is trimethoprim-sulfamethoxazole, already in the calculator: 40/200 mg per 5 mL ×5 and 80/400 mg per 5 mL ×9).
- **Nifuroxazide + metronidazole** suspension 200 + 250 mg/5 mL — 12 (ATC A01AB17/G01AF20/P01AB01).
- **Furazolidone** suspension 50 mg/15 mL — 3 (ATC G01AX06).
- **Pyrantel + oxantel** suspension 250 + 250 mg/5 mL — 3.
- **Flubendazole** suspension 100 mg/5 mL — 2.
- **Rifaximin** suspension 100 mg/5 mL — 3 (A07AA11).
- **Itraconazole** oral solution 10 mg/mL — 2; **ketoconazole** suspension 100 mg/5 mL — 1.
- **Niclosamide** chewable tablet 500 mg — 1; **artemether-lumefantrine** dispersible tablet 20/120 mg — 1.
- Not relevant to the calculator but present: valganciclovir oral solution, methisoprinol syrup, antiretroviral liquids (abacavir, lamivudine, zidovudine, nevirapine, raltegravir, lopinavir/ritonavir). Piperazine syrup and chloroquine oral solution are TNC only.

Other systemic J01 substances currently registered that are on neither list (no paediatric oral form): cefalotin 1 g (6), cefditoren tablets (2), ceftaroline, ceftazidime-avibactam, ceftolozane-tazobactam, aztreonam, imipenem-cilastatin (6), doripenem, tigecycline (6), lymecycline, polymyxin B (9), daptomycin (8), pipemidic acid tablets (2), sulfadiazine tablets (1), sulfamethizole (1), delafloxacin (1).

Gaps worth knowing when choosing what to add (on the check-list but **not** available as a registered paediatric oral form): cefixime, cefaclor, cefprozil (all lapsed); clindamycin oral liquid (never registered); linezolid suspension, rifampicin suspension, nitrofurantoin suspension, oseltamivir suspension, nalidixic acid suspension, chloramphenicol suspension (all lapsed); levofloxacin oral liquid (never found); ivermectin tablets (lapsed); erythromycin ethylsuccinate (none current).

## 13. What could not be determined

1. **Secnidazole paediatric suspension strength per 5 mL.** The registry gives grams of secnidazole per bottle of powder (3 g / 30.08 g; 3.333 g / 48.28 g) with no reconstituted volume in `unidadreferencia`. Query that returned this: ATC sweep `starts_with(atc,'P01')` on `i7cb-raxc`.
2. **Identity of INVIMA 2020M-0019595 (oral suspension 250 mg/5 mL, ATC J01DB05).** Ingredient text reads "cefradilo monohidrato"; the ATC is cefadroxil's, the spelling is closer to cefradine. Counted in neither total.
3. **Whether "penicilina G procaínica 800,000 UI" is plain procaine penicillin or a procaine + sodium penicillin mixture.** Only a procaine ingredient row is present.
4. **Actual availability / stock** of any product, and whether "Vigente" products are marketed. Not answerable from the registry. Products with a single registration are the most fragile: penicillin V solution, ceftibuten, ceftazidime, erythromycin suspension, spiramycin, chloramphenicol, oseltamivir, sulfacetamide, plain otic ciprofloxacin.
5. **Expiry dates of current registrations**: `fechavencimiento` is blank for most (limit 4 in section 1).
6. **Otros estados name sweep**: NAME_SWEEP_STATUS_PLACEHOLDER
