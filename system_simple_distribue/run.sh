#Université
#!/bin/bash -l
#mpicc -std=c99 -lm -Wall projet.c gestionFichier.c utils.c commandes.c -o projet 
#mpirun -n $2 -H $3 ./projet

#Foncia
mpicc -std=c99 -lm -Wall projet.c gestionFichier.c utils.c commandes.c -o projet
mpirun -n $2 -host $3 ./projet