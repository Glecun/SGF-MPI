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

int main (int argc, char ** argv){
	system("vim a");
	/*MPI_Init (&argc, &argv);
	MPI_Finalize();*/
	return EXIT_SUCCESS;
}
