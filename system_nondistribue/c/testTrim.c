#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void trim(char *str)
{
	int i;
	int begin = 0;
	int end = strlen(str) - 1;

	while (isspace((unsigned char) str[begin]))
		begin++;

	while ((end >= begin) && isspace((unsigned char) str[end]))
		end--;

	// Shift all characters back to the start of the string array.
	for (i = begin; i <= end; i++)
		str[i - begin] = str[i];

	str[i - begin] = '\0'; // Null terminate string.
}






int main(int argc, char ** argv){

	char * lol = malloc(256*sizeof(char));
	char * a = "   yopyopyop   ";
	strcpy(lol, a);
	printf("avant : %s\n", lol);

	trim(lol);

	printf("apres : %s\n", lol);

	return EXIT_SUCCESS;
}
