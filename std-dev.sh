#!/bin/bash

trace="$1"
png="$2"
out="plot"
out="$out \"$trace\" title \"$trace\""
out2="fit f(x) \"$trace\" via mean_y"
out3="mean_y w l lt 3, \"$trace\" w p pt 7 lt 1 ps 1"

echo $out
gnuplot -persist <<- EOF
    reset
    ${out}
    unset table
    min_y = GPVAL_DATA_Y_MIN
    max_y = GPVAL_DATA_Y_MAX
    f(x) = mean_y
    ${out2}
    stddev_y = sqrt(FIT_WSSR / (FIT_NDF + 1))
    set label 1 gprintf("Mean = %g", mean_y) at 2, max_y-350
    set label 2 gprintf("Standard deviation = %g", stddev_y) at 2, max_y-400
    plot mean_y-stddev_y with filledcurves y1=mean_y lt 1 lc rgb "#bbbbdd", \
    mean_y+stddev_y with filledcurves y1=mean_y lt 1 lc rgb "#bbbbdd", \
    ${out3}
EOF

