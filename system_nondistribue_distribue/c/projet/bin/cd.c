#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constante.h"
#include "../get_env.h"

int main(int argc, char ** argv){
	if(argc < 2){
		printf("Usage : cd <foldername>\n");
		return EXIT_FAILURE;
	}

	FILE *fp = fopen(FILE_INDEX, "r+");
	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
	}

	char folder_name[255];
	strcpy(folder_name, argv[1]);
	unsigned long long parent;

	// on gere le retour au dossier parent
        if(strcmp(folder_name, "..") == 0){
        	get_parent(&parent);// lire le dossier actuel (get_parent) 
        	if(parent != 0){// si 0, rien faire // sinon, aller au pointeur, et lire son parent.
			fseek(fp, parent + 1, SEEK_SET); // on va au bon endroit, le +1 c'est pour l'active
			fread(&parent, sizeof(parent), 1, fp);
			set_parent(parent);// changer la valeur du parent environnement
		}
		fclose(fp);
        	return EXIT_SUCCESS;
        }

	unsigned long long tmp_cursor = file_exist(DOSSIER, folder_name);
	
	if(tmp_cursor != 0){ // le dossier existe
		set_parent(tmp_cursor); // on met le parent a jour.
	} else {
		printf("Le dossier : %s, n'existe pas.\n", folder_name);	
	}

	fclose(fp);

	return EXIT_SUCCESS;
}
