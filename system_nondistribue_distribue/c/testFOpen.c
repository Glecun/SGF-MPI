#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){



	FILE *fp = fopen("testFOpen.txt", "r+");

	if(fp == NULL){
		printf("Le fichier n'as pas put être ouvert.\n");
	} else {
		printf("ficheir bien ouvert.\n");
		fclose(fp);
		
	}




	return EXIT_SUCCESS;
}
