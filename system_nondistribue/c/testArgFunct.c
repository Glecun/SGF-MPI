#include <stdio.h>
#include <stdlib.h>

void test(int a, int b, int c){

	if(typeof(c) != typeof(NULL)){
		printf("ok");
	} else {
		printf("ko");
	}
	
}

int main(){
	
	test(1,2,NULL);


	return EXIT_SUCCESS;
}
