# Cómo instalar la calculadora

La calculadora funciona sin conexión a internet y no necesita instalar nada
más: el instalador trae el programa, sus bibliotecas y la base de datos de
dosis. No se requieren permisos de administrador.

> **Prototipo académico.** El contenido clínico está en borrador y sin
> verificación clínica. No usar para decisiones clínicas.

Los instaladores se descargan de la página de publicaciones del repositorio,
[Releases → última versión](https://github.com/juvenalyosa/calculadora-antibioticos-pediatricos/releases/latest),
iniciando sesión en GitHub con una cuenta que tenga acceso al repositorio.
Elija el archivo según su computador:

| Sistema | Archivo |
|---|---|
| Windows 10 u 11 (64 bits) | `CalculadoraAntibioticos-<versión>-windows-instalador.exe` |
| Mac (Intel o Apple Silicon), macOS 12 o posterior | `CalculadoraAntibioticos-<versión>-macos.dmg` |
| Linux de 64 bits | `CalculadoraAntibioticos-<versión>-linux-x86_64.AppImage` |

## Windows

1. Abra el archivo `...-windows-instalador.exe` con doble clic.
2. Windows puede mostrar el aviso azul «Windows protegió su PC», porque el
   programa todavía no tiene firma digital. Pulse **Más información** y luego
   **Ejecutar de todas formas**.
3. Siga el asistente: **Siguiente**, **Instalar**, **Finalizar**.
4. La calculadora queda en el menú Inicio como «Calculadora de antibióticos»
   y, si lo marcó, con un acceso directo en el escritorio.

Para quitarla: Configuración → Aplicaciones → «Calculadora de antibióticos
pediátricos» → Desinstalar.

**Sin instalar.** Si prefiere no instalar nada, use
`...-windows-portatil.zip`: descomprímalo en cualquier carpeta (o en una
memoria USB) y abra `CalculadoraAntibioticos.exe`.

## Mac

1. Abra el archivo `.dmg` con doble clic.
2. Arrastre **CalculadoraAntibioticos** sobre la carpeta **Aplicaciones**.
3. La primera vez, macOS la bloquea porque todavía no está firmada por un
   desarrollador registrado en Apple:
   - Abra la calculadora desde Aplicaciones; aparecerá un aviso. Ciérrelo.
   - Vaya a **Ajustes del Sistema → Privacidad y seguridad**, baje hasta el
     mensaje sobre «CalculadoraAntibioticos» y pulse **Abrir igualmente**.
   - Confirme con su contraseña o huella. Solo hay que hacerlo una vez.

Para quitarla: arrastre la aplicación desde Aplicaciones a la papelera.

## Linux

1. Guarde el archivo `.AppImage` donde quiera, por ejemplo en su carpeta
   personal.
2. Dele permiso de ejecución: clic derecho → **Propiedades** → **Permisos** →
   marque «Permitir ejecutar el archivo como un programa». O, en una terminal:
   `chmod +x CalculadoraAntibioticos-*.AppImage`
3. Ábralo con doble clic.

Para quitarla: borre el archivo.

## Dónde quedan los datos

- La **base de datos de dosis** viaja dentro del programa; no hay que
  descargarla ni configurarla.
- Los **reportes de error** que usted escriba con el botón «Reportar un
  error…» se guardan en un archivo dentro de su carpeta de usuario. No
  contienen datos que identifiquen al paciente.
- El programa no envía nada por internet.

## Si algo falla

- **No abre en Windows:** confirme que el equipo es de 64 bits y que
  descomprimió todo el contenido si usa la versión portátil.
- **«No se encontró la base de conocimiento»:** el programa se movió sin su
  carpeta. Vuelva a instalarlo o a descomprimir el paquete completo.
- **Otro problema:** anote el mensaje exacto y la versión (aparece en el
  título de la ventana) y repórtelo al equipo del proyecto.
