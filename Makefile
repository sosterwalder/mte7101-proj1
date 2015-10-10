# -*- coding: UTF-8 -*-
# vim: autoindent expandtab tabstop=4 sw=4 sts=4 filetype=make

.PHONY: doc doc-watch

doc:
	make -C doc clean
	make -C doc build

doc-watch:
	make -C doc watch