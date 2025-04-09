#!/bin/bash

#number of threads:
for t in 1 2 4 8
do
    for n in 100 1000 10000 50000 100000
    do
      echo NUMT = $t
      g++ -DNUMT=$t -DNUMTRIALS=$n Project1.cpp -o Project0  -lm -fopenmp
      ./Project1
      rm ./Project1
    done
done
