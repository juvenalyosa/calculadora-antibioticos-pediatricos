/// @file test_kb.hpp
/// @brief Base de conocimiento SINTÉTICA para las pruebas unitarias.
///
/// Usa medicamentos ficticios ("Fármaco A/B/C") con números redondos para que
/// los valores esperados se puedan comprobar mentalmente y para que las
/// pruebas del motor no cambien cuando se actualice el contenido clínico real.
///
/// Resumen de lo que contiene:
///   Fármaco A (mg)  ajuste renal por bandas: 30–50 ×0,5 · 10–30 ×0,5 y
///                   mínimo cada 24 h · <10 evitar. Suspensión 250 mg/5 mL y
///                   tableta ranurada de 500 mg.
///   Fármaco B (UI)  sin dato de ajuste renal. Vial divisible de 1 200 000 UI.
///   Fármaco C (mg)  no requiere ajuste renal. Sin presentaciones.
#pragma once

#include <string>

#include "calc/kb_loader.hpp"
#include "calc/patient.hpp"

namespace calc::testing {

inline const char* const kTestKbJson = R"json({
  "esquema_version": 1,
  "version": "prueba",
  "fecha": "2026-01-01",
  "fuentes": [
    {"id": "guia_prueba", "titulo": "Guía de prueba", "organizacion": "Pruebas", "anio": 2026}
  ],
  "medicamentos": [
    {
      "id": "farmaco_a", "nombre": "Fármaco A", "unidad": "mg",
      "ajuste_renal": {
        "tipo": "por_bandas", "estado": "verificado", "fuente_id": "guia_prueba",
        "bandas": [
          {"tfg_min": 30, "tfg_max": 50, "factor_dosis": 0.5},
          {"tfg_min": 10, "tfg_max": 30, "factor_dosis": 0.5, "intervalo_minimo_horas": 24},
          {"tfg_min": 0, "tfg_max": 10, "evitar": true, "nota": "Usar una alternativa."}
        ]
      },
      "presentaciones": [
        {"id": "a_susp", "descripcion": "Suspensión 250 mg/5 mL", "via": "oral",
         "tipo": "liquido", "cantidad": 250, "volumen_ml": 5},
        {"id": "a_tab", "descripcion": "Tableta 500 mg", "via": "oral",
         "tipo": "solido", "cantidad": 500, "unidad": "tableta", "divisible": true}
      ]
    },
    {
      "id": "farmaco_b", "nombre": "Fármaco B", "unidad": "UI",
      "ajuste_renal": {"tipo": "sin_dato", "estado": "muestra"},
      "presentaciones": [
        {"id": "b_vial", "descripcion": "Vial 1 200 000 UI", "via": "im",
         "tipo": "solido", "cantidad": 1200000, "unidad": "vial", "divisible": true}
      ]
    },
    {
      "id": "farmaco_c", "nombre": "Fármaco C", "unidad": "mg",
      "ajuste_renal": {"tipo": "no_requiere", "estado": "verificado", "fuente_id": "guia_prueba"},
      "presentaciones": []
    }
  ],
  "enfermedades": [
    {
      "id": "enfermedad_x", "nombre": "Enfermedad X", "grupo": "Pruebas",
      "requiere_antibiotico": true, "fuente_id": "guia_prueba",
      "lineas": [
        {
          "linea": "primera",
          "opciones": [
            {
              "numero": 1, "descripcion": "Fármaco A por día",
              "componentes": [{
                "medicamento_id": "farmaco_a", "via": "oral", "base": "mg_kg_dia",
                "dosis": {"min": 40, "usual": 50, "max": 60},
                "intervalo_horas": 8, "max_por_dia": 1500,
                "duracion_dias": {"min": 7, "max": 10},
                "fuente_id": "guia_prueba", "pagina": "Tabla 1",
                "estado": "verificado", "verificado_por": ["AB", "CD"]
              }]
            },
            {
              "numero": 2, "descripcion": "Fármaco B (menos de 27 kg)",
              "componentes": [{
                "medicamento_id": "farmaco_b", "via": "im", "base": "dosis_fija",
                "dosis": {"min": 600000, "usual": 600000, "max": 600000},
                "peso_kg": {"max": 27},
                "fuente_id": "guia_prueba", "estado": "muestra"
              }]
            },
            {
              "numero": 3, "descripcion": "Fármaco B (27 kg o más)",
              "componentes": [{
                "medicamento_id": "farmaco_b", "via": "im", "base": "dosis_fija",
                "dosis": {"min": 1200000, "usual": 1200000, "max": 1200000},
                "peso_kg": {"min": 27},
                "fuente_id": "guia_prueba", "estado": "muestra"
              }]
            }
          ]
        },
        {
          "linea": "segunda",
          "opciones": [
            {
              "numero": 1, "descripcion": "Combinación A + C",
              "componentes": [
                {
                  "medicamento_id": "farmaco_a", "via": "oral", "base": "mg_kg_dosis",
                  "dosis": {"min": 25, "usual": 25, "max": 25},
                  "intervalo_horas": 12, "max_por_dosis": 500,
                  "fuente_id": "guia_prueba",
                  "estado": "verificado", "verificado_por": ["AB", "CD"]
                },
                {
                  "medicamento_id": "farmaco_c", "via": "iv", "base": "mg_kg_dosis",
                  "dosis": {"min": 10, "usual": 10, "max": 10},
                  "intervalo_horas": 24,
                  "fuente_id": "guia_prueba",
                  "estado": "verificado", "verificado_por": ["AB", "CD"]
                }
              ]
            }
          ]
        },
        {
          "linea": "alergia",
          "opciones": [
            {
              "numero": 1, "descripcion": "Fármaco C tópico",
              "componentes": [{
                "medicamento_id": "farmaco_c", "via": "topica", "base": "sin_calculo",
                "nota": "Aplicar cada 8 horas.",
                "fuente_id": "guia_prueba",
                "estado": "verificado", "verificado_por": ["AB", "CD"]
              }]
            },
            {
              "numero": 2, "descripcion": "Fármaco A (desde los 2 años)",
              "componentes": [{
                "medicamento_id": "farmaco_a", "via": "oral", "base": "mg_kg_dosis",
                "dosis": {"min": 10, "usual": 10, "max": 10},
                "intervalo_horas": 12,
                "edad_meses": {"min": 24},
                "fuente_id": "guia_prueba",
                "estado": "verificado", "verificado_por": ["AB", "CD"]
              }]
            }
          ]
        }
      ]
    },
    {
      "id": "enfermedad_viral", "nombre": "Enfermedad viral",
      "requiere_antibiotico": false, "mensaje": "No requiere antibiótico."
    }
  ]
})json";

/// Base sintética ya cargada.
inline KnowledgeBase loadTestKb() {
  return loadKnowledgeBaseFromString(kTestKbJson);
}

// Los dos casos tipo de la validación alfa (documento de radicación).

/// Lactante: 1 año, 10 kg, 75 cm.
inline Patient infantCase() {
  return Patient{Age::fromYearsAndMonths(1, 0), Kilograms(10.0), Centimeters(75.0)};
}

/// Adolescente: 13 años, 50 kg, 162 cm.
inline Patient adolescentCase() {
  return Patient{Age::fromYearsAndMonths(13, 0), Kilograms(50.0), Centimeters(162.0)};
}

}  // namespace calc::testing
