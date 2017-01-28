#include <stdio.h>
#include <stdlib.h>


void mywrite(unsigned long long s){
	
	
	FILE *fp = fopen("testFWriteFunc.txt", "w+");

	fwrite(&s, sizeof(s), 1, fp);

	fclose(fp);

}

int main(int argc, char ** argv){

	mywrite(65);


	return EXIT_SUCCESS;
}
