#Université
#!/bin/bash -l
mpicc -std=c99 -lm -Wall projet.c gestionFichier.c utils.c commandes.c functions_filesystem.c get_env.c -o projet 
#mpirun -np $1 -H $2 ./projet $3
mpirun -np $1 ./projet $2

#Foncia
#mpicc -std=c99 -lm -Wall projet.c gestionFichier.c utils.c commandes.c -o projet
#mpirun -np $1 -host $2 ./projet
