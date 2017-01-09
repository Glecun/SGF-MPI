#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){

	FILE *fp = fopen("../index.jjg","r");

	unsigned long long cursor_end;
	unsigned long long cursor_tmp;
	char version;

	fseek(fp, 0, SEEK_SET); // on met le curseur a o
	fread(&cursor_end, sizeof(cursor_end), 1, fp);
	fread(&version, sizeof(version), 1, fp);
	cursor_tmp = 9;

	char active;
	unsigned long long parent;
	char type_file;
	char file_name[255];
	unsigned long long cursor_stock;
	unsigned long long length_file;
	

	while(cursor_tmp < cursor_end){

		fread(&active , sizeof(active), 1, fp);
		if(active == 1){
			// le fichier est bien actif, donc nous allons le marquer
			fread(&parent, sizeof(parent), 1, fp);
			fread(&type_file, sizeof(type_file), 1, fp);
			fread(&file_name, sizeof(file_name), 1, fp);
			fread(&cursor_stock, sizeof(cursor_stock), 1, fp);
			fread(&length_file, sizeof(length_file), 1, fp);
			
			printf("%s\n", file_name);	
		}

		cursor_tmp += 281;
	}

	printf("cursor = %llu\n", cursor_end);

	fclose(fp);



	return EXIT_SUCCESS;
}
