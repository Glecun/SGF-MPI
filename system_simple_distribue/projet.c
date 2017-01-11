#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>
#include "gestionFichier.h"

// Pour executer des regexp
int execRegex (char* str_regex,char* str_request);
// Commande Touch
void cmd_touch(char cmd[255],int num ,char nameMach[MPI_MAX_PROCESSOR_NAME]);

int main (int argc, char ** argv){
	//Variables
	int num;		// numéro du thread
	char cmd[255];	// variable qui contient la commande
	int tag = 0;
	int size;			//nombre de threads
	int lenNameMach; //l ongueur du nim de la machine
	char nameMach[MPI_MAX_PROCESSOR_NAME]; // nom de la machine
	MPI_Request request;
	MPI_Status status;
	
	//Init MPI
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &num);	// init le numéro du thread
	MPI_Comm_size( MPI_COMM_WORLD, &size);		// nombre de threads
	 MPI_Get_processor_name(nameMach, &lenNameMach);
	
	// Client unique qui envoi des commandes
	if (0==num){
		//supprimer le contenu du fichier commun dès le début
		supprimerContenu();

		strcpy(cmd,"touch coucou");
		//Ex 1 - le client veut creer des fichiers 
		for (int i =1 ;i<size;i++){ 
			printf ("Client: Serveur %d je t'envoi %s \n", i,cmd);
			MPI_Isend(&cmd,255,MPI_CHAR,i,tag,MPI_COMM_WORLD,&request); 
			// Pour le moment il envoie une fois a chaque thread, TODO: envoyer plutot la commande entière et l'analyser dans le serveur
		}
		
	// Serveurs	
	} else {	
		// TODO: faire fonctionner les serveurs en round-robin (tourniquet) les serveurs acèderont aux pocesseurs un par un pendant un temp (ou nb d'instructions ) limité
	
		// Réception de la commande
		MPI_Irecv(&cmd,255,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
		MPI_Wait (&request, &status);
		printf ("Serveur %d: j'ai reçu %s \n", num, cmd);
		
		// Analyse et traitement des commandes
		cmd_touch(cmd,num,nameMach);

	}
	
	MPI_Finalize ();
	
	return 0;
}

// executer une regexp
int execRegex (char* str_regex,char* str_request){
	int err, match;
	regex_t preg;
	err = regcomp (&preg, str_regex, REG_NOSUB | REG_EXTENDED);
	if (err != 0){ perror("compilaion regexp failed"); }
	match = regexec (&preg, str_request, 0, NULL, 0);
	regfree (&preg);
	return match;
}

// Commande Touch
void cmd_touch(char cmd[255],int num, char nameMach[MPI_MAX_PROCESSOR_NAME]){
		int match;
		
		// vérifie avec regex si la commande est touch
		char *str_regex = "[ ]*touch[ ]*[-_[:alnum:]]+";
		match=execRegex(str_regex,cmd);
		if (match == 0) { 		
			printf ("Serveur %d: je crée un fichier\n", num);
			
			//Récupère le nom du fichier dans la commande
			char *split, *nameFich;
			split=strtok(cmd," ");
			nameFich=split;
			do{
				split=strtok(NULL," ");
				nameFich=split;
			} while(split != NULL && (strcmp(split,"touch")==0 || strcmp(split,"")==0) );
			
			//Mise à jour du fichier commun			
			char chemin[50];
			strcpy(chemin,nameFich);
			ajouterLigne(nameMach,chemin);
			
			//TODO: brancher le truc de joachim pour creation du fichier réel
		}
}
