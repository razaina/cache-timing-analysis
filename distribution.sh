#!/bin/bash

trace="$1"
output=""
out="plot"

out="$out \"$trace\" u (\$1):(1) smooth freq t 'Smooth frequency' w boxes, \"$trace\"  u (\$1):(1) smooth cumulative w l t 'Smooth cumulative'"

echo $out
gnuplot -persist <<- EOF
    ${out}
EOF    

#gnuplot -persist <<- EOF
    ##set terminal png size 450, 360 small \n
    #set output 'output.png' \n
    #${out}
#EOF    

