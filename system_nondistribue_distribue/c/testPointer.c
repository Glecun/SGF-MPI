#include <stdio.h>
#include <stdlib.h>

void incre(int * a){
	printf("%u",a);
	(*a)++;
}

int main(int argc, char ** argv){


	int a = 0;
	printf("debut : %d\n", a);	
	incre(&a);
	printf("fin : %d\n", a);	

	return EXIT_SUCCESS;
}
