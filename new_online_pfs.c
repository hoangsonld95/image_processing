#include "new_online_lib.h"


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

double takeRandomU(double Q) {

	double start;
	double end;
	double randVal;

	start = Q/2.0;
	end = (-1.0 + sqrt(1.0 + 4.0*Q)) / 2.0;

	randVal = start + ((double)rand() / (double)RAND_MAX) * (end-start);

	return randVal;

}

double takeRandomW(double u, double Q) {

	double w;

	w = 2.0 - (Q / u);

	return w;

}

double initMembership() {

	int i,j,k,p;
	double sum, Q, sumT, A;
	double temp;

	U = (double**)realloc(U, (N+P)*sizeof(double*));
	T = (double**)realloc(T, (N+P)*sizeof(double*));
	W = (double**)realloc(W, (N+P)*sizeof(double*));

	printf("%d\n", N);
	printf("%d\n", P);

	for (k = N; k < (N+P); k++) {
		
		U[k] = (double*)malloc(C*sizeof(double));
		T[k] = (double*)malloc(C*sizeof(double));
		W[k] = (double*)malloc(C*sizeof(double));

		sum = 0.0;

		for (j = 0; j < C; j++)	{
			
			sum += calculateDistance(k,j);

		}

		printf("%lf\n", sum);

		for (j = 0; j < C; j++)	{
			
			temp = calculateDistance(k, j);
			Q = temp/sum;

			printf("%lf\n", Q);

			U[k][j] = takeRandomU(Q);

			printf("U[%d][%d]: %lf\t", k, j, U[k][j]);

			W[k][j] = takeRandomW(U[k][j], Q);

			printf("W[%d][%d]: %lf\t", k, j, W[k][j]);

			printf("\n");
							
		}

		A = 0.0;

		for (j = 0; j < C; j++)	{
			
			A += W[k][j]/C;

		}

		sumT = 0.0;

		for (j = 0; j < C; j++) {
			
			T[k][j] = (double)rand();
			sumT += T[k][j];

		}

		for (j = 0; j < C; j++)	{
			
			T[k][j] = (T[k][j]/sumT) * A;

		}


	}

	return 0.0;

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

	double ifv, entropy;
	double db;
	double silhouette;
	double exec_time;

	strcpy(fileName, argv[1]);

	input(fileName);

	strcpy(fileBatch, "batch_source.txt");
	f_batch = fopen(fileBatch, "r");

	strcpy(fileOutput, "new_online_clustering_result.txt");

	FEED_TIME = 5;
	P = 5;

	batch = 0;

	initMemForV();

	PFS(&iters);
	printf("IFV1: %lf\n", IFV_PFS());
	
	
	do {
		iters = 0;
		exec_time = 0;
		// sleep(5);
		exec_time += feedData(f_batch);
		exec_time = initMembership();
		N = N + P;
		exec_time += PFS(&iters);
		ifv = IFV_PFS();
		db = DB_PFS();
		entropy = calculateEntropy_old();
		// silhouette = SHE_PFS();

		printf("iters: %d\n", iters);
		printf("ifv: %lf\n", ifv);
		printf("entropy: %lf\n", entropy);
		// printf("db: %lf\n", db);
		output(fileOutput, batch, exec_time, iters, ifv, db, entropy);
		batch++;
	}while(!feof(f_batch));
	

	return 0;
}