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
		return EXIT_FAILURE;
	}

	char file_name[255];
	strcpy(file_name, argv[1]);

	char active = 0;

	unsigned long long tmp_cursor = file_exist(FICHIER, file_name);
		
	if(tmp_cursor != 0){ // le fichier existe
		fseek(fp, tmp_cursor, SEEK_SET); // je vais sur la ligne du fichier
		fwrite(&active, sizeof(active), 1, fp); //je met le char active a 0.
	} else {
		printf("Le dossier : %s, n'existe pas.\n", file_name);	
	}

	fclose(fp);

	return EXIT_SUCCESS;
}
