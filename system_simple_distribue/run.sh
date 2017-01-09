#!/bin/bash -l
mpicc -std=c99 -lm -Wall $1.c -o $1 
mpirun -n $2 -H $3 ./$1
