#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>
#include "gestionFichier.h"
#include "utils.h"
#include "commandes.h"
#include "constante.h"
#include "functions_filesystem.h"

int main (int argc, char ** argv){
	
	//Variables
	int num;// numéro du thread						
	char * commande = (char * ) malloc(sizeof(char)* 4096);// variable qui contient la commande
	char ** split_commande = malloc(256*256*sizeof(char));
	int tag = 0;
	char *ret;// info de retour des serveurs
	int size;// nombre de threads
	int length;// taille de la commande
	int lengthRet;// taille du retour
	int lenNameMach;// longueur du nim de la machine
	int argc_commande = 3; // ### pas sure 

	char nameMach[MPI_MAX_PROCESSOR_NAME];// nom de la machine
	MPI_Request request;
	MPI_Status status[4];//Init MPI
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &num);// init le numéro du thread
	MPI_Comm_size( MPI_COMM_WORLD, &size);// nombre de threads
	MPI_Get_processor_name(nameMach, &lenNameMach);
	
	// Client unique qui envoi des commandes
	if (0==num){
		// Supprimer le contenu du fichier commun dès le début
		supprimerContenu(); // ### a enlever..
		
		// Premier thread destinataire
		int threadDest=1;		

		// Simulation terminal
		while(1){
			// On demande la commande
			printf("user@machine$>");
			fflush(stdout);
			if(fgets(commande, 4096, stdin) == NULL){
				printf("lol");	
				break;
			}
			// on supprime la touche entrée
			commande[strcspn(commande, "\n")] = 0;

			// on trim la commande
			trim(commande);
			
			// Sortie du programme (exit)
			char *str_regex = "^[ ]*exit[ ]*$";
			if(!execRegex(str_regex,commande)){ 
				MPI_Abort(MPI_COMM_WORLD,0); // ###
			}
			
			parsing(commande, split_commande, &argc_commande);

			// Si on demande le contenu du fichier data.bd (index)
			ret = cmd_showdata(commande);
			
			// On envoi aux serveurs que si il s'agit de suppression ou d'ajout (retour vide jusque la)
			if (!strcmp(ret,"")){ 
				
				// TODO: Dans le cas de suppression, il faudrait déterminer quelle machine possède les fichiers concernés en regardant dans la table pour envoyer les demandes de suppression
				// TODO: Gérer le multi fihier en distribué, ex - la commande: touch coucou coucou2 devrait etre analysée en splitant les fichiers pour les envoyer un à un en round-robin

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
				
				// On incrémente le numéro du thread destinataire seulement si le fichier a été crée
				//printf("thread = %d\n",threadDest);
				char *str_regex_threadDest = ".*fichier cré.*";
				if (!execRegex(str_regex_threadDest,ret) ){
					threadDest=(threadDest+1)%size;
					if (threadDest==0) threadDest=1;
				}
			}
			
			//affichage du retour
			printf("%s %s\n",ret, commande);

		}
		
		free(commande);
		
	// Serveurs	
	} else {	
		
		while(1){
			// On recoit la longueur de la commande
			MPI_Recv (&length, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &status[0]);
			
			// Réception de la commande
			commande = (char *) malloc(length+1); // on set la taille de la commande
			MPI_Irecv(commande,length+1,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
			MPI_Wait (&request, &status[1]);
			
			// Analyse et traitement des commandes
			char * retour;
			
			// Commande de création
			retour = cmd_touch(commande,nameMach);
			
			// Commande de suppression
			if (!strcmp(retour,"")){
				retour = cmd_rm(commande);
			}
			
			// Commande inconnue
			if (!strcmp(retour,"")){
				retour="Commande inconnue\0";
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
