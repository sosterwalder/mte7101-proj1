# -*- coding: UTF-8 -*-
# vim: autoindent expandtab tabstop=4 sw=4 sts=4 filetype=make

JOB          := mte7101
BUILD        := mte7101
CLEAN_EXT    := nls,ilg,aux,log,idx,out,glo,toc,lot,tex~,backup,bbl,blg,1,2,3,4,5,6,7,8,9,10,dvi,emp*mp,ooplss*mp,nlo,tdo,dvi,lof,lol,glg,gls,ist,hst,ver
PDFLATEX     := pdflatex
DEBUGLATEX   := ./pdflatex-color.sh
BIBLIOGRAPHY := biber
OS           := $(shell uname)

.PHONY: all build build-fast build-debug clean dist-clean watch watch-fast

.set-build-fast:
	$(eval BUILD_COMMAND = build-fast)

.set-build:
	$(eval BUILD_COMMAND = build)

.watch-command:
ifeq ($(OS), Darwin)
	fswatch -o . -r --include '\.tex$$' --include '\.png$$' --include 'minutes/.*' --exclude '.*' | (while read; do make $(BUILD_COMMAND); done)
else
	while true; do inotifywait -r -e modify --exclude="(.*.lol|.*.lot|.*.lof|.*.toc|.*.tdo|.*.ist|.*.out|.*.pdf|.*.aux|.*.glo|.*.idx|.*.bbl|.*.swp|.*.blg|.*.fdb_latexmk|.*.fls|.*.log)" . && make $(BUILD_COMMAND); done
endif

test: .set-build
	@echo $(BUILD_TYPE)

test-fast: .set-build-fast
	@echo $(BUILD_TYPE)

all: clean build

build: clean
	$(PDFLATEX) -jobname $(JOB) -shell-escape -interaction=nonstopmode $(BUILD) 1> /dev/null
	$(BIBLIOGRAPHY) $(JOB) 1> /dev/null
	# $(PDFLATEX) -jobname $(JOB) -shell-escape -interaction=nonstopmode $(BUILD) 1> /dev/null
	makeglossaries $(JOB) 1> /dev/null
	$(PDFLATEX) -jobname $(JOB) -shell-escape -interaction=nonstopmode $(BUILD) 1> /dev/null
	# $(PDFLATEX) -jobname $(JOB) -shell-escape -interaction=nonstopmode $(BUILD) 1> /dev/null

build-fast:
	$(PDFLATEX) -jobname $(JOB) -shell-escape -interaction=nonstopmode $(BUILD) 1> /dev/null

build-debug:
	$(DEBUGLATEX) -jobname $(JOB) $(BUILD)

watch: .set-build .watch-command

watch-fast: .set-build-fast .watch-command

clean:
	@rm -f *{$(CLEAN_EXT)}
	@rm -f inc/*{$(CLEAN_EXT)}

dist-clean: clean
	@rm -f *.pdf
