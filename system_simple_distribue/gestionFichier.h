// Contenu du fichier commun aux serveurs 
typedef struct _fichier {
    char machine[50];
    char chemin[50];
}f_Fichier;

/** Permet d'ajouter une ligne au fichier commun 
*	Exemple d'utilisation:
*			char chemin[50];
*			sprintf(chemin, "./fichier_du_serv%d ",num);
*			ajouterLigne("Machine 1\0",chemin);
*/
int ajouterLigne(char machine[50], char chemin[50]);

/** Permet de récupérer la ligne i du fichier commun
*/
f_Fichier* getLigne(int i);
 
/** Permet de récupérer toutes les lignes du fichier commun
*	Exemple d'utilisation:
*		f_Fichier *t2 = getAllLignes();
*		for (int i = 0 ; i < getNbLignes(); i++){
*			printf("%d: %s %s \n",i, t2[i].machine, t2[i].chemin);
*		}
*/
f_Fichier* getAllLignes(); 

/** Permet de récupérer le nombre de lignes du fichier commun
*/
int getNbLignes();

/** Permet de supprimer le contenu du fichier commun
*/
void supprimerContenu();


