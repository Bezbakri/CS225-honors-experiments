#include <climits>
#include <iostream>
#include <omp.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>

// #define RANDTOP INT_MAX
#define RANDTOP 10

inline int loc(int i, int j, int n)
{
	return ( (n * i) + j);
}

//Print a n x n matrix
void printMatrix(double *a, int n)
{
	for(int i = 0; i < n ; i++)
	{
		for(int j = 0; j < n; j++)
			printf("%f\t",a[loc(i,j,n)]);
		printf("\n");
	}		
}	

//Print a n long vector
void printVector(double *a, int n)
{
	printf("(");
	for(int i = 0; i < n; i++)
		printf("%f ",a[i]);
	printf(")\n");
}


// Parallel
void matmul(double *A, double *B, double *C, int n)
{
	#pragma omp parallel for
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			C[loc(i,j,n)] = 0.0;
	
	#pragma omp parallel for
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			for(int k = 0; k < n; k++)
				C[loc(i,j,n)]+= A[loc(i,k,n)] * B[loc(k,j,n)];
				
}

// Normal
void mulMat(double* mat1, double* mat2, double* mat3, int n) 
{ 
  
  
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < n; j++) { 
            mat3[loc(i, j, n)] = 0; 
  
            for (int k = 0; k < n; k++) { 
                mat3[loc(i, j, n)] += mat1[loc(i, k, n)] * mat2[loc(k, j, n)]; 
            }   
        } 
    } 
} 

// Normal
void prefix(double *a, double *b, int n)
{
		for(int i = 0; i < n; i++)
		{
			if(i == 0)
				b[i] = a[i];
			else
				b[i] = b[i-1] + a[i];
		}
}

// Parallel, https://github.com/robfarr/openmp-prefix-sum/blob/master/main.c
void prefixParallel(double* a, double* b, int n) {
    memcpy(b, a, sizeof(double)*n);

    // 1 apart, 2 apart, 4 apart ...
    for (int apart=1; apart<=n; apart*=2) {

        // Create a copy so that we can safely do the sum in parallel by
        // removing the intra-array dependency
        double read[n];
        memcpy(read, b, sizeof(int)*n);

        // Compute the sum of pairs of values
        #pragma omp parallel for
        for (int i=apart; i<=n; i++) {
            b[i] += read[i-apart];
        }

    }

}

int main(int argc, char *argv[]) {

    int n;
	double *A;
	double *B;
	double *C;
	double *a;
	double *b;
	double *c;
	double result;
	int resultloc;


	std::cout << "Running with OMP max threads = " << omp_get_max_threads() << std::endl;

	if(argc == 1){
		n = 4;
	
		A =(double *) malloc(sizeof(double)*n*n);
		B =(double *) malloc(sizeof(double)*n*n);
		C = (double *) malloc(sizeof(double)*n*n);

		for(int i=0; i < n; i++)
			for(int j=0; j < n; j++)
			{
				A[loc(i,j,n)] = i + j;
				B[loc(i,j,n)] = (i == j) ? 1 : 0;
				C[loc(i,j,n)] = 9;
			}

		printf("A = \n");
		printMatrix(A, n);
		printf("B = \n");
		printMatrix(B, n);
		printf("C = \n");
		printMatrix(C, n);
		printf("A * B = \n");
		matmul(A, B, C, n);
		printMatrix(C, n);

	} else if(argc == 2) {
		n = 10;
		int testnumber = atoi(argv[1]);
		A = (double*) malloc(sizeof(double)*n*n);
		B = (double*) malloc(sizeof(double)*n*n);
		C = (double*) malloc(sizeof(double)*n*n);
		a = (double*) malloc(sizeof(double)*n);
		b = (double*) malloc(sizeof(double)*n);
		c = (double*) malloc(sizeof(double)*n);
		
		std::random_device rd;  // a seed source for the random number engine
		std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> randint(1, RANDTOP);


		for(int i=0; i < n; i++)
		{
			for(int j=0; j < n; j++)
			{
				A[loc(i,j,n)] = B[loc(i,i,n)] = i + 2 * j;
				C[loc(i,j,n)] = 9;
			}
			a[i] = randint(gen);
			b[i] = randint(gen);
			c[i] = randint(gen);
		}

		// test here
		prefixParallel(a, b, n);
		printVector(a, n);
		printVector(b, n);

	} else {
		std::cout << "This only takes the n dimension as an argument" << std::endl;
		return 1;
	}

	return 0;
}