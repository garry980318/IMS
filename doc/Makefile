# Author: Radoslav Grenčík <xgrenc00@stud.fit.vutbr.cz>

DOC = documentation
.PHONY: bibtech clean

$(DOC).pdf: $(DOC).tex
	pdflatex $(DOC).tex
	make bibtex
	pdflatex $(DOC).tex
	pdflatex $(DOC).tex

bibtex: $(DOC).bib
	bibtex $(DOC)

clean:
