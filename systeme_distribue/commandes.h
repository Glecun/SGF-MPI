#include <mpi.h>

/** Commande Touch (et tout autre commande d'ajout plus généralement)
*/
void cmd_mkdir(char ** s_c);

void cmd_del(char ** s_c);

void cmd_deldir(char ** s_c);

void cmd_cd(char ** s_c);

void cmd_pwd(char ** s_c);

void cmd_rename(char ** s_c);

void cmd_vim(char ** s_c);

void cmd_ls(char ** s_c);
//char* cmd_touch(char* cmd, char nameMach[MPI_MAX_PROCESSOR_NAME]);

void cmd_touch(char ** s_c);

void cmd_help();

/** Commande Rm (et tout autre commande de suppression plus généralement)
*/
//char* cmd_rm(char* cmd);

/** Commande Show data
* Permet de voir le contenu de data.bd
*/
char* cmd_showdata(char* cmd);
