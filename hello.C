#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
	int error;
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;
	error = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(error)
	{
		printf("Error code: %d \n", error);
		return error;
	}
	printf("Processor number %d says \"Hello World!\"\n", rank);
	if(rank==0){
		printf("Size of communicator: %d\n", size);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	const int N= 5;
	float a[N];
	if(rank == 0)
	{
		float* read = a;
		printf("Please enter %d variables: \n", N);
		for(int i = 0 ; i < N ; i++)
		{	
			 scanf("%f", read++); 
			 //printf("%f\n", a[i]); 
		}

		for(int i = 1 ; i < size ; i++)
		{	
			MPI_Send(a, N, MPI_INT, i, i, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Recv(a, N, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
		printf("Processor %d received the array with first element equal to: %f\n", rank, a[0]);
	}

	MPI_Finalize();
	return 0;
}
