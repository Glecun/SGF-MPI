#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main( int argc, char** argv ){
	int size,rank;
	MPI_Init(&argc,&argv);
	
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	
	printf("coucou test 1\n");
	
	MPI_Finalize();
	return EXIT_SUCCESS;
}