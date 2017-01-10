#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>
#include "gestionFichier.h"

int main (int argc, char ** argv){
	//Variables
	int num;		// numéro du thread
	int cmd = 0;	// **temporaire**: variable qui détermine la nature de la commande: 0 -> creation fichier, 1-> suppression fichier
	int tag = 0;
	int size;			//nombre de threads
	MPI_Request request;
	MPI_Status status;
	
	//supprimer le contenu du fichier commun dès le début
	supprimerContenu();
	
	//Init MPI
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &num);	// init le numéro du thread
	MPI_Comm_size( MPI_COMM_WORLD, &size);		// nombre de threads
	
	// Client unique qui envoi des commandes
	if (0==num){
		
		//Ex 1 - le client veut creer des fichiers 
		for (int i =1 ;i<size;i++){ 
			printf ("Client: Serveur %d je t'envoi %d \n", i,cmd);
			MPI_Isend(&cmd,1,MPI_INT,i,tag,MPI_COMM_WORLD,&request); //TODO: pour le moment il envoie une fois a chaque thread
		}
		
	// Serveurs	
	} else {	
		// TODO: faire fonctionner les serveurs en round-robin (tourniquet)
	
		// Réception de la commande
		MPI_Irecv(&cmd,1,MPI_INT,0,tag,MPI_COMM_WORLD,&request);
		MPI_Wait (&request, &status);
		printf ("Serveur %d: j'ai reçu %d \n", num, cmd);
		
		// Analyse et traitement de la commande
		if (cmd==0){			// Creation
			printf ("Serveur %d: je crée un fichier\n", num);
			//Mise à jour du fichier commun
			char chemin[50];
			sprintf(chemin, "./fichier_du_serv%d ",num);
			ajouterLigne("Machine 1\0",chemin);
			//Creation du fichier réel
				//TODO: brancher le truc de joachim
		}
		else if (cmd==1){		// Suppression
			printf ("Serveur %d: je supprime un fichier\n", num);
		}
	}
	
	MPI_Finalize ();
	
	return 0;
}
