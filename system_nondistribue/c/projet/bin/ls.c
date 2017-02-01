#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constante.h"
#include "../get_env.h"

int main(int argc, char ** argv){

	FILE *fp = fopen(FILE_INDEX, "r");
	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
		return EXIT_FAILURE;
	}

    unsigned long long cursor_end;
    unsigned long long cursor_tmp = FILE_BASE_ENV;
	unsigned long long parent;

	fseek(fp, 0,  SEEK_SET);
	fread(&cursor_end, sizeof(cursor_end), 1, fp);
	get_parent(&parent);

	fseek(fp, cursor_tmp,  SEEK_SET);
	
	char active;
	unsigned long long tmp_parent;
	char type_file;
	char file_name[255];
	char char_type_file;
	unsigned long long file_cursor_stock;
	unsigned long long file_size;

	while(cursor_tmp < cursor_end){
		fread(&active, sizeof(active), 1, fp);
		fread(&tmp_parent, sizeof(tmp_parent), 1, fp);
		fread(&type_file, sizeof(type_file), 1, fp);
		if(active && tmp_parent == parent){ // on verifie qu'il est actif, et qu'il est bien dans le repertoire courant
			fread(file_name, sizeof(file_name), 1, fp);
			// on affiche les infos 
			if(type_file == FICHIER){ // fichier
				//fseek(fp, sizeof(unsigned long long), SEEK_CUR); // on saute l'endroit ou est stocké le fichier
				fread(&file_cursor_stock, sizeof(file_cursor_stock), 1, fp);
				fread(&file_size, sizeof(file_size), 1, fp);
				char_type_file = 'f';	
				
			} else {
				char_type_file = 'd';
				file_size = 1; // on met file_size a 1...
			}
			
			// ### peut être plus de différence sur file_size.. un if en plus..
			printf("%c\t%s\t%llu\t%llu\n", char_type_file, file_name, file_cursor_stock,file_size); 
		}

		cursor_tmp += INDEX_LINE_SIZE;
		fseek(fp, cursor_tmp, SEEK_SET); // on place le curseur au bon endroit pour la suite ( parce qu'on ne lis pas tout.. surtout si c'est un fichier..

	}

	fclose(fp);

	return EXIT_SUCCESS;
}
