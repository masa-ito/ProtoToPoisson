
all : equations.pdf

clean : 
	rm -rf *.log *.aux *.dvi *.pdf *.*~

equations.dvi : equations.tex
	platex $<

equations.pdf : equations.dvi
	dvipdf $<

