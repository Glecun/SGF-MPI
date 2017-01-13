#include <mpi.h>

/** Commande Touch (et tout autre commande d'ajout plus généralement)
*/
char* cmd_touch(char* cmd, char nameMach[MPI_MAX_PROCESSOR_NAME]);

/** Commande Rm (et tout autre commande de suppression plus généralement)
*/
char* cmd_rm(char* cmd);

/** Commande Show data
* Permet de voir le contenu de data.bd
*/
char* cmd_showdata(char* cmd);