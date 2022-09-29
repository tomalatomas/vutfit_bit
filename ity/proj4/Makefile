DOCNAME = proj4

ALL: clean
	latex ${DOCNAME}.tex
	bibtex ${DOCNAME}
	latex ${DOCNAME}.tex
	latex ${DOCNAME}.tex
	dvips ${DOCNAME}.dvi
	ps2pdf -sPAPERSIZE=a4 ${DOCNAME}.ps

c: clean
clean:
	rm -f *.dvi *.log *.blg *.bbl *.toc *.aux *.out *.lof *.ptc
	rm -f *~
