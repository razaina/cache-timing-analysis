#!/bin/bash
#Usage
# ./display-hist.sh PATH_TO_HIST "[x1:x2]" OUTPUT_FILE_NAME
#e.g display hist from 0 to 1000
# ./display-hist.sh dir/hist_1 "[0:1000]" my_hist1
# An image (*.png) will be created.
# If you want to display the result in Gnuplot instead of having an image as an output
#just remove/comment the following lines:
# set term png size 700, 400
# set output "$3.png"
trace="$1"
x_range="$2"
out="$3"
echo $trace

gnuplot -persist <<- EOF
    set xrange $x_range
    plot "$trace" 
    set term png size 700, 400 
    set output "$3.png"
    min_y = GPVAL_DATA_Y_MIN
    max_y = GPVAL_DATA_Y_MAX
    set grid
    set style fill solid border -1
    set label 2 gprintf("Maximum = %g", max_y) at 630, max_y-2050 
    plot "$trace" w boxes notitle
EOF

