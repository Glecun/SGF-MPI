#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constante.h"
#include "../get_env.h"

int main(int argc, char **argv){
	/**
	 *	Argument : touch NomDeFichier
	 *	### ajouter parent
	 **/

	// Usage	
	if(argc < 2){
		printf("Usage : touch <filename>\n");
		return EXIT_FAILURE;
	}

	FILE *fp = fopen("/home/joachim/system/c/projet/index.jjg","r+"); // r+ c'est read and write, si tu met w, c'est read and write mais ça écrase le fichier
	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
	}


	int i;
	for(i = 1; i < argc ; i++){
		unsigned long long cursor;		

		fseek(fp, 0, SEEK_SET);
		fread(&cursor, sizeof(cursor), 1, fp);

		// printf("valeur = %llu \n", cursor);

		fseek(fp, cursor, SEEK_SET);

		// valeur a rajouter dans le fichier
		char active = 1; // octet active
		unsigned long long parent; // parent est envoyé en argument de l'executable, pour le moment se sera 0 // ### a changer
		get_parent(&parent);
		char type_file = 1; // type de fichier, 1 = fichier (parce que touch), 0 pour un dossier 
		char file_name[255]; // le nom du fichier // ### quand un ficheir est creer avec touch, on n'écris rien dans stockage.jjg, dés qu'il y a du contenue on remplacera cette valeur..
		unsigned long long cursor_stock = 0; // adresse a laquelle se trouve le fichier dans stockage.jjg // ### a changer, aller chercher la valeur dans le fichier stockage.jjg
		unsigned long long length_file = 0; // taille du fichier, 0 puisque juste créer, par de contenue 

		strcpy(file_name, argv[i]); // on copie la chaine de charactere dans file_name pour être sur d'avoir les 255 char

		// on verifie que la personne n'essaye pas de rajouter quelque chose qui existe déjà
		if(file_exist(FICHIER, file_name) != 0){
			printf("Le fichier : \"%s\" existe déjà.\n", file_name);
		} else {	

			// on ajoute les valeurs
			fwrite(&active,sizeof(active), 1, fp); // on met rend active la ligne 
			fwrite(&parent,sizeof(parent), 1, fp); // on écris le parent ### pas encore fait
			fwrite(&type_file,sizeof(type_file), 1, fp); 
			fwrite(file_name,sizeof(file_name), 1, fp); 
			fwrite(&cursor_stock,sizeof(cursor_stock), 1, fp); 
			fwrite(&length_file,sizeof(length_file), 1, fp); 

			// on remet le cuseur de debut de fichier de index.jjg a la nouvelle valeur
			cursor += 1 + 8 + 1 + 255 + 8 + 8;
			fseek(fp, 0, SEEK_SET); // on se remet au début
			fwrite(&cursor, sizeof(cursor), 1, fp);
		}
	}
	fclose(fp);

	return EXIT_SUCCESS;
}
