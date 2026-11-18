# Empaquetado

Cómo se construyen los instaladores. Para instalar, ver
[instalacion.md](instalacion.md).

## Qué se entrega

| Sistema | Paquete | Herramienta | Dónde se construye |
|---|---|---|---|
| Windows | Instalador `.exe` por usuario, sin administrador, y `.zip` portátil | `windeployqt` + Inno Setup | GitHub Actions |
| macOS | Imagen `.dmg` con binario universal (Intel y Apple Silicon) | `macdeployqt` + `hdiutil` | GitHub Actions o un Mac |
| Linux | `AppImage` de 64 bits | `linuxdeploy` con su complemento de Qt | GitHub Actions |

Los tres llevan dentro Qt, las bibliotecas del compilador y
`base_conocimiento.json`. El flujo `.github/workflows/paquetes.yml` los
construye y, antes de publicarlos, ejecuta el programa ya empaquetado para
comprobar que arranca y encuentra su base.

## Publicar una versión

1. Actualizar `VERSION` en `CMakeLists.txt` y, si cambió el contenido clínico,
   `data/fuente/metadatos.json`.
2. Crear y subir una etiqueta: `git tag v0.2.0 && git push origin v0.2.0`.
3. El flujo deja un **borrador** de publicación en GitHub con los instaladores.
   Revisarlo y publicarlo es una decisión manual.

Sin etiqueta, el flujo también se puede lanzar a mano desde la pestaña
*Actions*; los instaladores quedan como artefactos de esa ejecución.

## Construir el de macOS en un Mac

```bash
cmake -S . -B build/paquete -DCMAKE_BUILD_TYPE=Release -DCALC_BUILD_TESTS=OFF \
  -DCMAKE_PREFIX_PATH=<ruta de Qt> "-DCMAKE_OSX_ARCHITECTURES=x86_64;arm64" \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0
cmake --build build/paquete --parallel
MACDEPLOYQT=<ruta de Qt>/bin/macdeployqt sh packaging/macos/crear_dmg.sh \
  build/paquete/src/gui/CalculadoraAntibioticos.app salida/CalculadoraAntibioticos-macos.dmg
```

## Iconos

El icono se dibuja por código (`renderAppIcon` en `src/gui/theme.cpp`). Si
cambia, hay que regenerar los archivos de `packaging/iconos/`:

```bash
CalculadoraAntibioticos --tema claro --exportar-icono <carpeta>   # PNG de 16 a 1024 px
```

Con esos PNG se arman el `.icns` (`iconutil`), el `.ico` y el PNG de 256 px.

## Pendiente: firma digital

Los instaladores no están firmados. Por eso Windows muestra el aviso de
SmartScreen y macOS exige autorizar la primera apertura en *Privacidad y
seguridad*. Para quitar esos avisos se necesita:

- **Windows:** un certificado de firma de código y firmar el `.exe` y el
  instalador.
- **macOS:** una cuenta de desarrollador de Apple, firmar con *Developer ID* y
  notarizar la imagen.

Ambos son trámites de la institución, no de programación.

## Licencias

Qt se usa bajo LGPL v3 y se distribuye como bibliotecas separadas del
ejecutable, que el usuario puede reemplazar. Falta incluir en los instaladores
el aviso de licencias de terceros.
