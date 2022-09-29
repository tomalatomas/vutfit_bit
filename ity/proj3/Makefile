DOCNAME=proj3

ALL: clean 
	latex ${DOCNAME}.tex
	latex ${DOCNAME}.tex
	dvips ${DOCNAME}.dvi
	ps2pdf -sPAPERSIZE=a4 ${DOCNAME}.ps

c: clean
clean:
	rm -f ${DOCNAME}.dvi
	rm -f ${DOCNAME}.ps
	rm -f ${DOCNAME}.pdf
