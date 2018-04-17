#include "online_lib.h"


double feedData(FILE *f_batch) {

	int i,j;
	clock_t start, end;

	start = clock();

	X = (double**)realloc(X, (N+P)*sizeof(double*));

	for (i = N; i < (N + P); i++) {
		X[i] = (double*)malloc(D*sizeof(double));		
		for (j = 0; j < D; j++)	{
			fscanf(f_batch, "%lf\t", &X[i][j]);
			printf("%lf\t", X[i][j]);
		}
		fscanf(f_batch, "\n");
		printf("\n");
	}

	// Normalize feed data 
	for (i = N; i < (N+P); i++)	{
		for (j = 0; j < D; j++)	{
			X[i][j] = (X[i][j] - minX) / (maxX - minX);
		}
	}

	end = clock();

	return ((double)(end - start)/ CLOCKS_PER_SEC);


}

double randomizeMembership() {

	int i,j,k;
	double A, sum;
	time_t start, end;


	start = clock();

	// Randomize U 

	U = (double**)realloc(U, (N+P)*sizeof(double*));
	T = (double**)realloc(T, (N+P)*sizeof(double*));
	W = (double**)realloc(W, (N+P)*sizeof(double*));

	for (i = N; i < (N+P); i++)	{
		
		U[i] = (double*)malloc(C*sizeof(double));
		T[i] = (double*)malloc(C*sizeof(double));
		W[i] = (double*)malloc(C*sizeof(double));

		A = (double) rand() / (double) RAND_MAX;
		sum = 0.0;

		for (j = 0; j < C; j++)	{
			
			U[i][j] = (double)rand();
			sum += U[i][j];

			T[i][j] = (double)rand();
			sum += T[i][j];

			W[i][j] = (double)rand();
			sum += W[i][j];

			U[i][j] = (U[i][j] / sum) * A;
			T[i][j] = (T[i][j] / sum) * A;
			W[i][j] = (W[i][j] / sum) * A;

		}

	}

	end = clock();

	return ((double)(end-start)/CLOCKS_PER_SEC);

}


int main(int argc, char const *argv[])
{
	char fileName[100];
	char fileOutput[100];
	char fileBatch[100];

	FILE *f_batch;
	FILE *f_output;

	int batch;
	int iters;

	double ifv;
	double db;
	double exec_time;

	strcpy(fileName, argv[1]);

	input(fileName);

	strcpy(fileBatch, "batch_source.txt");
	f_batch = fopen(fileBatch, "r");

	strcpy(fileOutput, "online_clustering_result.txt");

	FEED_TIME = 5;
	P = 5;

	batch = 0;

	do {
		iters = 0;
		exec_time = 0;
		sleep(FEED_TIME);
		exec_time += feedData(f_batch);
		exec_time += randomizeMembership();
		N = N + P;
		exec_time += PFS(&iters);
		ifv = IFV_PFS();
		db = DB_PFS();
		printf("iters: %d\n", iters);
		printf("ifv: %lf\n", ifv);
		printf("db: %lf\n", db);
		output(fileOutput, batch, exec_time, iters, ifv, db);
		batch++;
	}while(!feof(f_batch));


	

	return 0;
}