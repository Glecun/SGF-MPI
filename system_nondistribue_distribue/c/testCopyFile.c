#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){

	FILE *fp = fopen("testCopyFile.src", "r");

	fseek(fp, 0L, SEEK_END);
	unsigned long long sz;
	sz =  ftell(fp);	

	printf("cursor : %llu\n", sz);

	fclose(fp);
	return EXIT_SUCCESS;
}
