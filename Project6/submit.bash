#!/bin/bash
#SBATCH  -J  Proj06
#SBATCH  -A  cs475-575
#SBATCH  -p  classgpufinal
#SBATCH  --constraint=v100
#SBATCH  --gres=gpu:2
#SBATCH  -o  proj06.out
#SBATCH  -e  proj06.err
#SBATCH  --mail-type=BEGIN,END,FAIL
#SBATCH  --mail-user=xingru@oregonstate.edu

for s in 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304
do
        for b in 8 32 64 128 256
        do
                g++ -DDATASIZE=$s -DLOCALSIZE=$b -o proj06 proj06.cpp /usr/local/apps/cuda/11.7/lib64/libOpenCL.so.1  -lm -fopenmp
                ./proj06
        done
done
