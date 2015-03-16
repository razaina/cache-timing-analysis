#!/bin/bash

trace="$1"
xlabel="$2"
ylabel="$3"
#png="$2"
out="plot"
out="$out \"$trace\" w l notitle"
out2="fit f(x) \"$trace\" via mean_y"
out3="plot mean_y-stddev_y w filledcurves y1=mean_y lt 1 lc rgb \"#bbbbdd\" t \"Avg - Std. dev\", \
      mean_y+stddev_y w filledcurves y1=mean_y lt 1 lc rgb \"#bbbbdd\" t \"Avg + Std. dev\", \
      mean_y w l lt 3 t \"Average nb. of cycles\", \
      \"$trace\" w l notitle"


echo $out
gnuplot -persist <<- EOF
    ${out}
    min_y = GPVAL_DATA_Y_MIN
    max_y = GPVAL_DATA_Y_MAX
    f(x) = mean_y
    ${out2}
    stddev_y = sqrt(FIT_WSSR / (FIT_NDF + 1))
    set grid
    set xlabel "${xlabel}"
    set ylabel "${ylabel}"
    set label 1 gprintf("Maximum = %g", max_y) at 2, max_y-2
    set label 2 gprintf("Minimum = %g", min_y) at 2, max_y-5
    set label 3 gprintf("Mean = %g", mean_y) at 2, max_y-8
    set label 4 gprintf("Std. deviation = %g", stddev_y) at 2, max_y-11
    set term png size 700, 400 
    set output "graph2.png"
    ${out3}
EOF
    #set term epslatex size 5,3 color colortext
    #set output "graph1.eps"
