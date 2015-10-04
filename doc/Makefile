# -*- coding: UTF-8 -*-
# vim: autoindent expandtab tabstop=4 sw=4 sts=4 filetype=make

.PHONY: all build clean dist-clean
JOB   := mte7101
BUILD := mte7101
CLEAN_EXT := nls,ilg,aux,log,idx,out,glo,toc,lot,tex~,backup,bbl,blg,1,2,3,4,5,6,7,8,9,10,dvi,emp*mp,ooplss*mp,nlo,tdo,dvi,lof,lol,glg,gls,ist

all: clean build

build:
	# -shell-escape allows LaTeX calls to the outside world
	pdflatex -jobname $(JOB) -shell-escape -interaction=nonstopmode $(BUILD) 1> /dev/null
	bibtex $(JOB) 1> /dev/null
	pdflatex -jobname $(JOB) -shell-escape -interaction=nonstopmode $(BUILD) 1> /dev/null
	makeglossaries $(JOB) 1> /dev/null
	pdflatex -jobname $(JOB) -shell-escape -interaction=nonstopmode $(BUILD) 1> /dev/null
	pdflatex -jobname $(JOB) -shell-escape -interaction=nonstopmode $(BUILD) 1> /dev/null

watch:
	while true; do inotifywait -r -e modify --exclude=".swp" . && make; done

clean:
	@rm -f *{$(CLEAN_EXT)}
	@rm -f inc/*{$(CLEAN_EXT)}

dist-clean: clean
	@rm -f *.pdf
