#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestionFichier.h"

int ajouterLigne(char newMachine[50], char newChemin[50]) {
    FILE * f = fopen("data.bd", "r+"); // ouvre le fichier en mode append
	if (!f) {
		perror("ajouterLigne: fopen failed");
		exit(1);
	}
	
    f_Fichier t1; // a ajouter
	strcpy( t1.machine, newMachine );
	strcpy( t1.chemin, newChemin );
	
	f_Fichier t2; //contenu du fichier
	int i=1;
	if (!estDansFichier(t1)){
		
		// On essaye d'ajouter dans une ligne vide
		int alreadyAdded=0;
		while(1){
			if(fread(&t2, sizeof(f_Fichier), 1, f) != 1) {
				//fin du fichier
				break;
			} 
			
			// Si la ligne est vide
			if (!strcmp(t2.machine,"")){ 
				// On ajoute ici
				fseek(f, -sizeof(f_Fichier), SEEK_CUR); // Un pas en arri�re
				if( fwrite( &t1, sizeof(f_Fichier), 1, f) != 1) {
					perror("Erreur lors de l'ecriture des donnees dans le fichier!!");
					fclose(f);
					return 0;
				}
				alreadyAdded=1;
				break;
			} else {
				fseek(f,i*sizeof(f_Fichier), SEEK_SET); //on avance que si c'est aps la bonne ligne
			}
			
			i++;
		}
				
		// Si y'avait pas de place dans le fichier on ecrit � la fin
		if(alreadyAdded!=1) {
			if(fwrite(&t1, sizeof(f_Fichier), 1, f) != 1) {
				perror("Erreur lors de l'ecriture des donnees dans le fichier!!");
				fclose(f);
				return 0;
			}
		}
	} else {
		return 0;
	}
		    
	fclose(f);

	return 1;
}		 

f_Fichier* getLigne(int i) { 
	// Ouvre le fichier en mode lecture binaire
	FILE * f = fopen("data.bd", "r"); 
	if (!f) {
		perror("getLigne: fopen failed");
		exit(1);
	}
	
	// Mets le curseur du fichier au bon endroit i
	fseek(f, i*sizeof(f_Fichier), SEEK_SET); 
	
	// Lis � partir du fichier
	f_Fichier t2;
	if(fread(&t2, sizeof(f_Fichier), 1, f) != 1) 
	{
		perror("Erreur lors de la lecture des donnees dans le fichier!!");
	}
	fclose(f);
	
	// Cr�e la structure a renvoyer
	f_Fichier *ret = (f_Fichier*)malloc (sizeof (f_Fichier));
	strcpy( ret->chemin, t2.chemin );
	strcpy( ret->machine, t2.machine );
	
    return ret;
}

f_Fichier* getAllLignes() { 
	// Ouvre le fichier en mode lecture binaire
	FILE * f = fopen("data.bd", "r"); 
	if (!f) {
		perror("getAllLignes: fopen failed");
		exit(1);
	}
	
	int Nb_lignes=getNbLignes();
	
	// Remets le curseur du fichier au d�but
	fseek(f, 0, SEEK_SET); 
	
	// Lis � partir du fichier
	int i=1;
	int iret=0;
	f_Fichier *t2= (f_Fichier*)malloc (sizeof(f_Fichier));;
	f_Fichier *ret = (f_Fichier*)malloc (Nb_lignes*sizeof(f_Fichier));
	while(1){
		if(fread(t2, sizeof(f_Fichier), 1, f) != 1) {
			//fin du fichier
			break;
		} 
		fseek(f,i*sizeof(f_Fichier), SEEK_SET);
		
		// Cr�e la structure a renvoyer
		if (strcmp(t2->machine,"")){
			strcpy( ret[iret].chemin, t2->chemin );
			strcpy( ret[iret].machine, t2->machine );
			iret++;
		}
		i++;
	}
	fclose(f);

    return ret;
}

int getNbLignes(){
	// Ouvre le fichier en mode lecture binaire
	FILE * f = fopen("data.bd", "r"); 
	if (!f) {
		perror("getNbLignes: fopen failed");
		exit(1);
	}
	// Remets le curseur du fichier au d�but
	fseek(f, 0, SEEK_SET); 
	
	// Lis � partir du fichier
	int i=1;
	int ret=0;
	f_Fichier t2;
	while(1){
		if(fread(&t2, sizeof(f_Fichier), 1, f) != 1) {
			//fin du fichier
			break;
		} 
		fseek(f,i*sizeof(f_Fichier), SEEK_SET);
		
		if (strcmp(t2.machine,"")){
			ret++;
		}
		i++;
	}
	fclose(f);
	return ret;
}

void supprimerContenu() {
	FILE *fc;
	fc=fopen("data.bd","w");
	fclose(fc);
}

int supprimerLigne(char chemin[50]) {
	// Ouvre le fichier en mode lecture/�criture binaire
	FILE * f = fopen("data.bd", "r+"); 
	if (!f) {
		perror("supprimerLigne: fopen failed");
		exit(1);
	}

	// Remets le curseur du fichier au d�but
	fseek(f, 0, SEEK_SET); 
	
	// Lis � partir du fichier
	int i=1;
	f_Fichier t2;//= (f_Fichier*)malloc (sizeof(f_Fichier));;
	int ret = 0;
	while(1){
		if(fread(&t2, sizeof(f_Fichier), 1, f) != 1) {
			//fin du fichier
			break;
		} 
		
		// Supprime la ligne
		if (!strcmp(t2.chemin,chemin)){ 
			fseek(f, -sizeof(f_Fichier), SEEK_CUR); // Un pas en arri�re
			f_Fichier t3;
			strcpy( t3.machine, "" );
			strcpy( t3.chemin, "" );
			if(	fwrite( &t3 , sizeof(f_Fichier) , 1 , f)!= 1) {
				perror("Erreur lors de l'ecriture des donnees dans le fichier!!");
				fclose(f);
				return 0;
			}
			
			ret=1;
			break;
		} else {
			fseek(f,i*sizeof(f_Fichier), SEEK_SET); //on avance que si c'est aps la bonne ligne
		}
		
		i++;
	}
	fclose(f);

    return ret;
}

int estDansFichier (f_Fichier fich) {
	f_Fichier *all = getAllLignes();
	for (int i = 0 ; i < getNbLignes(); i++){
		//printf("fich.machine %s all[i].machine %s ;; fich.chemin %s all[i].chemin %s \n",fich.machine, all[i].machine,  fich.chemin , all[i].chemin);
		if(strcmp(fich.chemin, all[i].chemin)==0){
			return 1;
		}
	}
	return 0;
}