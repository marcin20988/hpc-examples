#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("Processor number %d says \"Hello World!\"\n", rank);
	if(rank==0){
		printf("Size of communicator: %d\n", size);
	}
	//printf("%f\n", (float)rank/8.0);
	MPI_Finalize();
	return 0;
}
