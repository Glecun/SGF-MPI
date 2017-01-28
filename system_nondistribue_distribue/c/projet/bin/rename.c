#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constante.h"
#include "../get_env.h"

int main(int argc, char **argv){
	/**
	 *	Argument : rename f|d NomSource Nomdestination
	 *	### ajouter parent
	 **/

	// Usage	
	if(argc < 4){
		printf("Usage : rename <d|f> <name_source> <name_dest>\n");
		return EXIT_FAILURE;
	}

	FILE *fp = fopen("/home/joachim/system/c/projet/index.jjg","r+"); // r+ c'est read and write, si tu met w, c'est read and write mais ça écrase le fichier
	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
	}

	// valeur a rajouter dans le fichier
	unsigned long long parent; // parent est envoyé en argument de l'executable, pour le moment se sera 0 // ### a changer
	get_parent(&parent);
	char type_file = 1; // type de fichier, 1 = fichier (parce que touch), 0 pour un dossier 
	char file_name_src[255]; // le nom du fichier // ### quand un ficheir est creer avec touch, on n'écris rien dans stockage.jjg, dés qu'il y a du contenue on remplacera cette valeur..
	char file_name_dest[255];

	strcpy(file_name_src, argv[2]); // on copie la chaine de charactere dans file_name pour être sur d'avoir les 255 char
	strcpy(file_name_dest, argv[3]); 

	if(argv[1][0] == 'd'){
		type_file = DOSSIER;
	} else if(argv[1][0] == 'f'){
		type_file = FICHIER;
	} else {
		printf("Sois c'est f, sois c'est d..\n");
		return EXIT_FAILURE;
	}

	// on verifie que la personne n'essaye pas de rajouter quelque chose qui existe déjà
	parent = file_exist(type_file, file_name_src);
	if(parent != 0){ // le fichier existe
		if(file_exist(type_file, file_name_dest) != 0){
			printf("Le fichier %s existe déjà\n", file_name_dest);	
		} else {
		fseek(fp, parent + 1 + 8 + 1, SEEK_SET);	
		fwrite(file_name_dest,sizeof(file_name_dest), 1, fp); 
		}
	} else {	
		printf("le fichier %s n'existe pas.", file_name_src);
	}

	fclose(fp);

	return EXIT_SUCCESS;
}
