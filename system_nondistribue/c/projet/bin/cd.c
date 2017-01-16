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

	unsigned long long cursor_end;	
        unsigned long long cursor_tmp = FILE_BASE_ENV;

	fseek(fp, 0,  SEEK_SET);
	fread(&cursor_end, sizeof(cursor_end), 1, fp);

	fseek(fp, cursor_tmp,  SEEK_SET);
	
	int finish = 0;

	char active;
	unsigned long long parent;
	char type_file;
	char file_name[255];
	
	printf("cursor : %llu\n", cursor_tmp);

	while(cursor_tmp < cursor_end && !finish){

		fread(&active, sizeof(active), 1, fp);
		fread(&parent, sizeof(parent), 1, fp);
		fread(&type_file, sizeof(type_file), 1, fp);
		if(active && !type_file){
			
			printf("Je suis un dossier et actif !\n");
		}

		if(type_file){
			cursor_tmp += 1+8+1+255+8+8;
		} else {
			cursor_tmp += 1+8+1+255;
		}

		fseek(fp, cursor_tmp, SEEK_SET);

	}

	return EXIT_SUCCESS;
}
