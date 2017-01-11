#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>
#include "gestionFichier.h"
#include "utils.h"
#include "commandes.h"

int main (int argc, char ** argv){
	//Variables
	int num;								// numéro du thread						
	char * commande; 						// variable qui contient la commande
	int tag = 0;
	char ret[1024];							// info de retour des serveurs
	int size;								// nombre de threads
	int length;								// taille de la commande
	int lenNameMach; 						// longueur du nim de la machine
	char nameMach[MPI_MAX_PROCESSOR_NAME]; 	// nom de la machine
	MPI_Request request;
	MPI_Status status[3];
	
	//Init MPI
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &num);				// init le numéro du thread
	MPI_Comm_size( MPI_COMM_WORLD, &size);				// nombre de threads
	MPI_Get_processor_name(nameMach, &lenNameMach);
	
	// Client unique qui envoi des commandes
	if (0==num){
		// Supprimer le contenu du fichier commun dès le début
		supprimerContenu();
		
		// Simulation terminal
		while(1){
			// On demande la commande
			printf("user@machine$> ");
			commande = inputString(stdin, 10);
			
			// Sortie
			if(!strcmp(commande, "exit")){ 
				return 1;
			}
			
			int threadDest=1; // TODO: envoyer au bon thread
			
			//On envoi la longueur de la commande
			length = strlen(commande);
			MPI_Send (&length, 1, MPI_INT, threadDest, tag, MPI_COMM_WORLD);
			
			// On envoi la commande au thread 
			MPI_Isend(commande,length+1,MPI_CHAR,threadDest,tag,MPI_COMM_WORLD,&request);
			
			// Recupération & Affichage de la reponse Serveur (erreur ou info)
			MPI_Recv (&ret, 1024, MPI_CHAR, threadDest, tag, MPI_COMM_WORLD, &status[3]);
			printf("%s\n",ret);
		}
		free(commande);
		
	// Serveurs	
	} else {	
		// TODO: faire fonctionner les serveurs en round-robin (tourniquet) les serveurs acèderont aux pocesseurs un par un pendant un temp (ou nb d'instructions ) limité
		
		// On recoit la longueur de la commande
		MPI_Recv (&length, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &status[0]);
		
		// Réception de la commande
		commande = (char *) malloc(length+1); // on set la taille de la commande
		MPI_Irecv(commande,length+1,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
		MPI_Wait (&request, &status[1]);
		
		// Analyse et traitement des commande
		char * retour = cmd_touch(commande,num,nameMach);
		
		//Envoi du retour (erreur ou info)
		MPI_Send (retour, 1024, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

	}
	
	MPI_Finalize ();
	
	return 0;
}