#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include "commandes.h"
#include "gestionFichier.h"
#include "utils.h"

// Commande Touch
void cmd_touch(char cmd[255],int num, char nameMach[MPI_MAX_PROCESSOR_NAME]){
		int match;
		
		// vérifie avec regex si la commande est touch
		char *str_regex = "[ ]*touch[ ]*[-_[:alnum:]]+";
		match=execRegex(str_regex,cmd);
		if (match == 0) { 		
			printf ("Serveur %d: je crée un fichier\n", num);
			
			//Récupère le nom du fichier dans la commande
			char *split, *nameFich;
			split=strtok(cmd," ");
			nameFich=split;
			do{
				split=strtok(NULL," ");
				nameFich=split;
			} while(split != NULL && (strcmp(split,"touch")==0 || strcmp(split,"")==0) );
			
			//Mise à jour du fichier commun			
			char chemin[50];
			strcpy(chemin,nameFich);
			ajouterLigne(nameMach,chemin);
			
			//TODO: brancher le truc de joachim pour creation du fichier réel
		}
}