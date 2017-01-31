#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // fonction access
#include <ctype.h> // function isprint
#include <sys/wait.h>
#include <dirent.h> 
#include <sys/types.h>
#include <sys/stat.h>

#include "constante.h"
#include "functions_filesystem.h"
#include "get_env.h"


void trim(char *str){
	int i;
	int begin = 0;
	int end = strlen(str) - 1;

	while (isspace((unsigned char) str[begin]))
		begin++;

	while ((end >= begin) && isspace((unsigned char) str[end]))
		end--;

	// Shift all characters back to the start of the string array.
	for (i = begin; i <= end; i++)
		str[i - begin] = str[i];

	str[i - begin] = '\0'; // Null terminate string.
}

/**
 * Fonction qui permet d'executer une une commande comme "touch","ls","cd" etc
 */
int exec_prog (char ** argv){
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

/**
 * Fonction qui parse une commande entré par l'utilisateur pour en extraire les différentes parties : commande, option, argument
 */
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
                                        trim(buffer[compteur]);
										if(strcmp(buffer[compteur], "") != 0){// si non vide
											r[compteur] = buffer[compteur]; 
											last_cursor = i + 1;
											compteur++;
										}
                                }
                        } else {
                                // printf("%c", s[i]);
                        }
                }
        }

        buffer[compteur] = malloc(256 * sizeof(char));
        memcpy( buffer[compteur], &s[last_cursor], i - last_cursor);
        buffer[compteur][i - last_cursor] = '\0';
        trim(buffer[compteur]);
        if(strcmp(buffer[compteur], "") != 0){// si non vide
        	r[compteur] = buffer[compteur]; 
			compteur++;
		}

        r[compteur] = NULL;
        *c = compteur;
}


int print_y_n(char * s){
        /**
         *      Function demandant une question fermé. oui ou non.
         *      return true ou false
         *
         *      Exemple :
         *      if(print_y_n("lol ?")){
         *              printf("oui");
         *      }
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

char * inputString(FILE* fp, size_t size){
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

/**
 * Fonction qui permet de supprimer un fichier si il existe déjà
 * @param fileName - le nom du fichier
 */
void if_file_isset_then_delete(char * fileName){
	char buffer_string[1024]; // formations des phrases avec snprintf
	if( access( fileName, F_OK ) != -1 ) { // si il existe // ### faire sous forme de fonction
			// en gros snprintf permet de placer des variables dans une chaine de la même façon que printf, sauf que ça ne l'affiche pas ça forme la chaine
			snprintf(buffer_string, sizeof(buffer_string), "fichier : %s existe deja. le supprimer ?", fileName);
			if(print_y_n(buffer_string)){ // on supprime le fichier si oui
					if(remove(fileName) == 0){
							printf("fichier %s a ete supprimer.\n", fileName);
					} else {
							printf("error : fichier %s non supprimer.\n", fileName);
					}
			}
	}
}
/**
 * Fonction qui permet de supprimer un dossier si il existe déjà
 * @param nomDir - le nom du dossier
 */
void if_dir_isset_then_delete(char * dirName){
	char buffer_string[1024]; // formations des phrases avec snprintf
	if( access( dirName, F_OK ) != -1 ) { // si il existe // ### faire sous forme de fonction
			// en gros snprintf permet de placer des variables dans une chaine de la même façon que printf, sauf que ça ne l'affiche pas ça forme la chaine
			snprintf(buffer_string, sizeof(buffer_string), "Dossier : %s existe deja. le supprimer ?", dirName);
			if(print_y_n(buffer_string)){ // on supprime le fichier si oui
					if(remove_directory(dirName) == 0){
							printf("Dossier %s a ete supprimer.\n", dirName);
					} else {
							printf("error : Dossier %s non supprimer.\n", dirName);
					}
			}
	}
}

/**
 * Fonction qui supprimer un dossier du serveur ainsi que tout les fichiers qui sont contenu dedans
 */
int remove_directory(char * dirName){
	struct dirent *lecture;
	DIR *rep;
	rep = opendir( dirName );
	while ((lecture = readdir(rep))) {
		remove(lecture->d_name);
	}
	closedir(rep); 
	int r = rmdir(dirName);
	return r;
}

/**
 * Fonction qui initialise les deux fichiers servant à la base de données : index et bdd
 */
void init_bdd(){

        char version = 1; // version a 1 //  ### changer en fonction d'une option
        unsigned long long dossier_parent = 0;

        // on calcul le pointeur a partir duquel on peux écrire ( en nombre d'octet)
        unsigned long long index_last_cursor= 8 + 1 + 8 + 3 + 3*8 + 3*255; // long long pour forcer a 8 octet, de base sur linux un simple long aurait suffit..

        // Création du fichier d'index
		if_file_isset_then_delete( FILE_NAME_INDEX );
		
		// on créer les fichiers
        FILE *fp = fopen(FILE_NAME_INDEX, "w+"); // contient la liste des fichiers et leur emplacement dans le fichier stockage
        fseek(fp, 0, SEEK_SET); // on remet le curseur au début du fichier

        // on écris le pointeur vers la derniere ligne disponible (sur 64 bit), suivis d'un indicateur de version (8 bit)
        fwrite(&index_last_cursor, sizeof(index_last_cursor), 1, fp);
        fwrite(&version, sizeof(version), 1, fp);
        fwrite(&dossier_parent, sizeof(dossier_parent), 1, fp);
        // maintenant au cas ou.. pour la suite on prévoie.. on met 3 char, 3 unsigned longlong et 3 char 255
        fseek(fp, 3 + 3*8 + 3*255, SEEK_CUR);
        fclose(fp);
        // creation, avec touch de "/" , l'element racine ###
		
        // Création du fichier BDD
        if_file_isset_then_delete( FILE_NAME_STOCKAGE );
		
		fp = fopen(FILE_NAME_STOCKAGE, "w+"); // contient le contenu des fichiers
        fseek(fp, 0, SEEK_SET);
        unsigned long long stock_last_cursor = 8;
        fwrite(&stock_last_cursor, sizeof(stock_last_cursor), 1, fp);
        fclose(fp);

        // Création du dossier SWP
        if_dir_isset_then_delete( PATH_FOLDER_SWAP );
		mkdir( PATH_FOLDER_SWAP , S_IRWXU );

}

