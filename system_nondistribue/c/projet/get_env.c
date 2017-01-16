#include <stdio.h>
#include <stdlib.h>
#include "constante.h"
#include "get_env.h"

void get_version( char * v ){
	
	FILE *fp = fopen(FILE_INDEX, "r");

	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
	}

	fseek(fp, 8, SEEK_SET); // on passe le cursor
	
	char version;

	fread(&version, sizeof(version), 1, fp);

	(*v) = version;
	fclose(fp);
};

void get_parent( unsigned long long * p){
	FILE *fp = fopen(FILE_INDEX, "r");
                                                                      	
	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
	}

	fseek(fp, 9, SEEK_SET); // on passe le cursor et la version
	
	unsigned long long parent;

	fread(&parent, sizeof(parent), 1, fp);

	(*p) = parent;
	fclose(fp);
};
