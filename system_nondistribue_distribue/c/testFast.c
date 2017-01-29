#include <stdio.h>
#include <stdlib.h>

int main(){

	int fin_terminal = 0;	
	int compteur = 0;

	while(!fin_terminal && compteur < 200000){
		compteur++;
		printf("%d\n", compteur);
	
	}

	return EXIT_SUCCESS;

}

