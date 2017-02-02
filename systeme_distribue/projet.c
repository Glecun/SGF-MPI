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
#include "commandes.h"
#include "constante.h"
#include "functions_filesystem.h"
#include "get_env.h"

int main (int argc, char ** argv){
	
	//Variables
	int num; // numéro du thread						
	char * commande = (char * ) malloc(sizeof(char)* 4096);// variable qui contient la commande
	char ** split_commande = malloc(256*256*sizeof(char));
	int tag = 0;	
	MPI_Status status[4];	
	//char *ret;// info de retour des serveurs0
	int size;// nombre de threads
	//int length;// taille de la commande
	//int lengthRet;// taille du retour
	int lenNameMach;// longueur du nim de la machine
	int argc_commande = 3;
	char nameMach[MPI_MAX_PROCESSOR_NAME]; 	// nom de la machine
	//MPI_Request request;
	
	//Init MPI
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &num);	// init le numéro du thread
	MPI_Comm_size( MPI_COMM_WORLD, &size);	// nombre de threads
	MPI_Get_processor_name(nameMach, &lenNameMach);

	// Génération du nom du dossier BDD
	char nom_dossier_bdd [1000]; 
	snprintf( nom_dossier_bdd , 1000 , "%s%d/" , PATH_GLOBAL_DIR , num );

	//printf("Création du dossier %s\n" , nom_dossier_bdd);

	// Création du nom du fichier BDD
	char nom_fichier_bdd[1000];
	snprintf(nom_fichier_bdd, 1000, "%s%s", nom_dossier_bdd, FILE_NAME_STOCKAGE);
	
	// Si l'utilisateur demande l'initialisation
	if( argc == 2 && !strcmp( argv[1] , "-i" ) ){
		FILE *fp;
	
		if( num == 0 ){
		
			int version = 1; // version a 1 //  ### changer en fonction d'une option
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

			sleep( 1 );
		
		} else {

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
	
		if( access( FILE_NAME_INDEX, R_OK|W_OK ) == -1 ) { // si il existe pas
			printf("Avez-vous éffectué l'initialisation \"-i\"?\n");
			MPI_Abort(MPI_COMM_WORLD, 0);
		}
		
		// Simulation terminal
		while(1){
			// On demande la commande
			printf("user@machine$>");
			fflush(stdout);
			if(fgets(commande, 4096, stdin) == NULL){
				break; // ### TODO a gere un peu plus drastiquement
			}
			// on supprime la touche entrée
			commande[strcspn(commande, "\n")] = 0;

			// on trim la commande
			trim(commande);

			if(!strcmp(commande, "exit")){
				MPI_Abort(MPI_COMM_WORLD, 0);
			} else if(!strcmp(commande, "")){
				continue;
			}

			// on découpe la commande en argument que l'on stocke dans split_commande
			parsing(commande, split_commande, &argc_commande);

			if(!strcmp(split_commande[0], "touch")){
				cmd_touch(split_commande);
			} else if (!strcmp(split_commande[0], "ls")){
				cmd_ls(split_commande);
			} else if (!strcmp(split_commande[0], "mkdir")){
				cmd_mkdir(split_commande);
			} else if (!strcmp(split_commande[0], "del")){
				cmd_del(split_commande);
			} else if (!strcmp(split_commande[0], "cd")){
				cmd_cd(split_commande);
			} else if (!strcmp(split_commande[0], "pwd")){
				cmd_pwd(split_commande);
			} else if (!strcmp(split_commande[0], "rename")){
				cmd_rename(split_commande);
			} else if (!strcmp(split_commande[0], "vim")){
				cmd_vim(split_commande);
			} else if (!strcmp(split_commande[0], "deldir")){
				cmd_deldir(split_commande);
			} else if (!strcmp(split_commande[0], "help") || !strcmp(split_commande[0], "?")){
                		cmd_help();
			} else {
				printf("La commande %s est inconnue\n", split_commande[0]);
			}		
	
		}
		
		free(commande);
	// Serveurs	
	} else {
		
		while(1){
			// 0 => GET , 1 => SET , 2 => ?
			int type_commande;

			// On recoit le type de commande
			MPI_Recv( &type_commande, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status[0] );
			
			// 0 => ERREUR , 1 => OK (en attente d'un autre message)
			int retour_commande = 0;

			// Si le serveur demande le contenu d'un fichier
			if( type_commande == 1 ){

				retour_commande = 1;
				// on retourne 1 pour prévenir qu'on attend les données du fichier
				MPI_Send(&retour_commande,1,MPI_INT,0,tag,MPI_COMM_WORLD);

				// données pour la lecture de fichier
				unsigned long long int param_commande_get[2];

				// on attend la position du fichier et sa taille
				MPI_Recv( &param_commande_get, 2, MPI_UNSIGNED_LONG_LONG, 0, tag, MPI_COMM_WORLD, &status[0] );

				// récupération des deux paramètres envoyé par le serveur
				unsigned long long int cursor_deb_file = param_commande_get[0];
				unsigned long long int file_size = param_commande_get[1];

				// récupération du contenu du fichier
				FILE *fp_src = fopen(nom_fichier_bdd, "r");
				fseek(fp_src, cursor_deb_file, SEEK_SET);
				char buffer[file_size];
				fread(buffer, sizeof(buffer), 1, fp_src);
				fclose(fp_src);

				// on retourne le contenu du fichier
				MPI_Send(buffer,file_size,MPI_CHAR,0,tag,MPI_COMM_WORLD);
				

			// Si le serveur demande de stocker un fichier
			} else if( type_commande == 2 ){
				retour_commande = 1;
				// on retourne 1 pour prévenir qu'on attend le contenu du fichier
				MPI_Send(&retour_commande,1,MPI_INT,0,tag,MPI_COMM_WORLD);
				
				unsigned long long int taille_du_nouveau_fichier = 0;

				// on attend la taille du nouveau fichier
				MPI_Recv( &taille_du_nouveau_fichier, 1, MPI_UNSIGNED_LONG, 0, tag, MPI_COMM_WORLD, &status[0] );

				char contenu_file[taille_du_nouveau_fichier];

				// On attend le contenu du nouveau fichier 
				MPI_Recv( &contenu_file, taille_du_nouveau_fichier, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status[0] );

				unsigned long long cursor_stock;
				get_stock_last_cursor(&cursor_stock, nom_fichier_bdd);

				// on ouvre le fichier stockage en écriture
				FILE *fp_dest = fopen(nom_fichier_bdd, "r+");
				fseek(fp_dest, cursor_stock, SEEK_SET);
				for(unsigned long long int i=0; i<taille_du_nouveau_fichier;i++ ){
					fwrite(&contenu_file[i], sizeof(char), 1, fp_dest);
				}
				fclose(fp_dest);

				unsigned long long int longeur_new_file = (cursor_stock+taille_du_nouveau_fichier);
				
				// on met a jour le curseur
				set_stock_last_cursor(  longeur_new_file, nom_fichier_bdd );

				MPI_Send(&cursor_stock,1,MPI_UNSIGNED_LONG_LONG,0,tag,MPI_COMM_WORLD);

			// sinon => commande inconnu
			} else {
				MPI_Send(&retour_commande,1,MPI_INT,0,tag,MPI_COMM_WORLD); // on retourne 0 pour prévenir de l'erreur
			}
		}

	}
	
	MPI_Finalize();
	
	return 0;
}
