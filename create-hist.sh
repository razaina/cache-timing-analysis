#!/bin/bash
#Usage
# ./create-hist.sh PATH_TO_TRACE PATH_TO_DIRECTORY_OUTPUT
cat $1 | awk -v colonne=1 -v largeur=1 -f hist.awk > $2/hist_1
cat $1 | awk -v colonne=2 -v largeur=1 -f hist.awk > $2/hist_2
#cat $1 | awk -v colonne=3 -v largeur=1 -f hist.awk > $2/hist_3
#cat $1 | awk -v colonne=4 -v largeur=1 -f hist.awk > $2/hist_4
