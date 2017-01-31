#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include "commandes.h"
#include "gestionFichier.h"
#include "utils.h"
#include "constante.h"
#include "get_env.h"

void cmd_touch(char ** s_c){

	int argc = 0;
	while(s_c[argc] != NULL){
		//printf("%s\n", s_c[argc]);
		argc++;
	}

	if(argc < 2){
	        printf("Usage : touch <filename>\n");
                return;
        }
                                                                                                                                                                                                  
        FILE *fp = fopen(FILE_INDEX,"r+"); // r+ c'est read and write, si tu met w, c'est read and write mais ça écrase le fichier
        if(fp == NULL){
                printf("Fichier non trouver, ou acces non permis.\n");
        	return;
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
                                                                                                                                                                                                  
                strcpy(file_name, s_c[i]); // on copie la chaine de charactere dans file_name pour être sur d'avoir les 255 char
                                                                                                                                                                                                  
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
                        cursor += INDEX_LINE_SIZE;
                        fseek(fp, 0, SEEK_SET); // on se remet au début
                        fwrite(&cursor, sizeof(cursor), 1, fp);
                }
        }
        fclose(fp);

}

void cmd_ls(char ** s_c){

	int argc = 0;
       	while(s_c[argc] != NULL){
       		//printf("%s\n", s_c[argc]);
       		argc++;
       	}

	FILE *fp = fopen(FILE_INDEX, "r");
	if(fp == NULL){
		printf("Fichier non trouver, ou acces non permis.\n");
		return;
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
	char char_type_file;
	unsigned long long file_cursor_stock;
	unsigned long long file_size;

	while(cursor_tmp < cursor_end){
		fread(&active, sizeof(active), 1, fp);
		fread(&tmp_parent, sizeof(tmp_parent), 1, fp);
		fread(&type_file, sizeof(type_file), 1, fp);
		if(active && tmp_parent == parent){ // on verifie qu'il est actif, et qu'il est bien dans le repertoire courant
			fread(file_name, sizeof(file_name), 1, fp);
			// on affiche les infos 
			if(type_file == FICHIER){ // fichier
				//fseek(fp, sizeof(unsigned long long), SEEK_CUR); // on saute l'endroit ou est stocké le fichier
				fread(&file_cursor_stock, sizeof(file_cursor_stock), 1, fp);
				fread(&file_size, sizeof(file_size), 1, fp);
				char_type_file = 'f';	
				
			} else {
				char_type_file = 'd';
				file_size = 1; // on met file_size a 1...
			}
			
			// ### peut être plus de différence sur file_size.. un if en plus..
			printf("%c\t%s\t%llu\t%llu\n", char_type_file, file_name, file_cursor_stock,file_size); 
		}

		cursor_tmp += INDEX_LINE_SIZE;
		fseek(fp, cursor_tmp, SEEK_SET); // on place le curseur au bon endroit pour la suite ( parce qu'on ne lis pas tout.. surtout si c'est un fichier..

	}

	fclose(fp);

}

// Commande Touch
/*char* cmd_touch(char* cmd, char nameMach[MPI_MAX_PROCESSOR_NAME]){
	int match;
	char* ret = (char *) malloc(sizeof(char)*1024); // on sait que ca ne dépassera pas 1024
	strcpy(ret,"");
	
	// Vérifie si ya des fichiers
	char *str_regex_hasFich = "^[ ]*touch[ ]*$";
	match=execRegex(str_regex_hasFich,cmd);
	if (match == 0) {	
		strcpy(ret,"vous devez renseigner un fichier"); 
	}
	
	// Vérifie avec regex si la commande est touch
	char *str_regex = "^[ ]*touch[ ]*[ -_[:alnum:]]+$";
	match=execRegex(str_regex,cmd);
	if (match == 0) {
	
		// Récupère le nom du fichier dans la commande
		char *split, *nameFich;
		split=strtok(cmd," "); 
		while(1){
			do{
				split=strtok(NULL," ");
				if (split==NULL) break; // si ya plus de fichiers on quitte
				nameFich=split;
			} while((strcmp(split,"touch")==0 || strcmp(split,"")==0) );
			if (split==NULL) break; // si ya plus de fichiers on quitte
			
			// Mise à jour du fichier commun			
			char chemin[50];
			strcpy(chemin,nameFich);
			if (ajouterLigne(nameMach,chemin)){
				// info retour
				if (!strcmp(ret,""))
					strcpy(ret,"fichier crée\0"); 	
				else
					strcat(ret,"\nfichier crée\0");
			}
			else{
				// info retour
				if (!strcmp(ret,""))
					strcpy(ret,"fichier déja existant\0");
				else
					strcat(ret,"\nfichier déja existant\0");
			}
			
			// TODO: Renseigner le code pour creation du fichier réel
			
		}

	}
	return ret;
} */

// Commande Rm
char* cmd_rm(char* cmd){
	int match;
	char* ret = (char *) malloc(sizeof(char)*1024); // on sait que ca ne dépassera pas 1024
	strcpy(ret,"");
	
	// Vérifie si ya des fichiers
	char *str_regex_hasFich = "^[ ]*rm[ ]*$";
	match=execRegex(str_regex_hasFich,cmd);
	if (match == 0) {	
		strcpy(ret,"vous devez renseigner un fichier"); 
	}
	
	// Vérifie avec regex si la commande est touch
	char *str_regex = "[ ]*rm[ ]*[-_[:alnum:]]+";
	match=execRegex(str_regex,cmd);
	if (match == 0) {			
		// Récupère le nom du fichier dans la commande
		char *split, *nameFich;
		split=strtok(cmd," ");	// TODO: faire pour de multiple fichier
		while(1){
			do{
				split=strtok(NULL," ");
				if (split==NULL) break; // si ya plus de fichiers on quitte
				nameFich=split;
			} while((strcmp(split,"touch")==0 || strcmp(split,"")==0) );
			if (split==NULL) break; // si ya plus de fichiers on quitte
			
			// Mise à jour du fichier commun			
			char chemin[50];
			strcpy(chemin,nameFich);
			if (supprimerLigne(chemin)){
				// info retour
				if (!strcmp(ret,""))
					strcpy(ret,"fichier supprimé\0");
				else
					strcat(ret,"\nfichier supprimé\0");
			}
			else{
				// info retour
				if (!strcmp(ret,""))
					strcpy(ret,"fichier inexistant\0");
				else				
					strcat(ret,"\nfichier inexistant\0");
			}
				
			// TODO: Renseigner le code pour suppression du fichier réel
			
		}
	}
	
	return ret;
}

// Commande Show data
char* cmd_showdata(char* cmd){
	int match;
	char* ret = (char *) malloc(sizeof(char)*1024); //1024 initialement
	strcpy(ret,"");
	
	// Vérifie avec regex si la commande est show data
	char *str_regex = "[ ]*show data[ ]*";
	match=execRegex(str_regex,cmd);
	if (match == 0) {
		// On recupere toutes les lignes du fichier
		f_Fichier *dataFich = getAllLignes();
		// On met les entetes
		strcpy(ret,"Num\tMachine\tChemin \n");
		// Variable temporaire
		char* retTmp=(char *) malloc(sizeof(char)*1024);
		for (int i = 0 ; i < getNbLignes(); i++){	
			// On realloue dynamiquement la variable temporaire (au cas ou la ligne dépasse 1024)
			int lenghtRetTmp = snprintf( NULL, 0, "%d\t%s\t%s \n",i, dataFich[i].machine, dataFich[i].chemin );
			retTmp=realloc(retTmp , lenghtRetTmp + 1 );
			sprintf( retTmp, "%d\t%s\t%s \n",i, dataFich[i].machine, dataFich[i].chemin);
			
			// On concatene en reallouant dynamiquement la variable temporaire(ligne) à la variable de retour
			int lenghtRet = snprintf( NULL, 0, " %s %s \n",ret, retTmp );
			ret=realloc(ret , lenghtRet + 1 );
			strcat(ret,retTmp);
		}
	}
	return ret;
}
