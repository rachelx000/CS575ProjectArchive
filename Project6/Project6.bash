#!/bin/bash
for s in 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304
do
        for b in 8 16 32 64 128 256
        do
                g++ -DDATASIZE=$s -DLOCALSIZE=$b -o proj06 proj06.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1  -lm -fopenmp
                ./proj06
                rm ./proj06
        done
done
