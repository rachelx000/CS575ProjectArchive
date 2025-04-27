#!/bin/bash

g++ -DNUMT=$t -DNUMTRIALS=$n Project2.cpp -o Project2  -lm -fopenmp
./Project2
rm ./Project2
