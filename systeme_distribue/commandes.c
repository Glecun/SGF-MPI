#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include "commandes.h"
#include "gestionFichier.h"
#include "utils.h"

// Commande Touch
char* cmd_touch(char* cmd, char nameMach[MPI_MAX_PROCESSOR_NAME]){
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
}

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
