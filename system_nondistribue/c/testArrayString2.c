#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void fo(char ** r){
	char * lol = "lol";
	char * string = " bonjour ";
	char ** buffer = malloc(5 * sizeof(char*));

	r[0] = lol;

	lol = "lal";
	r[1] = lol;
	
	buffer[0] = malloc(5 * sizeof(char));
	memcpy(buffer[0], &string[1], 4);
	buffer[0][4] = '\0';
	r[2] = buffer[0];

	buffer[1] = malloc(5 * sizeof(char));
	memcpy(buffer[1], &string[2], 4);
        buffer[1][4]= '\0';
	r[3] = buffer[1];

	r[4] = NULL;
	

}

int main(int argc, char ** argv){


	char ** r = malloc( 256 * 256 * sizeof(char));

	fo(r);


	int i=0;
	while(r[i] != NULL && i < 250){
		printf("%s\n", r[i]);

		i++;
	}

	return EXIT_SUCCESS;
}
