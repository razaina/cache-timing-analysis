#!/bin/bash

dir="$1"
xlabel="All possible values for a byte"
ylabel="Avg number of cycles (among all bytes)"

gnuplot <<- EOF
    set term postscript size 10,50 eps enhanced color font 'Helvetica,20' linewidth 2
    set output '$2_1.eps'
    set origin 0,0
    set multiplot layout 16,1 
    set style fill solid border -1
    set xrange [0:256]
    set grid
    set format x "0x%X"
    plot "$dir/0.profile" u 3:7 w boxes notitle 
    plot "$dir/1.profile" u 3:7 w boxes notitle 
    plot "$dir/2.profile" u 3:7 w boxes notitle 
    plot "$dir/3.profile" u 3:7 w boxes notitle 
    plot "$dir/4.profile" u 3:7 w boxes notitle 
    unset multiplot
EOF


gnuplot <<- EOF
    set term postscript size 10,50 eps enhanced color font 'Helvetica,20' linewidth 2
    set output '$2_2.eps'
    set origin 0,0
    set multiplot layout 16,1 
    set style fill solid border -1
    set xrange [0:256]
    set grid
    set format x "0x%X"
    plot "$dir/5.profile" u 3:7 w boxes notitle 
    plot "$dir/6.profile" u 3:7 w boxes notitle 
    plot "$dir/7.profile" u 3:7 w boxes notitle 
    plot "$dir/8.profile" u 3:7 w boxes notitle 
    plot "$dir/9.profile" u 3:7 w boxes notitle 
    unset multiplot
EOF


gnuplot <<- EOF
    set term postscript size 10,50 eps enhanced color font 'Helvetica,20' linewidth 2
    set output '$2_3.eps'
    set origin 0,0
    set multiplot layout 16,1 
    set style fill solid border -1
    set xrange [0:256]
    set grid
    set format x "0x%X"
    plot "$dir/10.profile" u 3:7 w boxes notitle 
    plot "$dir/11.profile" u 3:7 w boxes notitle 
    plot "$dir/12.profile" u 3:7 w boxes notitle 
    plot "$dir/13.profile" u 3:7 w boxes notitle 
    plot "$dir/14.profile" u 3:7 w boxes notitle 
    plot "$dir/15.profile" u 3:7 w boxes notitle 
    unset multiplot
EOF

