#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constante.h"
#include "get_env.h"

void get_index_last_cursor( unsigned long long * c ){
	FILE *fp = fopen(FILE_INDEX, "r");

	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
	}

	fseek(fp, 0, SEEK_SET); // on passe le cursor

	unsigned long long cursor;

	fread(&cursor, sizeof(cursor), 1, fp);

	(*c) = cursor;
	fclose(fp);
};

void get_stock_last_cursor( unsigned long long * c ){
	FILE *fp = fopen(FILE_STOCK, "r");

	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
	}

	fseek(fp, 0, SEEK_SET); // on passe le cursor

	unsigned long long cursor;

	fread(&cursor, sizeof(cursor), 1, fp);

	(*c) = cursor;
	fclose(fp);
};

void set_stock_last_cursor( unsigned long long cursor ){
	FILE *fp = fopen(FILE_STOCK, "r+");

	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
	}

	fseek(fp, 0, SEEK_SET); // on passe le cursor

	fwrite(&cursor, sizeof(cursor), 1, fp);

	fclose(fp);
};

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
		retourne la valeur du curseur de debut si le fichier exist, sinon renvoie 0 
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

void put_file(char * path_filename, unsigned long long cursor_stock, unsigned long long * file_size){
	/*
	   fonction, prennant un fichier sur le systeme hôte et le met dans le filesystem.
	   écris dans stockage.jjg a partir du cursor_stock
	 */

	// on ouvre le 1er fichier en lecture
	FILE *fp_src = fopen(path_filename, "r");		
	unsigned long long size_src;
	fseek(fp_src, 0, SEEK_END);
	size_src = ftell(fp_src);
	(*file_size) = size_src; // on transmet la taille du fichier

	// on ouvre le fichier stockage en écriture
	FILE *fp_dest = fopen(FILE_STOCK, "r+");

	// on prend le contenue du fichier et on le stock dans le fichier stockage
	// on regarde combien de bit il faut copier
	char tmp[1024];
	unsigned long long tmp_size_src = size_src; // compteur 
	int tmp_size; // retiens combien de char on copieras
	fseek(fp_dest, cursor_stock, SEEK_SET);
	fseek(fp_src, 0, SEEK_SET);
	while(tmp_size_src > 0){
		if(tmp_size_src > 1024){
			tmp_size = 1024;
			tmp_size_src -= 1024; 
		} else {
			tmp_size = (int) tmp_size_src;
			tmp_size_src =0;
		}

		fread(tmp, sizeof(char) * tmp_size, 1, fp_src);
		printf("tmp : %d ,contenue : %s\n", tmp_size ,tmp);
		fwrite(tmp, sizeof(char) * tmp_size, 1, fp_dest);
	}

	// on met a jour le curseur
	set_stock_last_cursor( cursor_stock + size_src);

	fclose(fp_src);
	fclose(fp_dest);
}

void ajouterLigne(unsigned long long cursor, char active, unsigned long long parent, char file_type, char * file_name, unsigned long long file_cursor_stock, unsigned long long file_size){
	FILE *fp = fopen(FILE_INDEX, "r+");

	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
	}

	fseek(fp, cursor, SEEK_SET); // on passe le cursor et la version

	fwrite(&active, sizeof(active), 1, fp);
	fwrite(&parent, sizeof(parent), 1, fp);
	fwrite(&file_type, sizeof(file_type), 1, fp);
	fwrite(file_name, sizeof(file_name), 1, fp);
	if(file_type == FICHIER){
		fwrite(&file_cursor_stock, sizeof(file_cursor_stock), 1, fp);
		fwrite(&file_size, sizeof(file_size), 1, fp);
	}

	fclose(fp);
};
