#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	La suite serait de mettre le tableau de tableau dans les arguments
*/

void  parsing( char * s, int t , char r[256][256], int * c){
	//char r[256][256];

	int i;
        int isQuoted = -1;
	int last_cursor = 0;
	int compteur = 0;

	printf("%s\n", s);
        
        for(i=0; i < strlen(s); i++){
        	if(s[i] == '"'){
        		if(i-1 >= 0){
        			if(s[i-1] == '\\'){
        				printf("\"");
        			} else {
        				isQuoted *= -1;		
        			}	
        		} else {
        			isQuoted *= -1;
        		}
        	} else {
			if(s[i] == ' '){
        			if(isQuoted == 1){
        				printf("%c", s[i]);
        			} else {
					memcpy(r[compteur], &s[last_cursor], i - last_cursor);
					printf(" --- %d -> %d", last_cursor, i - 1);		
					last_cursor = i + 1;
        				printf("\n");
					compteur++;
        			}
        		} else {
        			printf("%c", s[i]);
        		}
        	} 
        }
	memcpy(r[compteur], &s[last_cursor], i - last_cursor);
                                                                 
	printf(" --- %d -> %d", last_cursor, i);
        printf("\n");
	compteur++;

	*c = compteur;

	/*
	for(i=0;i<compteur;i++){
		printf("%s\n", r[i]);
	}
	*/

//	return r;

}

int main(int argc, char ** argv){

	char * s = "touch bonjour \"holo a\" \"\\\"lol\" \"a\"a une autre chaine";
	char r[256][256];
	int i;
	int compteur;	


	parsing(s, sizeof(s), r, &compteur);

	for(i=0;i<compteur;i++){
        	printf("%s\n", r[i]);
        }

	
	

	/*
	int i;
	int isQuoted = -1;
	
	for(i=0; i < strlen(s); i++){
		if(s[i] == '"'){
			if(i-1 >= 0){
				if(s[i-1] == '\\'){
					printf("\"");
				} else {
					isQuoted *= -1;		
				}	
			} else {
				isQuoted *= -1;
			}
		} else {
			if(s[i] == ' '){
				if(isQuoted == 1){
					printf("%c", s[i]);
				} else {
					printf("\n");
				}
			} else {
				printf("%c", s[i]);
			}
		} 
	}

	printf("\n");
	*/

	return EXIT_SUCCESS;
}
