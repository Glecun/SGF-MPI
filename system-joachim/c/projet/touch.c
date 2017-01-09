#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
	/**
	*	Argument : touch NumeroDeVersion NomDeFichier
	**/
	
	if(argc == 3){
	 	FILE *fp = fopen("index.jjg","r"); // on ouvre le fichier
		/*
		unsigned long long aze;		

		fseek(fp, 0, SEEK_SET); // on place le curseur en début de fichier
		fread(&aze, sizeof(aze), 1, fp); // on lit le fichier

		printf("valeur = %llu \n", aze);
		*/

		fseek(fp, 0, SEEK_SET);
        	unsigned long long aze;
        	fread(&aze, sizeof(aze), 1, fp);
        	printf("ici :: %llu , %lu\n", aze, sizeof(aze));
						
		fclose(fp); // on ferme le fichier
	} else {
		printf("Usage : touch <filename>\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
