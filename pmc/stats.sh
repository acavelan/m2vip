#!/bin/bash

for i in $(ls times); do
    SUM=0
    for j in $(cat times/$i); do
        SUM=$(($SUM+$j))
    done
    MOY=$(($SUM / 10))
    echo $i $MOY
done
