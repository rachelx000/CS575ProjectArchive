#!/bin/bash

#number of threads:
for t in 1 2 4 6 8
do
    for n in 1 10 100 1000 10000 100000 500000
    do
      # echo NUMT = $t
      g++ -DNUMT=$t -DNUMTRIALS=$n Project1.cpp -o Project1  -lm -fopenmp
      ./Project1
      rm ./Project1
    done
done
