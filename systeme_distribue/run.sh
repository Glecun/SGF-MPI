#!/bin/bash

mpicc -std=c99 -lm -Wall projet.c commandes.c functions_filesystem.c get_env.c -o projet

if [ "$#" -eq 1 ]; then
	mpirun -np $1 ./projet localhost
fi

if [ "$#" -eq 2 ]; then
	if [ "$2" = "-i" ]; then
		mpirun -np $1 ./projet $2
	else
		mpirun -np $1 -H $2 ./projet
	fi
fi

if [ "$#" -eq 3 ]; then
	mpirun -np $1 -H $2 ./projet $3
fi
