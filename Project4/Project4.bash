#!/bin/bash

for n in 4 40 400 1000 4000 10000 40000 80000 100000 400000 800000 1000000 2000000 4000000 8000000
#for n in 10 100 1000 5000
do
  g++   proj04.cpp  -DARRAYSIZE=$n  -o proj04  -lm  -fopenmp
  ./proj04
  rm ./proj04
done
