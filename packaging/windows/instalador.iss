; Instalador de Windows (Inno Setup 6).
;
; Se instala para el usuario actual, sin permisos de administrador: la carpeta
; por defecto queda dentro de su perfil. Uso:
;   ISCC.exe /DVersion=0.2.0 packaging\windows\instalador.iss
; Espera el programa ya desplegado (con las bibliotecas de Qt) en dist\.

#ifndef Version
  #define Version "0.0.0"
#endif

[Setup]
AppId={{6F3B2C1E-8A47-4D5B-9E0C-2B7A1D4C9F31}
AppName=Calculadora de antibióticos pediátricos
AppVersion={#Version}
AppPublisher=Universidad Militar Nueva Granada
DefaultDirName={autopf}\CalculadoraAntibioticos
DisableProgramGroupPage=yes
PrivilegesRequired=lowest
OutputDir=..\..\salida
OutputBaseFilename=CalculadoraAntibioticos-{#Version}-windows-instalador
SetupIconFile=..\iconos\CalculadoraAntibioticos.ico
UninstallDisplayIcon={app}\CalculadoraAntibioticos.exe
UninstallDisplayName=Calculadora de antibióticos pediátricos
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

[Languages]
Name: "es"; MessagesFile: "compiler:Languages\Spanish.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "..\..\dist\*"; DestDir: "{app}"; Flags: recursesubdirs ignoreversion

[Icons]
Name: "{autoprograms}\Calculadora de antibióticos"; Filename: "{app}\CalculadoraAntibioticos.exe"
Name: "{autodesktop}\Calculadora de antibióticos"; Filename: "{app}\CalculadoraAntibioticos.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\CalculadoraAntibioticos.exe"; Description: "{cm:LaunchProgram,Calculadora de antibióticos}"; Flags: nowait postinstall skipifsilent
