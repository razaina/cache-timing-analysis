#!/bin/bash

from=0
to=0
file=$1
dir=$2
numoflines=$(wc -l < $file)
from=$(((numoflines-(256*16))+1))
echo "NumOfLines = $numoflines\n"
for i in `seq 0 15`;
do
    to=$((from+255))
    echo "From: $from to: $to\n"
    sed -n $from,$((to))p $file > $dir/$i.profile 
    from=$((from+256))    
done
