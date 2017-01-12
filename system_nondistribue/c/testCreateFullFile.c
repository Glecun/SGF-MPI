#include <stdio.h>
#include <stdlib.h>
int main(void) {
        int X = 512 * 1024 * 1024 - 1;
        FILE *fp = fopen("testCreateFullFile.txt", "w+");
        fseek(fp, X , SEEK_SET);
        fputc('\0', fp);
        fclose(fp);
	
	return EXIT_SUCCESS;
}
