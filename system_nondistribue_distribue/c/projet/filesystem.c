#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // fonction access
#include <ctype.h> // function isprint
#include <sys/wait.h>
#include <mpi.h>

#include "constante.h"
#include "functions_filesystem.h"

/**
 * Permet de modifier la valeur du dossier parent de l'environnement de l'utilisateur
 * @param c - la valeur du nouveau parent
 */
void set_parent( unsigned long long parent){
        FILE *fp = fopen(FILE_INDEX, "r+");

        if(fp == NULL){
                printf("Fichier non trouver, ou acces non permis.\n");
        }

        fseek(fp, 9, SEEK_SET); // on passe le cursor et la version

        fwrite(&parent, sizeof(parent), 1, fp);

        fclose(fp);
};

int main(int argc, char **argv){
	int fin_terminal = 0;
	char * commande; // commandes entrée dans le terminal
	char ** split_commande = malloc(256*256*sizeof(char)); // commande une fois passer a travers parsing()
	int argc_commande; // nombre de commande..
	int compteur_while = 0;

	// Options // se referer a l'exemple testopt.c
	int iflags = 0; // initialisation : crée les deux premier fichiers
	int c; // case variable
	opterr = 0;

	// on determine les options
	while ((c = getopt (argc, argv, "i")) != -1){
			switch (c){
					case 'i': // initialisation
							iflags = 1;
							break;
					case '?': // cas erreurs
							if (isprint (optopt))
									fprintf (stderr, "Unknown option `-%c'.\n", optopt);
							else
									fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
							return 1;
					default:
							abort ();
			}
	}

	if(iflags){ // initialisation
			//////
			// on crée les différents fichiers
			// on verifie qu'il y a assez de place sur le disque
			// oué.. on verra ###
			// on verifie que des fichiers n'ont pas déjà ce nom
			init_bdd();
	}
	
	if( access( FILE_INDEX, R_OK|W_OK ) == -1 ) { // si il existe pas
		printf("le fichier \"%s\" n'existe pas, ou pas de droits sur le fichier.\n", FILE_INDEX);
		return 1;
	}

	if( access( FILE_STOCK, R_OK|W_OK ) == -1 ) { // si il existe pas
		printf("le fichier \"%s\" n'existe pas, ou pas de droits sur le fichier.\n", FILE_STOCK);
		return 1;
	}

	// réinitialisation du curseur
	set_parent((unsigned long long) 0); // on remet le curseur a 0

	while(!fin_terminal && compteur_while < 100){
			compteur_while++;
			// on demande la commande suivante
			printf("user@machine:pwd$ ");
			commande = inputString(stdin, 10);

			// on verifie si la personne ne veux pas sortir du terminal
			if(!strcmp(commande, "exit")){
					fin_terminal = 1;
			} else {
					parsing(commande, split_commande, &argc_commande);

					char concat[256] = PATH_BIN_FILE;//"/home/joachim/system/c/projet/bin/";
					strcat(concat, split_commande[0] );
					//printf("%s\n", concat);
					split_commande[0] = concat;

					exec_prog(split_commande);
					int status = 0;
					wait(&status); // on attend la fin des forks
			}
	}

	free(commande); // on libere la place alloué pour la commande
	return EXIT_SUCCESS;
}
