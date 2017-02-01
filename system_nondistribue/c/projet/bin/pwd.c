#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constante.h"
#include "../get_env.h"

int main(int argc, char ** argv){

	unsigned long long parent;
	get_parent(&parent);

	FILE *fp = fopen(FILE_INDEX, "r");
        if(fp == NULL){
        	printf("Fichier non trouver, ou acces non permis.\n");
			return EXIT_FAILURE;
        }

	int finish = 0;
	char path[4096] = "";

	char active;
    unsigned long long tmp_parent;
    char type_file;
    char folder_name[255];
	char concat[4096];

	while(!finish){
				
		fread(&active, sizeof(active), 1, fp);
	
		if(parent == 0){
			concat[0] = '/'; concat[1] = '\0';
			strcat(concat, path);
			printf("%s\n", concat);
			finish = 1;
		} else {
			fseek(fp, parent, SEEK_SET);

			fread(&active, sizeof(active), 1, fp);
                        fread(&tmp_parent, sizeof(tmp_parent), 1, fp);
                        fread(&type_file, sizeof(type_file), 1, fp);
                        fread(&folder_name, sizeof(folder_name), 1, fp);

			strcpy(concat, folder_name);
			strcat(concat, "/");
		
			strcat(concat, path);
			strcpy(path, concat);
			
			parent = tmp_parent;
		}
	}
	
	fclose(fp);

	return EXIT_SUCCESS;
}
