#Université
#!/bin/bash -l
#mpicc -std=c99 -lm -Wall projet.c gestionFichier.c utils.c commandes.c -o projet 
#mpirun -np $1 -H $2 ./projet

#Foncia
mpicc -std=c99 -lm -Wall projet.c gestionFichier.c utils.c commandes.c -o projet
mpirun -np $1 -host $2 ./projet