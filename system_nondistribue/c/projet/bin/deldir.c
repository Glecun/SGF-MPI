#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constante.h"
#include "../get_env.h"

int main(int argc, char ** argv){

        if(argc < 2){
                printf("Usage : deldir <foldername>\n");
                return EXIT_FAILURE;
        }

	unsigned long long parent;
	get_parent(&parent);

        char file_name[255];
        strcpy(file_name, argv[1]);

        unsigned long long tmp_cursor = file_exist(DOSSIER, file_name);

        if(tmp_cursor != 0){ // le fichier existe
		supprimerDossier(file_name, tmp_cursor);
        } else {
                printf("Le dossier : %s, n'existe pas.\n", file_name);
        }

        return EXIT_SUCCESS;
}
