#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constante.h"
#include "get_env.h"
#include "mpi.h"

/*
 * Fonction qui permet d'avoir le la dernière position du fichier index pour pouvoir créer une entrée pour un nouveau fichier
 * @param c - l'endroit ou sera stocké la position du pointer
 * @return cursor - la valeur de l'index du cursor à la fin du fichier BDD
 */
void get_index_last_cursor( unsigned long long * c ){
        FILE *fp = fopen(FILE_INDEX, "r");

        if(fp == NULL){
                printf("Fichier non trouver, ou acces non permis.\n");
        }

        fseek(fp, 0, SEEK_SET); // on passe le cursor

        unsigned long long cursor;

        fread(&cursor, sizeof(cursor), 1, fp);

        (*c) = cursor;
        fclose(fp);
};

/*
 * Fonction qui permet d'avoir le la dernière position de la BDD pour pouvoir sotcker à la fin
 * @param c - l'endroit ou sera stocké la position du pointer
 * @return cursor - la valeur de l'index du cursor à la fin du fichier BDD
 */
void get_stock_last_cursor( unsigned long long * c , char * nom_fichier_bdd ){
        FILE *fp = fopen(nom_fichier_bdd, "r");

        if(fp == NULL){
                printf("Fichier non trouver, ou acces non permis.\n");
        }

        fseek(fp, 0, SEEK_SET); // on passe le cursor

        unsigned long long cursor;

	fread(&cursor, sizeof(cursor), 1, fp);

        (*c) = cursor;
        fclose(fp);
};


/*
* Fonction qui recupere le round robin actuel.
* @param rr - l'endroit ou sera stocké le round_robin
* @return rr - la valeur du round robin
*/

void get_round_robin(unsigned long long * rr){
	
	 FILE *fp = fopen(FILE_INDEX, "r");
                                                                        
         if(fp == NULL){
                 printf("Fichier non trouver, ou acces non permis.\n");
         }
                                                                        
         fseek(fp, 8 + 1 + 8, SEEK_SET);
                                                                        
         unsigned long long round;
                                                                        
         fread(&round, sizeof(round), 1, fp);
                                                                        
         (*rr) = round;
         fclose(fp);
}

/*
 * Fonction qui permet de modifier la valeur du round_robin
 * @param rr - le nouveau round robin
 */
void set_round_robin( unsigned long long rr){
        FILE *fp = fopen(FILE_INDEX, "r+");

        if(fp == NULL){
                printf("Fichier non trouver, ou acces non permis.\n");
        }

        fseek(fp, 8 + 1 + 8, SEEK_SET); // on place le cursor

        fwrite(&rr, sizeof(rr), 1, fp);

        fclose(fp);
};


/*
 * Fonction qui permet de modifier la valeur du dernier cursor du fichier de BDD
 * @param c - l'endroit ou sera stocké la position du pointer
 * @return cursor - la valeur de l'index du cursor à la fin du fichier BDD
 */
void set_stock_last_cursor( unsigned long long cursor , char * nom_fichier_bdd ){
        FILE *fp = fopen(nom_fichier_bdd, "r+");

        if(fp == NULL){
                printf("Fichier non trouver, ou acces non permis.\n");
        }

        fseek(fp, 0, SEEK_SET); // on passe le cursor
	
	fwrite(&cursor, sizeof(cursor), 1, fp);

        fclose(fp);
};

/**
 * Permet d'avoir la version du système de gestion de fichier (actuellement inutilisé car une seule version)
 */
void get_version( char * v ){
        FILE *fp = fopen(FILE_INDEX, "r");

        if(fp == NULL){
                printf("Fichier non trouver, ou acces non permis.\n");
        }

        fseek(fp, 8, SEEK_SET); // on passe le cursor

        char version;

        fread(&version, sizeof(version), 1, fp);

        (*v) = version;
        fclose(fp);
};

/**
 * Permet d'avoir le numéro de la ligne du parent du dossier courant de l'utilisateur (dossier parent de l'environnement)
 * @param c - l'endroit ou sera stocké la position du pointer
 * @return cursor - la valeur de l'index du cursor à la fin du fichier BDD
 */
void get_parent( unsigned long long * p){
        FILE *fp = fopen(FILE_INDEX, "r");

        if(fp == NULL){
                printf("Fichier non trouver, ou acces non permis.\n");
        }

        fseek(fp, 9, SEEK_SET); // on passe le cursor et la version

        unsigned long long parent;

        fread(&parent, sizeof(parent), 1, fp);

        (*p) = parent;
        fclose(fp);
};

/**
 * Permet de modifier la valeur du dossier parent de l'environnement de l'utilisateur
 * @param c - la valeur du nouveau parent
 */
void set_parent( unsigned long long parent){
        FILE *fp = fopen(FILE_INDEX, "r+");

        if(fp == NULL){
                printf("Fichier non trouver, ou acces non permis.\n");
        }

        fseek(fp, 9, SEEK_SET); // on passe le cursor et la version

        fwrite(&parent, sizeof(parent), 1, fp);

        fclose(fp);
};

unsigned long long file_exist(char arg_type_file, char * arg_file_name){
        /*
                Préviens si un fichier|dossier existe de nom file_name
                retourne la valeur du curseur de debut si le fichier exist, sinon renvoie 0
         */

        FILE *fp = fopen(FILE_INDEX, "r");
        if(fp == NULL){
                printf("Fichier non trouver, ou acces non permis.\n");
        }

        unsigned long long cursor_end;
        unsigned long long cursor_tmp = FILE_BASE_ENV;
        unsigned long long parent;

        fseek(fp, 0,  SEEK_SET);
        fread(&cursor_end, sizeof(cursor_end), 1, fp);
        get_parent(&parent);
        fseek(fp, cursor_tmp,  SEEK_SET);

        char active;
        unsigned long long tmp_parent;
        char type_file;
        char file_name[255];

        while(cursor_tmp < cursor_end){
                fread(&active, sizeof(active), 1, fp);
                fread(&tmp_parent, sizeof(tmp_parent), 1, fp);
                fread(&type_file, sizeof(type_file), 1, fp);
                if(active && tmp_parent == parent){ // on verifie que c'est un fichier, qu'il est actif, et qu'il est bien dans le repertoire courant
                        // on affiche les infos
                        if(type_file == arg_type_file){ // fichier
                                fread(file_name, sizeof(file_name), 1, fp);
                                if(strcmp(file_name, arg_file_name) == 0){
                                        fclose(fp);
                                        return cursor_tmp;
                                }
                        }
                }
                cursor_tmp += INDEX_LINE_SIZE;
                fseek(fp, cursor_tmp, SEEK_SET); // on place le curseur au bon endroit pour la suite ( parce qu'on ne lis pas tout.. surtout si c'est un fichier..
        }

        fclose(fp);
        return 0;
}

void put_file(char * path_filename, unsigned long long *file_size, unsigned long long num_rank, unsigned long long * cursor_stock){
	int tag = 0;	
	MPI_Status status;        
	/*
           fonction, prennant un fichier sur le systeme hôte et le met dans le filesystem.
           écris dans stockage.jjg a partir du cursor_stock
         */

        // on ouvre le 1er fichier en lecture
        FILE *fp_src = fopen(path_filename, "r");
        unsigned long long size_src;
        fseek(fp_src, 0, SEEK_END);
        size_src = ftell(fp_src);
        (*file_size) = size_src; // on transmet la taille du fichier

	// Lecture du fichier dans /SWP/
	char file_content[size_src];
	file_content[0] = '\0';
	fseek(fp_src, 0, SEEK_SET);
	fread(file_content, sizeof(char) * size_src, 1, fp_src);
	fclose(fp_src);

	if( size_src > 0 ){
		int type_commande = 2;

		MPI_Send( &type_commande, 1, MPI_INT, num_rank, tag, MPI_COMM_WORLD);

		int res_type_commande = 0;
		MPI_Recv( &res_type_commande, 1, MPI_INT, num_rank, tag, MPI_COMM_WORLD, &status );

		if( res_type_commande > 0 ){

			// On envoie la taille du fichier
			MPI_Send( &size_src, 1, MPI_UNSIGNED_LONG, num_rank, tag, MPI_COMM_WORLD );

			// On envoie le contenu du fichier
			MPI_Send( &file_content, size_src, MPI_CHAR, num_rank, tag, MPI_COMM_WORLD );

			unsigned long long int new_cursor_stock;
			// récéption de la nouvelle valeur de fin de fichier
			MPI_Recv( &new_cursor_stock, 1, MPI_UNSIGNED_LONG_LONG, num_rank, tag, MPI_COMM_WORLD, &status );
			(*cursor_stock)=new_cursor_stock;
		}
	}
}

void extract_file(char * path_filename, unsigned long long cursor_stock, unsigned long long file_size, unsigned long long num_rank){
	int tag = 0;	
	MPI_Status status[4];
	/*
		fonction qui prend une partie du fichier stockage, et la met dans le fichier du systeme hôte
	*/
	/*	
		// on lis l'info dans stockage.jjg => seulement en séquentiel
		FILE *fp_src = fopen(FILE_STOCK, "r");
		fseek(fp_src, cursor_stock, SEEK_SET);
		char buffer[file_size];
		fread(buffer, sizeof(buffer), 1, fp_src);
		fclose(fp_src);
	*/
	if( file_size != 0 ){
		int type_commande = 1;

		MPI_Send( &type_commande, 1, MPI_INT, num_rank, tag, MPI_COMM_WORLD);

		int res_type_commande = 0;
		MPI_Recv( &res_type_commande, 1, MPI_INT, num_rank, tag, MPI_COMM_WORLD, &status[0] );

		if( res_type_commande > 0 ){

			// données pour la lecture de fichier
			unsigned long long int param_commande_get[2];
			param_commande_get[ 0 ] = cursor_stock;
			param_commande_get[ 1 ] = file_size;

			// on attend la position du fichier et sa taille
			MPI_Send( param_commande_get, 2, MPI_UNSIGNED_LONG_LONG, num_rank, tag, MPI_COMM_WORLD );

			// récupération du contenu du fichier
			char buffer[file_size];
			MPI_Recv( buffer, file_size, MPI_CHAR, num_rank,tag, MPI_COMM_WORLD, &status[0] );

			// on écris l'info dans le fichier path_filename
			FILE *fp_dest = fopen(path_filename, "r+");
			fseek(fp_dest, 0, SEEK_SET);
			fwrite(buffer, sizeof(buffer), 1, fp_dest);
			fclose(fp_dest);
		}
	}
}


/**
 * Fonction qui permet de créer une entrée pour un fichier dans le fichier index
 */
void ajouterLigne(unsigned long long cursor, char active, unsigned long long parent, char file_type, char * file_name, unsigned long long file_cursor_stock, unsigned long long file_size, unsigned long long machine){
        FILE *fp = fopen(FILE_INDEX, "r+");

        if(fp == NULL){
                printf("Fichier non trouver, ou acces non permis.\n");
        }

        fseek(fp, cursor, SEEK_SET); // on passe le cursor au bon endroit

        fwrite(&active, sizeof(active), 1, fp);
        fwrite(&parent, sizeof(parent), 1, fp);
        fwrite(&file_type, sizeof(file_type), 1, fp);
        fwrite(file_name, sizeof(char) * 255, 1, fp);
        if(file_type == FICHIER){
        	fwrite(&file_cursor_stock, sizeof(file_cursor_stock), 1, fp);
        	fwrite(&file_size, sizeof(file_size), 1, fp);
        	fwrite(&machine, sizeof(machine), 1, fp);
        }

        fclose(fp);
};

void supprimerLigne(unsigned long long cursor){
	FILE *fp = fopen(FILE_INDEX, "r+");
        if(fp == NULL){
                printf("Fichier non trouver, ou acces non permis.\n");
        }
	fseek(fp, cursor, SEEK_SET);
	
	char active = 0;

	fwrite(&active, sizeof(active), 1, fp);
	
	fclose(fp);
}


unsigned long long get_fils(unsigned long long cursor){
         FILE *fp = fopen(FILE_INDEX, "r");
         if(fp == NULL){
                 printf("Fichier non trouver, ou acces non permis.\n");
         }
                                                                                                                                                                    
         unsigned long long cursor_end;
         unsigned long long cursor_tmp = FILE_BASE_ENV;
         unsigned long long parent;
                                                                                                                                                                    
         fseek(fp, 0,  SEEK_SET);
         fread(&cursor_end, sizeof(cursor_end), 1, fp);
         get_parent(&parent);
         fseek(fp, cursor_tmp,  SEEK_SET);
                                                                                                                                                                    
         char active;
         unsigned long long tmp_parent;
                                                                                                                                                                    
         while(cursor_tmp < cursor_end){
                 fread(&active, sizeof(active), 1, fp);
                 fread(&tmp_parent, sizeof(tmp_parent), 1, fp);
                 if(active && tmp_parent == cursor){ // on verifie que c'est un fichier, qu'il est actif, et qu'il est bien dans le repertoire courant
                 	fclose(fp);
                        return cursor_tmp;
                 }
                 cursor_tmp += INDEX_LINE_SIZE;
                 fseek(fp, cursor_tmp, SEEK_SET); // on place le curseur au bon endroit pour la suite ( parce qu'on ne lis pas tout.. surtout si c'est un fichier..
         }
                                                                                                                                                                    
         fclose(fp);
         return 0;
}

void supprimerDossier(char * path, unsigned long long parent){
	char active;
	unsigned long long parent_tmp;
	char file_type;
	char file_name[255];

	unsigned long long cursor;	

	while((cursor = get_fils(parent)) != 0){
		FILE *fp = fopen(FILE_INDEX, "r+");
                                                                                
                if(fp == NULL){
                        printf("Fichier non trouver, ou acces non permis.\n");
                }

		fseek(fp, cursor, SEEK_SET);
		fread(&active, sizeof(active), 1, fp);		
		fread(&parent_tmp, sizeof(parent_tmp), 1, fp);		
		fread(&file_type, sizeof(file_type), 1, fp);		
		fread(&file_name, sizeof(file_name), 1, fp);		

		fclose(fp);		

		if(file_type == FICHIER){
			printf("fichier %s supprimer\n", file_name);
			supprimerLigne(cursor);
			
		} else {
			supprimerDossier(file_name, cursor);
		}
	}

	printf("dossier %s supprimer\n", path);
	supprimerLigne(parent);
}
