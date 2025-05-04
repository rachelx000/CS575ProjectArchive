#!/bin/bash

g++   proj04.cpp  -DARRAYSIZE=$n  -o proj04  -lm  -fopenmp
./proj04
rm ./proj04
