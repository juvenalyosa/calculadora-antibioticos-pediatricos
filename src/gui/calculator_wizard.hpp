/// @file calculator_wizard.hpp
/// @brief Ventana principal: asistente por pasos de la calculadora.
///
///     paciente → función renal → enfermedad → resultado
///
/// En todos los pasos hay un botón «Reportar un error…» que abre el formato
/// semiautomatizado con los parámetros introducidos hasta ese momento.
#pragma once

#include <QWizard>

#include "calc/advisor.hpp"
#include "calc/error_report.hpp"
#include "calc/knowledge_base.hpp"
#include "session.hpp"

namespace calc::gui {

class DiseasePage;
class PatientPage;
class RenalPage;
class ResultPage;
class StepSidebar;

class CalculatorWizard final : public QWizard {
  Q_OBJECT

 public:
  /// Identificadores de página, en el orden del flujo.
  enum PageId { kPatientPage, kRenalPage, kDiseasePage, kResultPage };

  /// La base, el destino de reportes y los módulos de apoyo deben vivir más
  /// que la ventana.
  CalculatorWizard(const KnowledgeBase& kb, ReportSink& report_sink,
                   const AdvisorRegistry& advisors, QWidget* parent = nullptr);

  // Acceso a las páginas para cargar valores sin intervención del usuario
  // (pruebas y capturas de pantalla).
  [[nodiscard]] PatientPage* patientPage() const { return patient_page_; }
  [[nodiscard]] RenalPage* renalPage() const { return renal_page_; }
  [[nodiscard]] DiseasePage* diseasePage() const { return disease_page_; }

  /// Arma el reporte de error del paso actual con lo que hay en la sesión.
  [[nodiscard]] ErrorReport buildReportForCurrentStep() const;

  /// En la última página, «Nuevo cálculo» vuelve al paso 1 en vez de cerrar.
  void accept() override;

 private:
  /// Nombre del paso que queda escrito en el reporte de error.
  [[nodiscard]] std::string currentStepName() const;
  void openReportDialog();

  const KnowledgeBase& kb_;
  ReportSink& report_sink_;
  Session session_;
  StepSidebar* sidebar_ = nullptr;
  PatientPage* patient_page_ = nullptr;
  RenalPage* renal_page_ = nullptr;
  DiseasePage* disease_page_ = nullptr;
  ResultPage* result_page_ = nullptr;
};

}  // namespace calc::gui
