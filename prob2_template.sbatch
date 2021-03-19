#!/bin/bash
# Choose the partition that you are gonna use
# You can choose: parallel_programming_cpu OR debug_5min OR debug OR others 
#SBATCH --partition=parallel_programming_cpu
#SBATCH --exclusive
#SBATCH --ntasks=16
#SBATCH --ntasks-per-node=16
#SBATCH --chdir=/home/user007/HW3/Problem_2
#SBATCH --output=/home/user007/HW3/Problem_2/hw2_%J_stdout.txt
#SBATCH --error=/home/user007/HW3/Problem_2/hw2_%J_stderr.txt
# Adjust time according to your needs and queue limit 
#SBATCH --time=00:15:00
#SBATCH --job-name=hw3-p2
#SBATCH --mail-user=youremail@ou.edu
#SBATCH --mail-type=ALL

#################################################

echo "**Prob 2 - Part a - 1M - 1 threads**"
./kmeans_parallel_a 1000000 points_1M.csv 1 16 initial_centroid.csv final_a_1M_th1_centroid_values.csv time_a_1M_th1.csv