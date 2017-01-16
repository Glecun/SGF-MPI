#include <stdio.h>
#include <stdlib.h>
#include "../constante.h"


int main(int argc, char ** argv){
	/*
		Affiche les informations du repertoire courant.
	
	*/

	FILE *fp = fopen("/home/joachim/system/c/projet/index.jjg","r");

	unsigned long long cursor_end;
	unsigned long long cursor_tmp;
	char version;

	fseek(fp, 0, SEEK_SET); // on met le curseur a o
	fread(&cursor_end, sizeof(cursor_end), 1, fp);
	fread(&version, sizeof(version), 1, fp);
//	cursor_tmp = 8 + 1 + 8 + 3 * ( 1 + 8 + 255 );
	cursor_tmp = FILE_BASE_ENV;

	char active;
	unsigned long long parent;
	char type_file;
	char file_name[255];
	unsigned long long cursor_stock;
	unsigned long long length_file;
	
	printf("cursor : %llu", cursor_tmp);
	fseek(fp, cursor_tmp, SEEK_SET);

	while(cursor_tmp < cursor_end){
		printf("Here ?\n");
		fread(&active , sizeof(active), 1, fp);
		if(active == 1){
			printf("yes\n");
			// le fichier est bien actif, donc nous allons le marquer
			fread(&parent, sizeof(parent), 1, fp);
			fread(&type_file, sizeof(type_file), 1, fp);
			fread(&file_name, sizeof(file_name), 1, fp);
			if(type_file == 1){ // si c'est un fichier..
				fread(&cursor_stock, sizeof(cursor_stock), 1, fp);
				fread(&length_file, sizeof(length_file), 1, fp);
				printf("f : %s\n", file_name);	
				cursor_tmp += 1 + 8 + 1 + 255 + 8 + 8;
			} else {
				printf("d : %s\n", file_name);	
				cursor_tmp += 1 + 8 + 1 + 255;
			}
		} else {
			fread(&parent, sizeof(parent), 1, fp);
			fread(&type_file, sizeof(type_file), 1, fp);
			if(type_file){ // c'est un fichier
				cursor_tmp += 1 + 8 + 1 + 255 + 8 + 8;		
			} else {
				cursor_tmp += 1 + 8 + 1 + 255;
			}
		}

	}

	printf("cursor = %llu\n", cursor_end);

	fclose(fp);

	return EXIT_SUCCESS;
}
