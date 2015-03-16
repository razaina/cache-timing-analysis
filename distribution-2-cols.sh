#!/bin/bash

trace="$1"

gnuplot -persist <<- EOF
set grid
intervals = 100
plot "$trace" u 1
min = GPVAL_DATA_Y_MIN
max = GPVAL_DATA_Y_MAX
width = (max_y - min_y)/intervals
bin(x,width) = width*floor(x/width) + width/2.0
set xtics min,(max-min)/10,max
set boxwidth width
set style fill solid 0.5 border
set ylabel 'Frequency'
set ytics nomirror
plot "$trace" using (bin(
EOF    

