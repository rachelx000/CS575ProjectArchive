#!/bin/bash

#SBATCH -J Fourier
#SBATCH -A cs475-575
#SBATCH -p classmpifinal
#SBATCH -N 8
#SBATCH -n 8
#SBATCH -o fourier.out
#SBATCH -e fourier.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=xingru@oregonstate.edu

module load slurm
module load openmpi
mpic++ proj07.cpp -o proj07 -lm
for p in 1 2 4 6 8
do
    mpiexec -mca btl self,tcp -np $p ./proj07
    echo " "
done
rm ./proj07
