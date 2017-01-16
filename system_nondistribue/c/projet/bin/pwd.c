#include <stdio.h>
#include <stdlib.h>
#include "../constante.h"
#include "../get_env.h"

int main(int argc, char ** argv){

	unsigned long long parent;
	get_parent(&parent);

	if(parent == 0){
		printf("/\n");
	}

	return EXIT_SUCCESS;
}
