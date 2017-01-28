#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constante.h"
#include "../get_env.h"

int main(int argc, char **argv){
	/**
	 *	Argument : vim NomDuFichier
	 **/

	if(argc < 2){
		printf("Usage : vim <filename>\n");
		return EXIT_FAILURE;
	}

	FILE *fp = fopen(FILE_INDEX,"r+"); // r+ c'est read and write, si tu met w, c'est read and write mais ça écrase le fichier
	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
		return EXIT_FAILURE;
	}

	unsigned long long cursor;		

	// valeur a rajouter dans le fichier
	char active; // octet active
	unsigned long long parent; // parent est envoyé en argument de l'executable, pour le moment se sera 0 // ### a changer
	get_parent(&parent);
	char file_type;
	char file_name[255]; // le nom du fichier // ### quand un ficheir est creer avec touch, on n'écris rien dans stockage.jjg, dés qu'il y a du contenue on remplacera cette valeur..
	unsigned long long file_cursor_stock;
	unsigned long long file_size;

	strcpy(file_name, argv[1]); // on copie la chaine de charactere dans file_name pour être sur d'avoir les 255 char

	cursor = file_exist(FICHIER, file_name);

	if(cursor !=0){
		fseek(fp, cursor, SEEK_SET); // on place le cursor au bon endroit.
		//	On crée le fichier fichier sur le filesystem hote.
			// On recupere les infos.
			fread(&active, sizeof(active), 1, fp);
			fread(&parent, sizeof(parent), 1, fp);
			fread(&file_type, sizeof(file_type), 1, fp);
			fread(file_name, sizeof(file_name), 1, fp);
			fread(&file_cursor_stock, sizeof(file_cursor_stock), 1, fp);
			fread(&file_size, sizeof(file_size), 1, fp);

			char concat[4096];                                                           			
                        strcat(concat, PATH_FOLDER_SWAP); // on met le chemin vers le dossier de swap
                        strcat(concat, file_name); // on rajoute le nom du fichier
                        strcat(concat, ".swp");  // on rajotue l'extension .swp

			// on regarde si le fichier est vide.
			if(file_cursor_stock == 0) { // le fichier n'as jamais été instancié. Donc on crée le fichier sur le filesystem hôte
				printf("le fichier n'as pas été instancié .\n");
				FILE *fp_swp = fopen(concat, "w+"); // on crée le fichier.

				fclose(fp_swp);		
				

			} else { // le fichier à déjà été instancié , on va devoir écrire dans le .swp, ce qu'il y a déjà d'écrit sur le filesystem
				
			}

		//	On ouvre le fichier ainsi crée grace a vim
			char command[4096];
			snprintf(command, sizeof(command), "vim %s", concat); 
			system(command);

		//	Une fois le fichier éditer, on écrit le résultat dans notre filesystem
			unsigned long long cursor_stock;
			get_stock_last_cursor(&cursor_stock);
			printf("cursor = %llu\n",cursor_stock);
			put_file(concat,cursor_stock, &file_size);
			
			// on change le fichier dans index, avec la nouvelle taille et l'endroit ou on le met.. ###

			ajouterLigne(cursor, active, parent, file_type, file_name, file_cursor_stock, file_size);
		
		//	On supprimer le fichier dans le filesystem hote
		
	
				

		printf("Le fichier : \"%s\" existe déjà. cursor = %llu \n", file_name, cursor);
	} else {	
		printf("Le fichier n'existe pas.\n");
	}
	fclose(fp);

	return EXIT_SUCCESS;
}
