# -*- coding: UTF-8 -*-
# vim: autoindent expandtab tabstop=4 sw=4 sts=4 filetype=make

.PHONY: build clean doc doc-watch

build:
	cd build/ && cmake ../src && make

clean:
	rm -rf build/*

doc:
	make -C doc clean
	make -C doc build

doc-watch:
	make -C doc watch
