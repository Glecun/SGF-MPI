#Foncia
mpicc -std=c99 -lm -Wall $1.c gestionFichier.c -o $1 
mpirun -n $2 -host $3 ./$1

#Université
#!/bin/bash -l
#mpicc -std=c99 -lm -wall $1.c -o $1 
#mpirun -n $2 -H $3 ./$1
