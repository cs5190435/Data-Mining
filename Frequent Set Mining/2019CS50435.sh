#!/bin/bash

if [ "$#" -eq 3 ]; then 
    #echo "in 1"
    ./w2 $1 $2 $3
    python3 plott.py $3
    
elif [ "$#" -eq 4 ]; then
    #echo "in 2"
    ./w1 $1 $2 $3 $4

else
    echo "Insufficient number of arguments."
fi