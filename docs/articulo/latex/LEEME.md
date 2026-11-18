# Artículo en LaTeX

- `manuscrito.tex`: artículo original en español (IMRyD), con resumen en
  español y en inglés.
- `material_suplementario.tex`: ecuaciones de búsqueda, motivos de exclusión,
  documentos incluidos, tablas de detalle y archivos para reproducir los
  resultados. Sus tablas se numeran S1 a S10 y el manuscrito las cita con
  esos números.
- `referencias.bib`: bibliografía. Cada entrada con DOI se comprobó en
  Crossref o PubMed (ver `notas_autores.md`).
- `vancouver-es.bst`: estilo Vancouver con los textos en español.
- `notas_autores.md`: datos por completar, auditoría de las referencias,
  revisión crítica simulada y pendientes. No se envía a la revista.
- `datos/`: datos de las figuras y tablas generadas. No editarlos a mano:
  - `peso_*.dat` y `amox_*.dat` salen de la prueba exhaustiva
    (`build/dev/tests/validacion/`);
  - `busqueda_*.tex` los genera `docs/articulo/tabla_busqueda.py`;
  - `incluidos.tex` lo genera `docs/articulo/tabla_incluidos.py`.

Compilar con XeLaTeX (las fuentes TeX Gyre vienen con TeX Live) o con Tectonic:

```bash
cd docs/articulo/latex
tectonic manuscrito.tex
tectonic material_suplementario.tex
# o bien, para cada uno:
xelatex manuscrito && bibtex manuscrito && xelatex manuscrito && xelatex manuscrito
```

También compila en Overleaf: subir la carpeta completa y elegir XeLaTeX como
compilador.
