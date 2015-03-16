#!/bin/bash

dir="$1"
#png="$2"
xlabel="$2"
ylabel="$3"
out="plot"
i=0
for entry in "$dir"/*
do 
    if [ "$i" -eq 0 ]; then
        out="$out \"$entry\" w l notitle"
    else
        out="$out, \"$entry\" w l notitle"
    fi
    i=$((i+1))    
done

echo $out
gnuplot -persist <<- EOF
    set grid
    set xlabel "${xlabel}"
    set ylabel "${ylabel}"
    set term png size 700, 400 
    set output "graph1.png"
    ${out}
EOF
    #set term epslatex size 5,3 color colortext
    #set output "graph1.eps"
