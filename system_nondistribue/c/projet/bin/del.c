#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constante.h"
#include "../get_env.h"

int main(int argc, char ** argv){

	if(argc < 2){
		printf("Usage : del <filename>\n");
		return EXIT_FAILURE;
	}

	FILE *fp = fopen(FILE_INDEX, "r+");
	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
	}

	char file_name_d[255];
	strcpy(file_name_d, argv[1]);

        unsigned long long cursor_end;
        unsigned long long cursor_tmp = FILE_BASE_ENV;
	unsigned long long parent;

	fseek(fp, 0,  SEEK_SET);
	fread(&cursor_end, sizeof(cursor_end), 1, fp);
	get_parent(&parent);

	fseek(fp, cursor_tmp,  SEEK_SET);
	
	int finish = 0;

	char active;
	unsigned long long tmp_parent;
	char type_file;
	char file_name[255];	

	while(cursor_tmp < cursor_end && !finish){

		fread(&active, sizeof(active), 1, fp);
		fread(&tmp_parent, sizeof(tmp_parent), 1, fp);
		fread(&type_file, sizeof(type_file), 1, fp);
		if(active && type_file && tmp_parent == parent){ // on verifie que c'est un fichier, qu'il est actif, et qu'il est bien dans le repertoire courant
			fread(file_name, sizeof(file_name), 1, fp);			

			if(strcmp(file_name_d, file_name) == 0){ // si les deux fichiers on le même nom
				// j'écris dans l'envirronnement que le parent a changer
				fseek(fp, cursor_tmp, SEEK_SET); // on remet au début des info fichier
				char active = 0;
				fwrite(&active, sizeof(active), 1, fp); // on met le flag active a 0
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
		printf("Le dossier : %s, n'existe pas.\n", file_name_d);
	}

	fclose(fp);

	return EXIT_SUCCESS;
}
