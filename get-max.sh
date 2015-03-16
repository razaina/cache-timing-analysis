#!/bin/bash

col=$1
in=$4
col1=$2
col2=$3

sort -k$col -n $in | tail -1
sort -k$col1 -n $in | head -1
sort -k$col2 -n $in | tail -1
