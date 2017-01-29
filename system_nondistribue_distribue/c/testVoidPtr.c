#include <stdio.h>
#include <stdlib.h>


void change(void * p){
	*((int *)p) = *((int *)p) + 1; // il faut rajouter un indice, pour dire que c'est du int du coup ( dans les arguments de la fonctions)
	
}

int main(int argc, char ** argv){

	int a = 0;
	change(&a);
	printf("%d\n", a);




	return EXIT_SUCCESS;
}
