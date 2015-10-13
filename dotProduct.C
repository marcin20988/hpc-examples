#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
	const int N = 50000000;
	double* a = new double[N];
	double* b = new double[N];
	double t1, t2;

	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	t1 = MPI_Wtime();
	// length of the vector passed to each processor
	// in case of N not divisible by n calculate 
	const int n = N / size;
	const int last_n = N - n * (size - 1);
	const int localn = (rank == size -1) ? last_n : n;

	const int start = n * rank;
	double localdot=0.0;

	for(int i = start; i < start + localn; i++)
	{
		a[i] = 1.0;
		b[i] = 1.0;
	}
	for(int i = start; i < start + localn; i++)
	{
		localdot += a[i] * b[i];
	}
	/*double* aptr = &a[start];
	double* bptr = &b[start];
	for(int i = 0; i < localn; i++)
	{
		localdot += (*(aptr++)) * (*(bptr++));
	}*/

	printf("%d \t %f\n", localn, localdot);

	double result;
	for(int i=0; i<size; i++)
	{
		MPI_Reduce(&localdot, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	t2 = MPI_Wtime();
	if(rank == 0) 
	{
		printf("The result is: %f\nwall-time: \n%f\n", result, t2 - t1);
	}
	MPI_Finalize();

	delete a;
	delete b;
	return 0;
}
