#!/bin/bash

mpicc -std=c99 -lm -Wall projet.c gestionFichier.c utils.c commandes.c functions_filesystem.c get_env.c -o projet 
mpirun -np $1 ./projet $2

