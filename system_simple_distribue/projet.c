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
	char *ret;								// info de retour des serveurs
	int size;								// nombre de threads
	int length;								// taille de la commande
	int lengthRet;							// taille du retour
	int lenNameMach; 						// longueur du nim de la machine
	char nameMach[MPI_MAX_PROCESSOR_NAME]; 	// nom de la machine
	MPI_Request request;
	MPI_Status status[4];
	
	//Init MPI
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &num);				// init le numéro du thread
	MPI_Comm_size( MPI_COMM_WORLD, &size);				// nombre de threads
	MPI_Get_processor_name(nameMach, &lenNameMach);
	
	// Client unique qui envoi des commandes
	if (0==num){
		// Supprimer le contenu du fichier commun dès le début
		supprimerContenu();
		
		// Premier thread destinataire
		int threadDest=1;
		
		// Simulation terminal
		while(1){
			// On demande la commande
			printf("user@machine$>");
			commande = inputString(stdin, 10);
			
			// Sortie du programme (exit)
			char *str_regex = "^[ ]*exit[ ]*$";
			if(!execRegex(str_regex,commande)){ 
				MPI_Abort(MPI_COMM_WORLD,0);
			}
			
			//On envoi la longueur de la commande
			length = strlen(commande);
			MPI_Send (&length, 1, MPI_INT, threadDest, tag, MPI_COMM_WORLD);
			
			// On envoi la commande au thread 
			MPI_Isend(commande,length+1,MPI_CHAR,threadDest,tag,MPI_COMM_WORLD,&request);
			
			// On recoit la longueur du retour
			MPI_Recv (&lengthRet, 2, MPI_INT, threadDest, tag, MPI_COMM_WORLD, &status[2]);
			
			// Recupération & Affichage de la reponse Serveur (erreur ou info)
			ret = (char *) malloc(lengthRet+1); 
			MPI_Recv (ret, lengthRet+1, MPI_CHAR, threadDest, tag, MPI_COMM_WORLD, &status[3]);
			printf("%s\n",ret);
			
			// On incrémente le numéro du thread
			threadDest=(threadDest+1)%size;
			if (threadDest==0) threadDest=1;
		}
		free(commande);
		
	// Serveurs	
	} else {	
	
		// TODO: faire fonctionner les serveurs en round-robin (tourniquet) les fichiers doivent être ecrits sur une machine différentes à chaque fois.
		
		while(1){
			// On recoit la longueur de la commande
			MPI_Recv (&length, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &status[0]);
			
			// Réception de la commande
			commande = (char *) malloc(length+1); // on set la taille de la commande
			MPI_Irecv(commande,length+1,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
			MPI_Wait (&request, &status[1]);
			
			// Analyse et traitement des commandes
			char * retour;
			retour = cmd_touch(commande,nameMach);
			if (!strcmp(retour,"")){
				retour = cmd_showdata(commande);
			}
			if (!strcmp(retour,"")){
				retour = cmd_rm(commande);
			}
			
			if (!strcmp(retour,"")){
				retour="Commande inconnue";
			}
			//On envoi la longueur du retour
			lengthRet = strlen(retour);
			MPI_Send (&lengthRet, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
			
			//Envoi du retour (erreur ou info)
			MPI_Send (retour, lengthRet, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
		}

	}
	
	MPI_Finalize();
	
	return 0;
}