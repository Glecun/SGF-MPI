#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constante.h"
#include "../get_env.h"

/**
	A faire : 
		- gerer le .. , retour au dossier parent
**/

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

        unsigned long long cursor_end;
        unsigned long long cursor_tmp = FILE_BASE_ENV;
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

	fseek(fp, 0,  SEEK_SET);
	fread(&cursor_end, sizeof(cursor_end), 1, fp);
	get_parent(&parent);

	fseek(fp, cursor_tmp,  SEEK_SET);
	
	int finish = 0;

	char active;
	unsigned long long tmp_parent;
	char type_file;
	char file_name[255];
	
	printf("cursor : %llu\n", cursor_tmp);

	while(cursor_tmp < cursor_end && !finish){

		fread(&active, sizeof(active), 1, fp);
		fread(&tmp_parent, sizeof(tmp_parent), 1, fp);
		fread(&type_file, sizeof(type_file), 1, fp);
		if(active && !type_file && tmp_parent == parent){ // on verifie que c'est un dossier, qu'il est actif, et qu'il est bien dans le repertoire courant
			fread(file_name, sizeof(file_name), 1, fp);			

			if(strcmp(folder_name, file_name) == 0){ // si les deux fichiers on le même nom
				// j'écris dans l'envirronnement que le parent a changer
				set_parent(cursor_tmp);
				finish = 1; // on a trouver ce qu'on cherchait on finis l'exécution.
			}
		}

		if(type_file){
			cursor_tmp += 1+8+1+255+8+8;
		} else {
			cursor_tmp += 1+8+1+255;
		}

		fseek(fp, cursor_tmp, SEEK_SET); // on place le curseur au bon endroit pour la suite ( parce qu'on ne lis pas tout.. surtout si c'est un fichier..

	}

	if(!finish){
		printf("Le dossier : %s, n'existe pas.\n", folder_name);
	}

	fclose(fp);

	return EXIT_SUCCESS;
}
