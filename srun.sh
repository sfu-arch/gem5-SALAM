#!/bin/sh
#
#SBATCH --cpus-per-task=8
#SBATCH --time=120:00
#SBATCH --mem=40G
srun /localhome/mha157/gem5-SALAM/ad.sh -b sddmm