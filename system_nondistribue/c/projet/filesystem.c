#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // fonction access
#include <ctype.h> // function isprint

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
	char buffer_string[1024]; // formations des phrases avec snprintf
	
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
			snprintf(buffer_string, sizeof(buffer_string), "Fichier : %s existe deja. Le supprimer ?", file_index);
			if(print_y_n(buffer_string)){ // on supprime le fichier si oui
				if(remove(file_index) == 0){
					printf("Fichier %s a ete supprimer.\n", file_index);
				} else {
					printf("Error : Fichier %s non supprimer", file_index);
				}
			}
		}
		// réiterer pour file_stockage
		
		/*
			snprintf(buffer_string, sizeof(buffer_string), "Fichier : %s existe deja. Le supprimer ?", file_stockage);
			print_y_n(buffer_string);
		*/

		// on crée les fichiers
		FILE *fp = fopen(file_index, "w+"); // contient la liste des fichiers et leur emplacement dans le fichier stockage
		fseek(fp, 0, SEEK_SET); // on remet le curseur au début du fichier
		// on calcul le pointeur a partir duquel on peux écrire ( en nombre d'octet)
		unsigned long long index_last_cursor= 8 + 1; // long long pour forcer a 8 octet, de base sur linux un simple long aurait suffit..
		char version = 1; // version a 1 //  ### changer en fonction d'une option
		// on écris le pointeur vers la derniere ligne disponible (sur 64 bit), suivis d'un indicateur de version (8 bit) 
		fwrite(&index_last_cursor, sizeof(index_last_cursor), 1, fp);
		fwrite(&version, sizeof(version), 1, fp);
		/* 	code pour trouver le premier pointeur
		fseek(fp, 0, SEEK_SET);
		unsigned long long aze;
		fread(&aze, sizeof(aze), 1, fp);
		printf("ici :: %llu \n", aze);
		*/
		fclose(fp);
		// creation, avec touch de "/" , l'element racine ###		

		fp = fopen(file_stockage, "w+"); // contient le contenu des fichiers
		fclose(fp);
	}

	while(!fin_terminal){
		// on demande la commande suivante
		printf("user@machine:pwd$ ");
		commande = inputString(stdin, 10);
		
		// on verifie si la personne ne veux pas sortir du terminal
		if(!strcmp(commande, "exit")){
			fin_terminal = 1;
		} else if(!strcmp(commande, "touch")){
			system("./bin/touch");
		}
	}

	free(commande); // on libere la place alloué
	return EXIT_SUCCESS;
}
