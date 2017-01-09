#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>

// Contenu du fichier commun aux serveurs 
typedef struct _fichier
{
    char machine[50];
    char chemin[50];
}f_Fichier;

// Permet d'ajouter une ligne au fichier commun 
int addLine(char machine[50], char chemin[50]);
// Permet de récupérer la ligne i du fichier commun 
int getLine(int i);

int main (int argc, char ** argv){
	//Variables
	int num;		// numéro du thread
	int cmd = 0;	// **temporaire**: variable qui détermine la nature de la commande: 0 -> creation fichier, 1-> suppression fichier
	int tag = 0;
	int size;			//nombre de threads
	MPI_Request request;
	MPI_Status status;
	
	// Test des foncions du fichier commun
	/*
	char machine[50];
    char chemin[50];
	strcpy(machine,"F205\0");
	strcpy(chemin,"./test\0");
	addLine(machine,chemin);
	addLine(machine,chemin);
	getLine(1);
	*/
	
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
		if (cmd==0){				// Creation
			printf ("Serveur %d: je crée un fichier\n", num);
		}
		else if (cmd==1){		// Suppression
			printf ("Serveur %d: je supprime un fichier\n", num);
		}
	}
	
	MPI_Finalize ();
	
	return 0;
}


 //TODO: ajouter à la suite pour ne pas ecraser (regarder code joachim)
int addLine(char newMachine[50], char newChemin[50]) {
    FILE * f = fopen("data.bd", "wb+"); // ouvre le fichier en mode lecture/écriture binaire
	if (!f) {
		perror("addLine: fopen failed");
		exit(1);
	}
	
    f_Fichier t1;
	strcpy( t1.machine, newMachine );
	strcpy( t1.chemin, newChemin );
 
	printf("test1 %s \n", t1.machine);
	printf("test2 %s \n", t1.chemin);
	
    if(fwrite(&t1, sizeof(f_Fichier), 1, f) != 1) // écris dans le fichier avec fwrite
	{
		perror("Erreur lors de l'ecriture des donnees dans le fichier!!");
		fclose(f);
		return -1;
    }
		    
	fclose(f);

	return 0;
}		 

int getLine(int i) { 
	FILE * f = fopen("data.bd", "r"); // ouvre le fichier en mode lecture/écriture binaire
	if (!f) {
		perror("getLine: fopen failed");
		exit(1);
	}

	fseek(f, i*sizeof(f_Fichier), SEEK_SET); // remets le curseur du fichier au début
	
	f_Fichier t2;

	if(fread(&t2, sizeof(f_Fichier), 1, f) != 1) // lis à partir du fichier
	{
		perror("Erreur lors de la lecture des donnees dans le fichier!!");
	}
	else
	{
		printf("%s %s \n", t2.machine, t2.chemin); // affiche le résultat
	}

	fclose(f);
    return 0;
}
