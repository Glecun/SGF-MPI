#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestionFichier.h"

int ajouterLigne(char newMachine[50], char newChemin[50]) {
    FILE * f = fopen("data.bd", "a"); // ouvre le fichier en mode append
	if (!f) {
		perror("ajouterLigne: fopen failed");
		exit(1);
	}
	
    f_Fichier t1;
	strcpy( t1.machine, newMachine );
	strcpy( t1.chemin, newChemin );
	
	//TODO: vérifier si il existe pas déja
	
	// écris dans le fichier avec fwrite
    if(fwrite(&t1, sizeof(f_Fichier), 1, f) != 1) 
	{
		perror("Erreur lors de l'ecriture des donnees dans le fichier!!");
		fclose(f);
		return -1;
    }
		    
	fclose(f);

	return 0;
}		 

f_Fichier* getLigne(int i) { 
	// Ouvre le fichier en mode lecture/écriture binaire
	FILE * f = fopen("data.bd", "r"); 
	if (!f) {
		perror("getLigne: fopen failed");
		exit(1);
	}
	
	// Mets le curseur du fichier au bon endroit i
	fseek(f, i*sizeof(f_Fichier), SEEK_SET); 
	
	// Lis à partir du fichier
	f_Fichier t2;
	if(fread(&t2, sizeof(f_Fichier), 1, f) != 1) 
	{
		perror("Erreur lors de la lecture des donnees dans le fichier!!");
	}
	fclose(f);
	
	// Crée la structure a renvoyer
	f_Fichier *ret = (f_Fichier*)malloc (sizeof (f_Fichier));
	strcpy( ret->chemin, t2.chemin );
	strcpy( ret->machine, t2.machine );
	
    return ret;
}

f_Fichier* getAllLignes() { 
	// Ouvre le fichier en mode lecture/écriture binaire
	FILE * f = fopen("data.bd", "r"); 
	if (!f) {
		perror("getAllLignes: fopen failed");
		exit(1);
	}
	
	int Nb_lignes=getNbLignes();
	
	// Remets le curseur du fichier au début
	fseek(f, 0, SEEK_SET); 
	
	// Lis à partir du fichier
	int i=1;
	f_Fichier t2;
	f_Fichier *ret = (f_Fichier*)malloc (Nb_lignes*sizeof(f_Fichier));
	while(1){
		if(fread(&t2, sizeof(f_Fichier), 1, f) != 1) {
			//fin du fichier
			break;
		} 
		fseek(f,i*sizeof(f_Fichier), SEEK_SET);
		
		// Crée la structure a renvoyer
		strcpy( ret[i-1].chemin, t2.chemin );
		strcpy( ret[i-1].machine, t2.machine );
		
		i++;
	}
	fclose(f);

    return ret;
}

int getNbLignes(){
	// Ouvre le fichier en mode lecture/écriture binaire
	FILE * f = fopen("data.bd", "r"); 
	if (!f) {
		perror("getNbLignes: fopen failed");
		exit(1);
	}
	// Remets le curseur du fichier au début
	fseek(f, 0, SEEK_SET); 
	
	// Lis à partir du fichier
	int i=1;
	f_Fichier t2;
	while(1){
		if(fread(&t2, sizeof(f_Fichier), 1, f) != 1) {
			//fin du fichier
			break;
		} 
		fseek(f,i*sizeof(f_Fichier), SEEK_SET);
		i++;
	}
	fclose(f);
	return i-1;
}

void supprimerContenu() {
	FILE *fc;
	fc=fopen("data.bd","w");
	fclose(fc);
}