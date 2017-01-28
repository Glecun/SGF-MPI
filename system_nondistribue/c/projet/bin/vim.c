#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constante.h"
#include "../get_env.h"

int main(int argc, char **argv){
	// Usage
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

			// on creer le fichier
                        FILE *fp_swp = fopen(concat, "w+"); // on crée le fichier.
			fclose(fp_swp);		


			// on regarde si le fichier n'est pas vide, on le remplis. // oué oué ça fait sens
			//if(file_cursor_stock != 0 && file_size != 0) {
			extract_file(concat, file_cursor_stock, file_size);					
			//}

		//	On ouvre le fichier ainsi crée grace a vim
			char command[4096];
			snprintf(command, sizeof(command), "vim %s", concat); 
			system(command);

		//	Une fois le fichier éditer, on écrit le résultat dans notre filesystem
			unsigned long long cursor_stock;
			get_stock_last_cursor(&cursor_stock);
			put_file(concat,cursor_stock, &file_size);
		
			printf("Size = %llu", file_size);
	
			// on change le fichier dans index, avec la nouvelle taille et l'endroit ou on le met.. ###

			ajouterLigne(cursor, active, parent, file_type, file_name, cursor_stock, file_size);
		
		//	On supprimer le fichier dans le filesystem hote
				

	} else {	
		printf("Le fichier \"%s\" n'existe pas.\n", file_name);
	}
	fclose(fp);

	return EXIT_SUCCESS;
}
