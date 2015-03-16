#!/bin/bash

trace="$1"
out="plot"
out="$out \"$trace\" title \"$trace\""
out2="plot \"$trace\" u (\$1 == min_y ? \$1 : 1/0):1"
out3="plot \"$trace\" u (\$1 == max_y ? \$1 : 1/0):1"
out4="plot \"$trace\" w p pt 6"

echo $out2
gnuplot -persist <<- EOF
    ${out}
    min_y = GPVAL_DATA_Y_MIN
    max_y = GPVAL_DATA_Y_MAX
    ${out2}
    min_pos_x = GPVAL_DATA_Y_MIN
    ${out3}
    max_pos_x = GPVAL_DATA_Y_MAX
    set arrow 1 from min_pos_x, min_y-5 to min_pos_x, min_y lw 0.5
    set arrow 2 from max_pos_x, max_y+5 to max_pos_x, max_y lw 0.5
    set label 1 'Minimum' at min_pos_x, min_y-25 centre
    set label 2 'Maximum' at max_pos_x, max_y+20 centre
    ${out4}
EOF
