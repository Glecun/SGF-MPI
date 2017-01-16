#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
	/**
	*	Argument : mkdir NomDeDossier
	*	### ajouter parent
	**/
	
	if(argc == 2){
	 	FILE *fp = fopen("/home/joachim/system/c/projet/index.jjg","r+"); // r+ c'est read and write, si tu met w, c'est read and write mais ça écrase le fichier
		if(fp == NULL){
			printf("Fichier non trouver, ou acces non permis.\n");
			return EXIT_FAILURE;
		}

		unsigned long long cursor;		
		
		fseek(fp, 0, SEEK_SET);
		fread(&cursor, sizeof(cursor), 1, fp);

		// printf("valeur = %llu \n", cursor);

		fseek(fp, cursor, SEEK_SET);
		
		// valeur a rajouter dans le fichier
		char active = 1; // octet active
		unsigned long long parent = 0; // parent est envoyé en argument de l'executable, pour le moment se sera 0 // ### a changer
		char type_file = 0; // type de fichier, 1 = fichier (parce que touch), 0 pour un dossier 
		char file_name[255]; // le nom du fichier // ### quand un ficheir est creer avec touch, on n'écris rien dans stockage.jjg, dés qu'il y a du contenue on remplacera cette valeur..

		strcpy(file_name, argv[1]); // on copie la chaine de charactere dans file_name pour être sur d'avoir les 255 char

		// on ajotue les valeurs
		fwrite(&active,sizeof(active), 1, fp); // on met rend active la ligne 
		fwrite(&parent,sizeof(parent), 1, fp); // on écris le parent ### pas encore fait
		fwrite(&type_file,sizeof(type_file), 1, fp); 
		fwrite(file_name,sizeof(file_name), 1, fp); 

		// on remet le cuseur de debut de fichier de index.jjg a la nouvelle valeur
		cursor += 1 + 8 + 1 + 255;
		fseek(fp, 0, SEEK_SET); // on se remet au début
		fwrite(&cursor, sizeof(cursor), 1, fp);

		fclose(fp);
	} else {
		printf("Usage : mkdir <foldername>\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
