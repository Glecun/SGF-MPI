#include <mpi.h>

/** Commande Touch
*/
char* cmd_touch(char* cmd,int num, char nameMach[MPI_MAX_PROCESSOR_NAME]);

/** Commande Show data
* Permet de voir le contenu de data.bd
*/
char* cmd_showdata(char* cmd);