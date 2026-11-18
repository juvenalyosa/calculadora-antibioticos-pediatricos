"""Ejecuta la prueba exhaustiva completa. Uso: python3 docs/validacion/ejecutar.py <calc-dosis> [carpeta]

1. Genera la población modelada (poblacion.py).
2. Calcula todos los escenarios dos veces con `calc-dosis lote`.
3. Verifica los resultados (verificar.py) y escribe informe.json.
Termina con el código de salida de la verificación.
"""
import subprocess
import sys
from pathlib import Path

AQUI = Path(__file__).resolve().parent


def main():
    calc = sys.argv[1]
    carpeta = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("validacion")
    carpeta.mkdir(parents=True, exist_ok=True)
    poblacion = carpeta / "poblacion.csv"
    subprocess.run([sys.executable, str(AQUI / "poblacion.py"), str(poblacion)], check=True)
    for nombre in ("resultados.csv", "resultados_2.csv"):
        subprocess.run([calc, "lote", "--kb", str(AQUI.parent.parent / "data" / "kb" / "base_conocimiento.json"),
                        "--entrada", str(poblacion), "--salida", str(carpeta / nombre)], check=True)
    resultado = subprocess.run([sys.executable, str(AQUI / "verificar.py"), str(poblacion),
                                str(carpeta / "resultados.csv"), str(carpeta / "resultados_2.csv")])
    sys.exit(resultado.returncode)


if __name__ == "__main__":
    main()
