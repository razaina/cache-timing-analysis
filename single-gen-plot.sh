#!/bin/bash

trace="$1"
xlabel="$2"
ylabel="$3"
#png="$2"
out="plot"
out="$out \"$trace\" w l notitle"

echo $out
gnuplot -persist <<- EOF
    set grid
    set xlabel "${xlabel}"
    set ylabel "${ylabel}"
    set term png size 700, 400
    set output "graph.png"
    ${out}
EOF
    #set term epslatex size 5,3 color colortext
    #set output "graph.eps"
