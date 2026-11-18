"""Incorpora a data/fuente lo que la revisión de alcance encontró y la jerarquía
del protocolo selecciona (docs/revision/decisiones_integracion.md).

Uso: python3 docs/revision/integrar.py

Es idempotente: no vuelve a anexar filas que ya estén. Todo entra en estado
«borrador», con su fuente y página tal como aparecen en esquemas_G*.csv y
maximos_G*.csv (cita textual en esos archivos).
"""
import csv
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent.parent
FUENTE = REPO / "data" / "fuente"


def leer(nombre):
    with open(FUENTE / f"{nombre}.csv", encoding="utf-8", newline="") as archivo:
        return list(csv.reader(archivo))


def escribir(nombre, filas):
    with open(FUENTE / f"{nombre}.csv", "w", encoding="utf-8", newline="") as archivo:
        csv.writer(archivo, lineterminator="\n").writerows(filas)


def anexar(nombre, nuevas, clave):
    """Anexa las filas cuya clave (función de la fila) no esté ya."""
    filas = leer(nombre)
    encabezado = filas[0]
    existentes = {clave(dict(zip(encabezado, f))) for f in filas[1:]}
    agregadas = 0
    for fila in nuevas:
        fila = [str(c) for c in fila]
        assert len(fila) == len(encabezado), (nombre, fila[:3])
        if clave(dict(zip(encabezado, fila))) not in existentes:
            filas.append(fila)
            agregadas += 1
    escribir(nombre, filas)
    print(f"{nombre}: +{agregadas}")


def modificar_nota(nombre, filtro, agregado):
    filas = leer(nombre)
    encabezado = filas[0]
    i = encabezado.index("nota")
    for fila in filas[1:]:
        if filtro(dict(zip(encabezado, fila))) and agregado not in fila[i]:
            fila[i] = (fila[i] + " " + agregado).strip()
    escribir(nombre, filas)


# --- Fuentes nuevas ----------------------------------------------------------
CPS = "cps_meningitis_2014"
NICE_NG111 = "nice_ng111_2018"
SUIZA = "consenso_suizo_itu_2021"
SAP_PIEL = "sap_piel_2014"
CHILE_FAA = "sochinf_faringoamigdalitis_2019"
SADI = "sadi_neumonia_complicada_2024"
AAP_OMA = "aap_oma_2013"
SOCHINF_ITU = "sochinf_itu_2022"
AUSTRALIA_OM = "australia_otitis_media_2021"

anexar("fuentes", [
    [CPS, "Guidelines for the management of suspected and confirmed bacterial meningitis in Canadian children older than one month of age",
     "Canadian Paediatric Society (Paediatr Child Health 19(3):141-146)", 2014, "https://doi.org/10.1093/pch/19.3.141"],
    [NICE_NG111, "Pyelonephritis (acute): antimicrobial prescribing (NG111)",
     "National Institute for Health and Care Excellence (Reino Unido)", 2018, "https://www.nice.org.uk/guidance/ng111"],
    [SUIZA, "Swiss consensus recommendations on urinary tract infections in children",
     "Grupo de consenso suizo (Eur J Pediatr)", 2021, "https://doi.org/10.1007/s00431-020-03714-4"],
    [SAP_PIEL, "Infecciones de piel y partes blandas en pediatría: consenso sobre diagnóstico y tratamiento",
     "Sociedad Argentina de Pediatría, Comité Nacional de Infectología (Arch Argent Pediatr 112(2):183-191)", 2014,
     "https://doi.org/10.5546/aap.2014.183"],
    [CHILE_FAA, "Recomendaciones para el diagnóstico y tratamiento etiológico de la faringoamigdalitis aguda estreptocócica en pediatría",
     "Sociedad Chilena de Infectología (Rev Chilena Infectol 36(1):69-77)", 2019,
     "https://doi.org/10.4067/s0716-10182019000100069"],
    [SADI, "Recomendaciones sobre neumonías complicadas en pediatría",
     "Sociedad Argentina de Infectología (Actualizaciones en Sida e Infectología 32(116))", 2024,
     "https://doi.org/10.52226/revista.v32i116.345"],
    [AAP_OMA, "The Diagnosis and Management of Acute Otitis Media",
     "American Academy of Pediatrics (Pediatrics 131(3):e964-e999)", 2013, "https://doi.org/10.1542/peds.2012-3488"],
    [SOCHINF_ITU, "Recomendaciones para el diagnóstico y tratamiento de la infección del tracto urinario en pediatría (parte 1)",
     "Sociedad Chilena de Infectología (Rev Chilena Infectol 39(2))", 2022, ""],
    [AUSTRALIA_OM, "Otitis media guidelines for Australian Aboriginal and Torres Strait Islander children",
     "Grupo de guías de otitis media (Med J Aust)", 2021, "https://doi.org/10.5694/mja2.50953"],
], clave=lambda f: f["id"])

# --- Medicamentos nuevos (todos con registro INVIMA vigente; informe F) -------
B = "borrador"
anexar("medicamentos", [
    ["ampicilina_sulbactam", "Ampicilina-sulbactam", "mg", "sin_dato", "", "", B],
    ["cefotaxima", "Cefotaxima", "mg", "sin_dato", "", "", B],
    ["levofloxacina", "Levofloxacina", "mg", "sin_dato", "", "", B],
    ["vancomicina_iv", "Vancomicina (intravenosa)", "mg", "sin_dato",
     "Se elimina por el riñón: la dosis se ajusta según los niveles séricos y la función renal.", "", B],
    ["linezolid", "Linezolid", "mg", "no_requiere", "", "kinderformularium", B],
    ["cefadroxilo", "Cefadroxilo", "mg", "sin_dato", "", "", B],
], clave=lambda f: f["id"])
anexar("presentaciones", [
    ["cefadroxilo_susp_250", "cefadroxilo", "Suspensión 250 mg/5 mL", "oral", "liquido", 250, 5, "", "", ""],
    ["cefadroxilo_susp_500", "cefadroxilo", "Suspensión 500 mg/5 mL", "oral", "liquido", 500, 5, "", "", ""],
    ["cefadroxilo_cap_500", "cefadroxilo", "Cápsula 500 mg", "oral", "solido", 500, "", "cápsula", "no", ""],
    ["linezolid_tab_600", "linezolid", "Tableta 600 mg", "oral", "solido", 600, "", "tableta", "no", ""],
], clave=lambda f: f["id"])

# --- Topes por medicamento y vía (dosis máxima; maximos_G*.csv) ---------------
FIN, PIDS = "finlandia_vri_2023", "pids_idsa_neumonia_2011"
anexar("topes_medicamento", [
    ["amoxicilina", "oral", "", 4000, FIN, "Tabla 3 (p. 253)", "Coincide con PIDS/IDSA 2011, tabla 7."],
    ["amoxicilina_clavulanato", "oral", "", 4000, PIDS, "Tabla 7", "En amoxicilina."],
    ["amoxicilina_clavulanato_7_1", "oral", "", 4000, PIDS, "Tabla 7",
     "En amoxicilina. PIDS/IDSA da el máximo para la amoxicilina-clavulanato sin precisar la relación."],
    ["ampicilina", "iv", "", 12000, CPS, "Tabla 2", ""],
    ["ceftriaxona", "iv", "", 4000, FIN, "Tabla 3 (p. 253)",
     "Igual en la Sociedad Canadiense de Pediatría 2014 y en NICE NG111. La OMS (AWaRe 2022) da 3 g en niños."],
    ["ceftriaxona", "im", "", 4000, NICE_NG111, "Tabla 3", "NICE lo da para la vía intravenosa o intramuscular."],
    ["cefotaxima", "iv", "", 12000, CPS, "Tabla 2", "La guía da 8 a 12 g al día."],
    ["cefuroxima_sodica", "iv", "", 6000, FIN, "Tabla 3 (p. 253)", ""],
    ["cefuroxima", "oral", "", 1000, SUIZA, "Tabla 2", "Dosis máxima diaria de SwissPedDose."],
    ["cefalexina", "oral", 1000, 4000, NICE_NG111, "Tabla 3", "Máximo para infecciones graves."],
    ["clindamicina", "oral", 450, "", "idsa_piel_2014", "Tabla 2", "Dosis de adulto de la misma tabla (300 a 450 mg cuatro veces al día)."],
    ["clindamicina", "iv", 900, "", SAP_PIEL, "p. 188", "Dosis de adulto (900 mg cada 8 horas)."],
    ["azitromicina", "oral", "", 500, FIN, "Tabla 3 (p. 253)", "Igual en PIDS/IDSA 2011."],
    ["claritromicina", "oral", "", 1000, FIN, "Tabla 3 (p. 253)", "Igual en PIDS/IDSA 2011."],
    ["doxiciclina", "oral", 100, 200, FIN, "Tabla 3 (p. 253)", "El máximo por toma es de la guía china de micoplasma (p. 7)."],
    ["metronidazol", "oral", 500, "", "idsa_shea_cdifficile_2017", "Tabla 2", ""],
    ["metronidazol", "iv", 500, "", "idsa_shea_cdifficile_2017", "Tabla 2", ""],
    ["vancomicina", "oral", 500, "", "idsa_shea_cdifficile_2017", "Tabla 2", "Máximo de la enfermedad grave; en la no grave es 125 mg."],
    ["ciprofloxacina", "oral", "", 1500, "francia_gastroenteritis_2017", "Tabla 1 (p. S28)", ""],
    ["ciprofloxacina", "iv", 400, "", "fda_fichas_tecnicas", "Ficha técnica de ciprofloxacina intravenosa", ""],
    ["trimetoprim_sulfametoxazol", "oral", 320, "", "idsa_piel_2014", "Tabla 2", "En trimetoprim; dosis de adulto (1 a 2 tabletas dobles)."],
    ["nitrofurantoina", "oral", 100, "", SUIZA, "Tabla 2", ""],
    ["amikacina", "iv", "", 1500, SUIZA, "Tabla 2", "El consenso colombiano de infección urinaria da 1 g al día."],
    ["penicilina_g", "iv", "", 24000000, FIN, "Tabla 3 (p. 253)", "Igual en la Sociedad Canadiense de Pediatría 2014."],
    ["cefpodoxima", "oral", "", 400, SUIZA, "Tabla 2", ""],
    ["linezolid", "iv", 600, "", SAP_PIEL, "Figura 4 (p. 185)", ""],
    ["linezolid", "oral", 600, "", SAP_PIEL, "Figura 4 (p. 185)", ""],
    ["levofloxacina", "iv", "", 750, PIDS, "Tabla 7", ""],
], clave=lambda f: (f["medicamento_id"], f["via"]))

# --- Enfermedades nuevas ------------------------------------------------------
anexar("enfermedades", [
    ["meningitis_bacteriana", "Meningitis bacteriana: tratamiento empírico (hospital)", "Fiebre y enfermedad grave",
     "si", "", "oms_aware_2022"],
    ["neumonia_complicada", "Neumonía complicada: derrame pleural o empiema (hospital)", "Respiratorias bajas",
     "si", "", SADI],
    ["rinosinusitis_hospitalizada", "Rinosinusitis grave que requiere hospitalización", "Respiratorias altas",
     "si", "", "acorl_rinosinusitis_2024"],
    ["piel_sarm_grave", "Infección grave de piel por S. aureus resistente a meticilina (hospital)",
     "Piel y tejidos blandos", "si", "", "acin_piel_2023"],
    ["yersiniosis", "Diarrea por Yersinia", "Gastrointestinales", "si", "", "francia_gastroenteritis_2017"],
], clave=lambda f: f["id"])


# --- Esquemas -----------------------------------------------------------------
def esquema(enfermedad, linea, opcion, descripcion, medicamento, via, base, dosis, intervalo,
            fuente, pagina, *, max_dosis="", max_dia="", dias=("", ""), edad=("", ""), nota=""):
    if isinstance(dosis, (int, float)):
        dosis = (dosis, dosis, dosis)
    return [enfermedad, linea, opcion, descripcion, medicamento, via, base, *dosis, intervalo,
            max_dosis, max_dia, *dias, *edad, "", "", nota, fuente, pagina, "borrador", "", ""]


HOSPITAL = "Uso hospitalario."
AWARE = "oms_aware_2022"
ACORL_RS, ACIN = "acorl_rinosinusitis_2024", "acin_piel_2023"
GASTRO, CDI = "francia_gastroenteritis_2017", "idsa_shea_cdifficile_2017"

anexar("esquemas", [
    # Meningitis bacteriana: OMS primero (jerarquía del protocolo); Canadá como alternativa.
    esquema("meningitis_bacteriana", "primera", 1, "Ceftriaxona (OMS)", "ceftriaxona", "iv", "mg_kg_dosis",
            100, 24, AWARE, "Tabla 26.4 (p. 358) y tabla 50.1 (p. 645)", max_dia=3000, dias=(10, 10),
            edad=(1, ""),
            nota=HOSPITAL + " 10 días si no se conoce el germen (meningococo 5 a 7; neumococo 10 a 14). "
                 "Tope de 3 g al día de la OMS para niños. El lineamiento colombiano de enfermedad "
                 "meningocócica (2018) escribe «100 mg/kg cada 12 horas» sin aclarar si es por día o por "
                 "dosis: no se cargó hasta aclararlo."),
    esquema("meningitis_bacteriana", "primera", 2, "Cefotaxima (OMS)", "cefotaxima", "iv", "mg_kg_dosis",
            50, 8, AWARE, "Tabla 26.4 (p. 358)", dias=(10, 10), edad=(1, ""), nota=HOSPITAL),
    esquema("meningitis_bacteriana", "segunda", 1, "Ceftriaxona más vancomicina (Sociedad Canadiense de Pediatría)",
            "ceftriaxona", "iv", "mg_kg_dia", 100, 12, CPS, "Tablas 1 y 2", max_dia=4000, edad=(1, ""),
            nota=HOSPITAL + " Recomendado como tratamiento empírico en Canadá, donde se cubre el neumococo "
                 "resistente a cefalosporinas."),
    esquema("meningitis_bacteriana", "segunda", 1, "Ceftriaxona más vancomicina (Sociedad Canadiense de Pediatría)",
            "vancomicina_iv", "iv", "mg_kg_dia", 60, 6, CPS, "Tablas 1 y 2", edad=(1, ""),
            nota="Ajustar según los niveles séricos (valle de 10 a 15 mg/L)."),

    # Neumonía complicada (SADI 2024, Argentina).
    esquema("neumonia_complicada", "primera", 1, "Ampicilina (mayores de 3 meses)", "ampicilina", "iv",
            "mg_kg_dia", 300, 6, SADI, "Tabla 2 (p. 13), tabla 4 y sección D (p. 14)", dias=(14, ""), edad=(3, ""),
            nota=HOSPITAL + " Al menos 2 semanas, según la evolución. Igual en la Sociedad Argentina de "
                 "Pediatría 2021 (p. S187)."),
    esquema("neumonia_complicada", "primera", 2, "Cefotaxima más ampicilina (1 a 3 meses)", "cefotaxima", "iv",
            "mg_kg_dia", (100, 150, 150), 8, SADI, "Tabla 2 (p. 13) y tabla 4 (p. 14)", dias=(14, ""), edad=(1, 3),
            nota=HOSPITAL + " La guía la da cada 6 a 8 horas; se calcula cada 8."),
    esquema("neumonia_complicada", "primera", 2, "Cefotaxima más ampicilina (1 a 3 meses)", "ampicilina", "iv",
            "mg_kg_dia", 300, 6, SADI, "Tabla 2 (p. 13) y tabla 4 (p. 14)", dias=(14, ""), edad=(1, 3)),
    esquema("neumonia_complicada", "segunda", 1, "Clindamicina (sospecha de S. aureus)", "clindamicina", "iv",
            "mg_kg_dia", 30, 8, SADI, "Tabla 2 (p. 13) y tablas 3 y 4 (p. 14)", nota=HOSPITAL),
    esquema("neumonia_complicada", "segunda", 2, "Vancomicina (sospecha de S. aureus resistente)", "vancomicina_iv",
            "iv", "mg_kg_dia", 60, 6, SADI, "Tabla 2 (p. 13) y tabla 4 (p. 14)",
            nota=HOSPITAL + " La guía la da cada 6 a 8 horas; se calcula cada 6. Ajustar según los niveles séricos."),

    # Rinosinusitis grave hospitalizada (ACORL 2024, Colombia).
    esquema("rinosinusitis_hospitalizada", "primera", 1, "Ampicilina-sulbactam", "ampicilina_sulbactam", "iv",
            "mg_kg_dia", (200, 400, 400), 6, ACORL_RS, "Tabla 5 (p. 625) y p. 624", dias=(5, 10),
            nota=HOSPITAL + " Duración usual de 7 días. La guía no dice a qué componente se refiere la dosis; "
                 "se toma en ampicilina: confirmar."),
    esquema("rinosinusitis_hospitalizada", "primera", 2, "Ceftriaxona", "ceftriaxona", "iv", "mg_kg_dia", 50, 12,
            ACORL_RS, "Tabla 5 (p. 625) y p. 624", dias=(5, 10),
            nota=HOSPITAL + " La guía la da cada 12 horas, que es poco habitual para la ceftriaxona: confirmar."),
    esquema("rinosinusitis_hospitalizada", "primera", 3, "Cefotaxima", "cefotaxima", "iv", "mg_kg_dia",
            (100, 200, 200), 6, ACORL_RS, "Tabla 5 (p. 625) y p. 624", dias=(5, 10), nota=HOSPITAL),
    esquema("rinosinusitis_hospitalizada", "segunda", 1, "Levofloxacina", "levofloxacina", "iv", "mg_kg_dia",
            (10, 20, 20), 12, ACORL_RS, "Tabla 5 (p. 625)", dias=(5, 10),
            nota=HOSPITAL + " La guía la da cada 12 a 24 horas; se calcula cada 12."),

    # Infección grave de piel por SARM (ACIN 2023, Colombia).
    esquema("piel_sarm_grave", "primera", 1, "Vancomicina", "vancomicina_iv", "iv", "mg_kg_dia", (40, 60, 60), 6,
            ACIN, "Tabla 3 (p. 120)",
            nota=HOSPITAL + " Indicada en infección grave o bacteriemia; considerar dosis de carga en casos "
                 "graves. Ajustar según los niveles séricos."),
    esquema("piel_sarm_grave", "segunda", 1, "Clindamicina", "clindamicina", "iv", "mg_kg_dia", (30, 40, 40), 8,
            ACIN, "Tabla 3 (p. 120)",
            nota=HOSPITAL + " La guía la da en 3 o 4 dosis; se calcula cada 8 horas. No se recomienda si se "
                 "sospecha bacteriemia."),
    esquema("piel_sarm_grave", "segunda", 2, "Linezolid", "linezolid", "iv", "mg_kg_dia", 30, 8, ACIN,
            "Tabla 3 (p. 120)",
            nota=HOSPITAL + " Alternativa a la clindamicina cuando se sospecha un mecanismo de toxinas. La guía "
                 "también da daptomicina y ceftarolina por edad, que no se cargaron (uso especializado)."),

    # Yersiniosis (guía francesa 2017).
    esquema("yersiniosis", "primera", 1, "Trimetoprim-sulfametoxazol", "trimetoprim_sulfametoxazol", "oral",
            "mg_kg_dia", 6, 12, GASTRO, "Tabla 1 (p. S28)", dias=(5, 5),
            nota="La guía da 30 mg/kg/día de sulfametoxazol, equivalentes a 6 mg/kg/día de trimetoprim "
                 "(relación 5:1)."),
    esquema("yersiniosis", "primera", 2, "Ceftriaxona", "ceftriaxona", "iv", "mg_kg_dia", 50, 24, GASTRO,
            "Tabla 1 (p. S28)", dias=(5, 5), nota="También puede darse por vía intramuscular."),
    esquema("yersiniosis", "segunda", 1, "Doxiciclina (8 años o más)", "doxiciclina", "oral", "mg_kg_dia", 4, 12,
            GASTRO, "Tabla 1 (p. S28)", max_dia=200, dias=(5, 5), edad=(96, "")),
    esquema("yersiniosis", "segunda", 2, "Ciprofloxacina", "ciprofloxacina", "oral", "mg_kg_dia", (20, 30, 30), 12,
            GASTRO, "Tabla 1 (p. S28)", max_dia=1500, dias=(5, 5)),

    # C. difficile (IDSA/SHEA 2017).
    esquema("clostridioides_difficile", "primera", 2, "Metronidazol oral (IDSA/SHEA)", "metronidazol", "oral",
            "mg_kg_dosis", 7.5, 8, CDI, "Tabla 2", max_dosis=500, dias=(10, 10),
            nota="Primer episodio no grave. La guía lo da 3 o 4 veces al día; se calcula cada 8 horas."),
    esquema("clostridioides_difficile", "segunda", 2, "Vancomicina oral (enfermedad grave o fulminante)",
            "vancomicina", "oral", "mg_kg_dosis", 10, 6, CDI, "Tabla 2", max_dosis=500, dias=(10, 10),
            nota="Con o sin metronidazol intravenoso (10 mg/kg por dosis cada 8 horas, máximo 500 mg)."),

    # Shigelosis: AIEPI 2012 (Colombia), único esquema colombiano dosificado con un medicamento registrado.
    esquema("shigelosis", "segunda", 3, "Ceftriaxona (AIEPI Colombia, 2 meses a 5 años)", "ceftriaxona", "iv",
            "mg_kg_dia", 100, 24, "aiepi_2012", "Sección 6.3.5 (p. 529)", dias=(5, 5), edad=(2, 60),
            nota=HOSPITAL + " AIEPI y la guía del Ministerio de 2013 dan como primera opción el ácido nalidíxico, "
                 "que no tiene registro sanitario vigente."),

    # Faringoamigdalitis: cefadroxilo (IDSA 2012 y Sociedad Chilena de Infectología 2019).
    esquema("faringoamigdalitis", "alergia", 6, "Cefadroxilo", "cefadroxilo", "oral", "mg_kg_dia", 30, 24,
            "idsa_faringitis_2012", "Tabla 2", max_dia=1000, dias=(10, 10),
            nota="Evitar si hubo hipersensibilidad inmediata a la penicilina. Igual en la Sociedad Chilena de "
                 "Infectología 2019 (tabla 5, p. 73)."),

    # Otitis media aguda: clindamicina (ACORL 2024 la recomienda en la alergia de tipo I, sin dosis).
    esquema("oma", "alergia", 5, "Clindamicina (alergia de tipo I a la penicilina)", "clindamicina", "oral",
            "mg_kg_dia", (30, 40, 40), 8, AAP_OMA, "Tabla 5 (p. e983)",
            nota="La guía colombiana ACORL 2024 recomienda clindamicina en la alergia de tipo I sin dar la dosis; "
                 "la dosis es de AAP 2013. No hay suspensión de clindamicina en Colombia."),
], clave=lambda f: (f["enfermedad_id"], f["linea"], f["opcion"], f["medicamento_id"]))

# --- Notas que la revisión precisa ----------------------------------------------
modificar_nota("esquemas", lambda f: f["enfermedad_id"] == "otitis_media_cronica",
               "La guía australiana de 2021 (niños aborígenes de alto riesgo) indica 5 gotas de ciprofloxacina "
               "cada 12 horas.")
modificar_nota("esquemas", lambda f: f["enfermedad_id"] in ("ivu_baja", "ivu_alta")
               and f["medicamento_id"] == "trimetoprim_sulfametoxazol",
               "La Sociedad Chilena de Infectología (2022) expresa la dosis en trimetoprim.")

(FUENTE / "metadatos.json").write_text(
    '{\n  "version": "0.5.0-borrador",\n  "fecha": "2026-11-18"\n}\n', encoding="utf-8")
