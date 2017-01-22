#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void set_parent( unsigned long long parent){
	FILE *fp = fopen(FILE_INDEX, "r+");
                                                                      	
	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
	}

	fseek(fp, 9, SEEK_SET); // on passe le cursor et la version

	fwrite(&parent, sizeof(parent), 1, fp);

	fclose(fp);
};

unsigned long long file_exist(char arg_type_file, char * arg_file_name){
	/*
		Préviens si un fichier|dossier existe de nom file_name 
	*/

	FILE *fp = fopen(FILE_INDEX, "r");
        if(fp == NULL){
        	printf("Fichier non trouver, ou acces non permis.\n");
        }

        unsigned long long cursor_end;
        unsigned long long cursor_tmp = FILE_BASE_ENV;
        unsigned long long parent;

        fseek(fp, 0,  SEEK_SET);
        fread(&cursor_end, sizeof(cursor_end), 1, fp);
        get_parent(&parent);
        fseek(fp, cursor_tmp,  SEEK_SET);
        
        char active;
        unsigned long long tmp_parent;
        char type_file;
        char file_name[255];
        
        while(cursor_tmp < cursor_end){
        	fread(&active, sizeof(active), 1, fp);
        	fread(&tmp_parent, sizeof(tmp_parent), 1, fp);
        	fread(&type_file, sizeof(type_file), 1, fp);
        	if(active && tmp_parent == parent){ // on verifie que c'est un fichier, qu'il est actif, et qu'il est bien dans le repertoire courant
        		// on affiche les infos 
        		if(type_file == arg_type_file){ // fichier
        			fread(file_name, sizeof(file_name), 1, fp);
				if(strcmp(file_name, arg_file_name) == 0){
					fclose(fp);
					return cursor_tmp;
				}
        		} 
        	}
        	if(type_file){
        		cursor_tmp += 1+8+1+255+8+8;
        	} else {
        		cursor_tmp += 1+8+1+255;
        	}
        	fseek(fp, cursor_tmp, SEEK_SET); // on place le curseur au bon endroit pour la suite ( parce qu'on ne lis pas tout.. surtout si c'est un fichier..
        }

        fclose(fp);
	return 0;
}
