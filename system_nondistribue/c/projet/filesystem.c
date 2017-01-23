#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // fonction access
#include <ctype.h> // function isprint
#include <sys/wait.h>

#include "constante.h"
#include "get_env.h"

static int exec_prog (char ** argv){
	// ok alors, on va tenter d'expliquer.. 

	// d'abord le char **, parce que char ** envoie qu'une valeur, la valeur du pointeur qui pointe sur quelque chose. ( ici c'est sur un tableau de 256 pointeur type char * )
	// je n'utilise pas de char[256][256] parce que ça enverrais pas une seul valeur mais bien 256 valeur, et on ne peux changer leur valeurs avec une simple initialisation du type "r[2] = NULL", car ce qui sera changer sera la valeur dans la fonctions et non pas celle dans la fonction principale.  

	pid_t   my_pid;

	if (0 == (my_pid = fork())) {		
		if (-1 == execve(argv[0], (char **)argv , NULL)) {
			printf("child process execve failed [%m]\n"); // ### a changer pour avoir un " commande inexistante "..
			exit(0); // si ça rate, on exit le fork, s'assurant de ne pas avoir le fork du programme continuer la boucle de commande.. ) 
			return -1; // cette instruction ne sert donc pas
		}
	}                                        	

	return 0;
}

void  parsing( char * s, char ** r, int * c){
	/*
		Need des commentaires ici !!! 
		En gros la fonction parsing prend une chaine de charactere s, et la découpe de la bonne façon, la faisait rentrer dans le tableau r. le dernierr valeur du tableau étant NULL.
	*/

	int i;
	int isQuoted = -1;
	int last_cursor = 0;
	int compteur = 0;	
	char ** buffer = malloc(256 * sizeof(char*)); // buffer des sous chaine de charactere

	// printf("%s\n", s);

	for(i=0; i < strlen(s); i++){
		if(s[i] == '"'){
			if(i-1 >= 0){
				if(s[i-1] == '\\'){
					// printf("\"");
				} else {
					isQuoted *= -1;
				}
			} else {
				isQuoted *= -1;
			}
		} else {
			if(s[i] == ' '){
				if(isQuoted == 1){
					// printf("%c", s[i]);
				} else {
					buffer[compteur] = malloc(256 * sizeof(char));
					memcpy( buffer[compteur], &s[last_cursor], i - last_cursor);
					buffer[compteur][i - last_cursor] = '\0';
					r[compteur] = buffer[compteur]; 
					// printf(" --- %d -> %d", last_cursor, i - 1);
					last_cursor = i + 1;
					// printf("\n");
					compteur++;
				}
			} else {
				// printf("%c", s[i]);
			}
		}
	}

	buffer[compteur] = malloc(256 * sizeof(char)); 
	memcpy( buffer[compteur], &s[last_cursor], i - last_cursor);
        buffer[compteur][i - last_cursor] = '\0';
        r[compteur] = buffer[compteur]; 
        
	// printf(" --- %d -> %d", last_cursor, i - 1);
	// printf(" --- %d -> %d", last_cursor, i);
	// printf("\n");
	compteur++;

	r[compteur] = NULL;

	*c = compteur;

	/*
	i = 0;
	while(i < 256 && r[i] != NULL){
		printf("%s\n", r[i]);
		i++;
	}
	*/
}


int print_y_n(char * s){
	/**
	 *	Function demandant une question fermé. oui ou non.
	 *	return true ou false
	 *
	 *	Exemple : 
	 *	if(print_y_n("lol ?")){
	 *		printf("oui");
	 *	}
	 **/

	printf("%s [y/n] ", s);
	char b;
	b = getchar();
	int answer;
	while(b != 'y' && b !='n'){
		while ( (b = getchar()) != '\n' && b != EOF ) { } // on vide le stdin.. 
		printf("%s [y/n] ", s);
		b = getchar();
	}
	answer = (int) b;

	while ( (b = getchar()) != '\n' && b != EOF ) { }
	return (answer == 'y');
}

char *inputString(FILE* fp, size_t size){
	/*
	   Alloue dynamiquement la taille de la chaine de charactere.
	 */
	// source : http://stackoverflow.com/questions/16870485/how-can-i-read-an-input-string-of-unknown-length
	//The size is extended by the input with the value of the provisional
	char *str;
	int ch;
	size_t len = 0;
	str = realloc(NULL, sizeof(char)*size);//size is start size
	if(!str)return str;
	while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
		str[len++]=ch;
		if(len==size){
			str = realloc(str, sizeof(char)*(size+=16));
			if(!str)return str;
		}
	}
	str[len++]='\0';

	return realloc(str, sizeof(char)*len);
}


int main(int argc, char **argv){
	int fin_terminal = 0;
	char * commande; // commandes entrée dans le terminal
	char ** split_commande = malloc(256*256*sizeof(char)); // commande une fois passer a travers parsing()
	int argc_commande; // nombre de commande..
	char buffer_string[1024]; // formations des phrases avec snprintf
	int i;
	int compteur_while = 0;

	// "constante"
	char * file_index = "index.jjg";
	char * file_stockage = "stockage.jjg";	

	// Options // se referer a l'exemple testopt.c
	int iflags = 0; // initialisation : crée les deux premier fichiers
	int c; // case variable
	opterr = 0;

	// on determine les options
	while ((c = getopt (argc, argv, "i")) != -1){
		switch (c){
			case 'i': // initialisation
				iflags = 1;
				break;
			case '?': // cas erreurs
				if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
				return 1;
			default:
				abort ();
		}
	}

	if(iflags){ // initialisation
		//////
		// on crée les différents fichiers
		//////		

		// on verifie qu'il y a assez de place sur le disque
		// oué.. on verra ###

		// on verifie que des fichiers n'ont pas déjà ce nom
		if( access( file_index, F_OK ) != -1 ) { // si il existe // ### faire sous forme de fonction
			// en gros snprintf permet de placer des variables dans une chaine de la même façon que printf, sauf que ça ne l'affiche pas ça forme la chaine
			snprintf(buffer_string, sizeof(buffer_string), "fichier : %s existe deja. le supprimer ?", file_index);
			if(print_y_n(buffer_string)){ // on supprime le fichier si oui
				if(remove(file_index) == 0){
					printf("fichier %s a ete supprimer.\n", file_index);
				} else {
					printf("error : fichier %s non supprimer.\n", file_index);
				}
			}
		}

		if( access( file_stockage, F_OK ) != -1 ) { // si il existe // ### faire sous forme de fonction
                	snprintf(buffer_string, sizeof(buffer_string), "fichier : %s existe deja. le supprimer ?", file_stockage);
                	if(print_y_n(buffer_string)){ // on supprime le fichier si oui
                		if(remove(file_stockage) == 0){
                			printf("fichier %s a ete supprimer.\n", file_stockage);
                		} else {
                			printf("error : fichier %s non supprimer.\n", file_stockage);
                		}
                	}
                }

		// on crée les fichiers
		FILE *fp = fopen(file_index, "w+"); // contient la liste des fichiers et leur emplacement dans le fichier stockage
		fseek(fp, 0, SEEK_SET); // on remet le curseur au début du fichier
		// on calcul le pointeur a partir duquel on peux écrire ( en nombre d'octet)
		unsigned long long index_last_cursor= 8 + 1 + 8 + 3 + 3*8 + 3*255; // long long pour forcer a 8 octet, de base sur linux un simple long aurait suffit..



		char version = 1; // version a 1 //  ### changer en fonction d'une option
		unsigned long long dossier_parent = 0;

		// on écris le pointeur vers la derniere ligne disponible (sur 64 bit), suivis d'un indicateur de version (8 bit) 
		fwrite(&index_last_cursor, sizeof(index_last_cursor), 1, fp);
		fwrite(&version, sizeof(version), 1, fp);
		fwrite(&dossier_parent, sizeof(dossier_parent), 1, fp);
		// maintenant au cas ou.. pour la suite on prévoie.. on met 3 char, 3 unsigned longlong et 3 char 255
		fseek(fp, 3 + 3*8 + 3*255, SEEK_CUR);
		fclose(fp);
		// creation, avec touch de "/" , l'element racine ###		

		fp = fopen(file_stockage, "w+"); // contient le contenu des fichiers
		fseek(fp, 0, SEEK_SET);
		unsigned long long stock_last_cursor = 8;
		fwrite(&stock_last_cursor, sizeof(stock_last_cursor), 1, fp);
		fclose(fp);
	}

	// réinitialisation ?
	set_parent((unsigned long long) 0); // on remet le curseur a 0

	while(!fin_terminal && compteur_while < 100){
		compteur_while++;
		// on demande la commande suivante
		printf("user@machine:pwd$ ");
		commande = inputString(stdin, 10);

		// on verifie si la personne ne veux pas sortir du terminal
		if(!strcmp(commande, "exit")){
			fin_terminal = 1;
		} else { 
			parsing(commande, split_commande, &argc_commande);
			
			/*
			for(i=0; i<argc_commande;i++){
				printf("%s\n", split_commande[i]);
			}
			*/
			
			char concat[256] = PATH_BIN_FILE;//"/home/joachim/system/c/projet/bin/";
			strcat(concat, split_commande[0] );
			//printf("%s\n", concat);
			split_commande[0] = concat;
			

			/*
			for(i=0; i<argc_commande;i++){
				printf("%s\n", split_commande[i]);
			}
			*/

			exec_prog(split_commande);
			int status = 0;
			wait(&status); // on attend la fin des forks
		}
	}

	free(commande); // on libere la place alloué pour la commande
	return EXIT_SUCCESS;
}
