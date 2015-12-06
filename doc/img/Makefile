# -*- coding: UTF-8 -*-
# vim: autoindent expandtab tabstop=4 sw=4 sts=4 filetype=make

.PHONY: all watch-asy

all: watch-asy

watch-asy:
	fswatch -0 *.asy |xargs -0 -n 1 asy
