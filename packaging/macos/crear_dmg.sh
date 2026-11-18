#!/bin/sh
# Crea la imagen de disco (.dmg) de macOS a partir del paquete .app compilado.
#
# Uso: crear_dmg.sh <CalculadoraAntibioticos.app> <salida.dmg>
#
# Pasos: copia el .app a una carpeta de trabajo, le añade las bibliotecas de Qt
# (macdeployqt), lo firma «ad hoc», comprueba que arranca y que encuentra su
# base de conocimiento, y arma la imagen con un acceso a Aplicaciones.
#
# La firma ad hoc permite ejecutar el programa, pero NO evita el aviso de
# «desarrollador no identificado»: para eso hace falta un certificado de Apple
# y notarizar la imagen (ver docs/empaquetado.md).
set -eu

APP="$1"
SALIDA="$2"
MACDEPLOYQT="${MACDEPLOYQT:-macdeployqt}"

TRABAJO="$(mktemp -d)"
trap 'rm -rf "$TRABAJO"' EXIT
cp -R "$APP" "$TRABAJO/"
COPIA="$TRABAJO/$(basename "$APP")"

"$MACDEPLOYQT" "$COPIA" -verbose=1
codesign --force --deep --sign - "$COPIA"
codesign --verify --deep --strict "$COPIA"

# Prueba de humo: recorre los cuatro pasos con el paquete ya desplegado. Si
# faltara una biblioteca o la base, fallaría aquí. El paquete solo lleva el
# complemento de pantalla de macOS, así que abre una ventana por un instante.
# PRUEBA_DE_HUMO=0 la omite (máquinas sin sesión gráfica).
if [ "${PRUEBA_DE_HUMO:-1}" = "1" ]; then
  "$COPIA/Contents/MacOS/CalculadoraAntibioticos" --tema claro --capturas "$TRABAJO/capturas"
  rm -rf "$TRABAJO/capturas"
fi

ln -s /Applications "$TRABAJO/Aplicaciones"
mkdir -p "$(dirname "$SALIDA")"
rm -f "$SALIDA"
hdiutil create -volname "Calculadora de antibioticos" -srcfolder "$TRABAJO" \
  -ov -format UDZO "$SALIDA"
echo "Imagen creada: $SALIDA"
