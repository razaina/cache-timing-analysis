#!/bin/bash

trace="$1"
#Count the number of *.dat file
sum=$(ls -l $trace/*.dat | wc -l)

paste -d" " $trace/*.dat | nawk -v s="$sum" '{
    for(i=0;i<=s-1;i++)
    {
        t1 = 1+(i*1)
        temp1 = temp1 + $t1
    }
    print temp1/s
    temp1=0
}'
