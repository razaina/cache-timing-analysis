#!/bin/bash
file=$1
dir=$2
for i in $(seq 0 7);
do
    grep " $i " $file > $dir/offset_$i
done
