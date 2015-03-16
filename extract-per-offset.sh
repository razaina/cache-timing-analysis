#!/bin/bash
file=$1
dir=$2
for i in $(seq 40 90);
do
    grep "$i\$" $file > $dir/nbhits_$i 
done
