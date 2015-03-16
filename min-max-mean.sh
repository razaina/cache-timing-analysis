#!/bin/bash

trace="$1"
png="$2"
out="plot"
out="$out \"$trace\" title \"$trace\""
out2="fit f(x) \"$trace\" via mean_y"
out3="\"$trace\" w p pt 7 lt 1 ps 1"

echo $out
gnuplot -persist <<- EOF
    ${out}
    min_y = GPVAL_DATA_Y_MIN
    max_y = GPVAL_DATA_Y_MAX
    f(x) = mean_y
    ${out2}
    set label 1 gprintf("Minimum = %g", min_y) at 2, min_y-4
    set label 2 gprintf("Maximum = %g", max_y) at 2, max_y+5
    set label 3 gprintf("Mean = %g", mean_y) at 2, max_y+15
    plot min_y with filledcurves y1=mean_y lt 1 lc rgb "#bbbbdd", \
    max_y with filledcurves y1=mean_y lt 1 lc rgb "#bbddbb", \
    ${out3}
EOF
