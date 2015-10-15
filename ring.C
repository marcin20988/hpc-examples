#include "mpi.h"
#include <stdio.h>
int main(int argc, char **argv)
{
	int i, size, rank;
	int tag1, tag2, count, ger;
	int prev, next;
	int tag[] = {0,1,2,3};
	double time1[8],time2[8];
	char *buffer;
	MPI_Status status;
	MPI_Request reqSend;
	MPI_Request reqRecv;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//printf("Hello world! This is processor %d out of %d\n", rank, size);
	if (rank >= size-1) next = 0; else next = rank+1;
	if (rank == 0) prev = size-1; else prev = rank-1;
	time1[rank] = MPI_Wtime();
	/*
	if(rank == 0)
	{
	time1 = MPI_Wtime();
	//MPI_Ssend(buffer, count, MPI_DOUBLE, 1, );
	}
	*/
	count = 1;
	MPI_Isend(&rank, count, MPI_INT, next,rank ,MPI_COMM_WORLD, &reqSend);
	//printf("processor %d the value of next is %d\n", rank, next);
	MPI_Irecv(&prev, count, MPI_INT, prev,prev, MPI_COMM_WORLD, &reqRecv);
	MPI_Wait(&reqRecv, &status);
	printf("processor %d the value of prev is %d\n", rank, prev);
	/*78
	if (rank == 0)
	{
	time2 = MPI_Wtime();
	printf("Time elapsed %lf\n", time2-time1);
	}
	*/
	time2[rank] = MPI_Wtime();
	//printf("Time elapsed for processor %d: %lf\n", rank, time2[rank]-time1[rank]);
	MPI_Finalize();
	return 0;
}
