#!/bin/bash

from=0
to=0
file=$1
dir=$2
numoflines=$(wc -l < $file)
split=$(((numoflines/4096)-1))
from=1
jump=4096
echo "[+] NumOfLines = $numoflines"
echo "[+] Split = $split"
for j in `seq 0 $split`
do
    mkdir $dir/$j
    for i in $(seq 0 15);
    do
        if [ ${i} -eq 0 ]; then
            to=$((from+255))
        else
            to=$((from+255))
        fi
        echo "[+]From: $from to: $to"
        sed -n $from,$((to))p $file > $dir/$j/$i.profile 
        from=$((to+1))    
    done
done
