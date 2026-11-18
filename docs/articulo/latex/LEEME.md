# Manuscrito en LaTeX

- `manuscrito.tex`: texto completo (español, IMRyD, anexos A a E).
- `referencias.bib`: bibliografía; cada entrada con DOI se comprobó en Crossref
  o PubMed (anexo B del manuscrito).
- `datos/`: datos de las figuras, generados a partir de la prueba exhaustiva
  (`build/dev/tests/validacion/`). No editarlos a mano.

Compilar con XeLaTeX (las fuentes TeX Gyre vienen con TeX Live) o con Tectonic:

```bash
cd docs/articulo/latex
tectonic manuscrito.tex
# o bien
xelatex manuscrito && bibtex manuscrito && xelatex manuscrito && xelatex manuscrito
```

También compila en Overleaf: subir la carpeta completa y elegir XeLaTeX como
compilador.
