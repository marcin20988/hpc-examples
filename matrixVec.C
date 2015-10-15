#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

class Matrix
{
private:
	// number of rows (global and local)
	int M_, m_;
	// number of columns (global and local)
	int N_, n_;
	// data
	double* globaldata_;
	double* data_;
	int rank_, size_;

	void rowSplit()
	{
		n_ = N_ / size_;
		m_ = M_;
		data_  = new double[n_ * M_];
		MPI_Scatter(globaldata_, n_ * M_, MPI_DOUBLE, data_, n_ * M_, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	}
public:
	Matrix(int N, int M)
	{
		N_ = N;
		M_ = M;


		MPI_Comm_size(MPI_COMM_WORLD, &size_);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank_);

		if(rank_ == 0)
		{
			globaldata_ = new double[N * M];

			for(int i = 0; i < N_; i++)
			{
				for(int j = 0; j < M_; j++) 
				{
					globaldata_[i * M_ + j] = 1.;
				}
			}

		}
		rowSplit();
		MPI_Barrier(MPI_COMM_WORLD);
	}

	double operator()(int i, int j) const
	{

		double a;
		if(rank_ == 0)
		{
			a = globaldata_[i * M_ + j];
		}
		MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		return a;	
	}
	
	double& operator()(int i, int j)
	{

		double* a = new double[1];
		if(rank_ == 0)
		{
			a = &globaldata_[i * M_ + j];
		}
		MPI_Bcast(a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		return *a;	
	}
	
	~Matrix()
	{
		if(rank_ == 0)
		{
			delete globaldata_;
		}
	}

	void printLocalData() const
	{
		for(int i = 0; i < n_; ++i)
		{
			for(int j = 0; j < m_; j++)
			{
				printf("%f \t", data_[i * m_ + j]);
			}
			printf("\n");
		}
	}

	double* multiply(double * v)
	{
		double* local_result = new double[n_];
		
		for(int i = 0; i < n_; i++)
		{
			local_result[i] =  0.0;
		}
		
		double *p = data_;
		for(int i = 0; i < n_; i++)
		{
			for(int j = 0; j < m_; j++)
			{
				local_result[i] += (*p++) * v[j];
			}
		}
		
		MPI_Barrier(MPI_COMM_WORLD);
		double* result;
		if(rank_ == 0) result = new double[N_];
		MPI_Gather(local_result, n_, MPI_DOUBLE, result, n_, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		return result;
	}
};


int main(int argc, char** argv)
{
	
	MPI_Init(&argc, &argv);

	int N = atoi(argv[1]);
	int M = atoi(argv[2]);

	Matrix A(N, M);
	double *x = new double[M];
	for(int i = 0; i < M; ++i) x[i] = 1.;

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        //A(2, 3) = 15;
	//printf("A(2,3)is %.1f \n", A(2, 3));


	double *result;
	double t1 = MPI_Wtime();
	result = A.multiply(x);
	double t2 = MPI_Wtime();
	if(rank == 0)
	{
		double sum = 0.0;
		for(int i=0; i<N; i++) sum += result[i];
		double exp = N * M;
		printf("Sum is %.1f (expected %.1f)\n", sum, exp);
		printf("time elapsed: %f\n", t2 - t1);
	}


	MPI_Finalize();

	return 0;
}
