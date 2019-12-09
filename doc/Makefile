# Author: Radoslav Grenčík <xgrenc00@stud.fit.vutbr.cz>

DOC = documentation
.PHONY: bibtech clean pdfdestroy

all: $(DOC).tex
	make pdfdestroy
	pdflatex $(DOC).tex
	make bibtex
	pdflatex $(DOC).tex
	pdflatex $(DOC).tex
	make clean

bibtex: $(DOC).bib
	bibtex $(DOC)

clean:
	rm -f $(DOC).aux $(DOC).bbl $(DOC).blg $(DOC).log $(DOC).out $(DOC).toc

pdfdestroy:
	rm -f $(DOC).pdf
