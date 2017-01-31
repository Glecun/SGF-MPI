#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h> // function isprint
#include <dirent.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "gestionFichier.h"
#include "utils.h"
#include "commandes.h"
#include "constante.h"
#include "functions_filesystem.h"

int main (int argc, char ** argv){
	
	//Variables
	int num; // numéro du thread						
	char * commande = (char * ) malloc(sizeof(char)* 4096);// variable qui contient la commande
	char ** split_commande = malloc(256*256*sizeof(char));
	int tag = 0;		
	char *ret;// info de retour des serveurs0
	int size;// nombre de threads
	int length;// taille de la commande
	int lengthRet;// taille du retour
	int lenNameMach;// longueur du nim de la machine
	int argc_commande = 3; // ### pas sure
	char nameMach[MPI_MAX_PROCESSOR_NAME]; 	// nom de la machine
	MPI_Request request;
	MPI_Status status[4];
	
	//Init MPI
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &num);	// init le numéro du thread
	MPI_Comm_size( MPI_COMM_WORLD, &size);	// nombre de threads
	MPI_Get_processor_name(nameMach, &lenNameMach);
	
	// Si l'utilisateur demande l'initialisation
	if( argc == 2 && !strcmp( argv[1] , "-i" ) ){
		FILE *fp;
	
		if( num == 0 ){
		
			char version = 1; // version a 1 //  ### changer en fonction d'une option
			unsigned long long dossier_parent = 0;

			// on calcul le pointeur a partir duquel on peux écrire ( en nombre d'octet)
			unsigned long long index_last_cursor= 8 + 1 + 8 + 3 + 3*8 + 3*255; // long long pour forcer a 8 octet, de base sur linux un simple long aurait suffit..
		
			// Création du fichier d'index
			if_file_isset_then_delete( FILE_NAME_INDEX );
		
			// on créer les fichiers
			fp = fopen(FILE_NAME_INDEX, "w+"); // contient la liste des fichiers et leur emplacement dans le fichier stockage
			fseek(fp, 0, SEEK_SET); // on remet le curseur au début du fichier

			// on écris le pointeur vers la derniere ligne disponible (sur 64 bit), suivis d'un indicateur de version (8 bit)
			fwrite(&index_last_cursor, sizeof(index_last_cursor), 1, fp);
			fwrite(&version, sizeof(version), 1, fp);
			fwrite(&dossier_parent, sizeof(dossier_parent), 1, fp);
			// maintenant au cas ou.. pour la suite on prévoie.. on met 3 char, 3 unsigned longlong et 3 char 255
			fseek(fp, 3 + 3*8 + 3*255, SEEK_CUR);
			fclose(fp);
			// creation, avec touch de "/" , l'element racine ###
		
			// Création du dossier SWP
			if_dir_isset_then_delete( PATH_FOLDER_SWAP );
			mkdir( PATH_FOLDER_SWAP , S_IRWXU );
		
		} else {

			char nom_dossier_bdd [1000]; 
			snprintf( nom_dossier_bdd , 1000 , "%s%d/" , PATH_GLOBAL_DIR , num );
	
			//printf("Création du dossier %s\n" , nom_dossier_bdd);
	
			// Création du fichier BDD
			char nom_fichier_bdd [1000]; 

			snprintf( nom_fichier_bdd , 1000 , "%s%s" , nom_dossier_bdd , FILE_NAME_STOCKAGE );
			//if_file_isset_then_delete( nom_fichier_bdd );

			// Création du dossier contenant le numéro de num
			if_dir_isset_then_delete( nom_dossier_bdd );
			mkdir( nom_dossier_bdd , S_IRWXU );
	
			
	
			//printf("Création du fichier %s\n" , nom_fichier_bdd);
	
			fp = fopen(nom_fichier_bdd, "w+"); // contient le contenu des fichiers
			fseek(fp, 0, SEEK_SET);
			unsigned long long stock_last_cursor = 8;
			fwrite(&stock_last_cursor, sizeof(stock_last_cursor), 1, fp);
			fclose(fp);
		}
	}
	
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
				MPI_Abort(MPI_COMM_WORLD,0);
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
