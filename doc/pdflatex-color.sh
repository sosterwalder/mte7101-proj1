#!/bin/bash
GREP="grep --color=always --line-buffered"
(/usr/bin/pdflatex -file-line-error $* 2>&1) \
 | $GREP -vP "^\s*((\[|\]|\(|\))\s*)+$" \
 | GREP_COLOR="01;31" $GREP -P "(^[^:]*:\d+: )|(^l\.\d+ )|(^! LaTeX Error: )|$" \
 | GREP_COLOR="00;33" $GREP -P "(LaTeX Warning(:|))|(Package [^\s]+ Warning(:|))|$" \
 | GREP_COLOR="00;32" $GREP -P "^Output written on .*\.pdf \(.*\)\.$|$" \
 | uniq
