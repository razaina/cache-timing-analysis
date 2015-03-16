#!/bin/bash

trace="$1"
png="$2"
out="plot"
out="$out \"$trace\" title \"$trace\" w p lt 1 pt 7 ps 1"

echo $out
gnuplot -persist <<- EOF
    ${out}
    linear(start,slope,x)=start+slope*x
    print start, slope
EOF
