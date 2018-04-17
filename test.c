#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int N, C, D;
double M,alpha;

double X[3][2];
double U[3][2];
double T[3][2];
double W[3][2];
double V[2][2];


double updateV() {

	int i,j,k,t;
	double sigma, sum;
	double P, Q;


	for (j = 0; j < C; j++)	{
		
		sigma = 0.0;

		for (i = 0; i < D; i++)	{
			
			sum = 0.0;
			Q = 0.0;

			for (k = 0; k < N; k++) {
				
				P = pow((U[k][j] * (2 - W[k][j])), M);
				P = P * X[k][i];

				Q += pow((U[k][j]*(2 - W[k][j])), M);

				sum += P;
			}

			V[j][i] = sum / Q;
			printf("Q: %lf\n", Q);
			printf("V[%d][%d]: %lf\t", j, i, V[j][i]);

		}

		printf("\n");

	}
	
}

double calculateDistance(int k, int j) {

	int i;
	double sum = 0.0;

	for (i = 0; i < D; i++) {
		
		sum += pow((X[k][i] - V[j][i]), 2);

	}

	return sqrt(sum);

} 

double updateU() {

	int i,j,k,t;
	double sum;
	double P, Q;


	for (k = 0; k < N; k++) {
		
		for (j = 0; j < C; j++)	{
			
			sum = 0.0;

			for (i = 0; i < C; i++)	{
				
				P = 0.0;

				P = calculateDistance(k,j) / calculateDistance(k,i);
				P = pow(P, (2/(M-1)));
				P = (2 - W[k][j])*P;

				printf("%lf\t", P);

				sum += P;					

			}


			U[k][j] = 1.0 / sum;

			printf("U[%d][%d]: %lf\t", k, j, U[k][j]);


		}

		printf("\n");

	}

}

double updateT() {

	int i,j,k;
	double e_sigma, sigma;

	for (k = 0; k < N; k++) {
		
		for (j = 0; j < C; j++)	{
			
			sigma = 0.0;
			e_sigma = 0.0;

			for (i = 0; i < C; i++) {
				
				sigma += W[k][i];
				e_sigma += exp(-W[k][i]);

			}

			printf("%lf\t", sigma);
			printf("%lf\t", e_sigma);

			T[k][j] = (1.0 - (1.0/C)*sigma) * (exp(-W[k][j])/e_sigma); 
			printf("T[%d][%d]: %lf\t", k,j,T[k][j]);
		}
		printf("\n");
	}

}

double updateW() {

	int j,k;
	double P;

	for (k = 0; k < N; k++) {
		for (j = 0; j < C; j++) {
			P = U[k][j] + T[k][j];
			W[k][j] = 1 - P - pow((1 - pow(P, alpha)), (1/alpha));
			printf("W[%d][%d]: %lf\t", k,j,W[k][j]);
		}
		printf("\n");
	}


}

double SDmax() {

	int i,j,k;
	double max;
	double subtract_V;

	max = 0.0;

	for (i = 0; i < C; i++)	{
		for (j = 0; j < C; j++)	{

			subtract_V = 0.0;

			for (k = 0; k < D; k++)	{
				subtract_V += pow(V[j][k] - V[i][k], 2); 			
			}

			printf("%lf\n", subtract_V);

			if(subtract_V >= max) max = subtract_V;
		}
	}

	return max;

}

double sigmaD() {

	int i,j,k;
	double sum, sigma;

	sigma = 0.0;

	for (j = 0; j < C; j++)	{
		
		sum = 0.0;

		for (i = 0; i < N; i++)	{
			
			sum += calculateDistance(i, j);

		}

		sigma += sum;

	}

	return (sigma / (N * C));

}


double IFV_PFS() {

	int i,j,k;
	double P, Q;
	double sigma_log_U;
	double sigma;
	double result;

	result = 0.0;

	for (j = 0; j < C; j++) {
		
		sigma_log_U = 0.0;

		for (k = 0; k < N; k++)	{
			
			sigma_log_U += log(U[k][j]) / log(2);

		}

		// printf("sigma_log_U: %lf\n", sigma_log_U);

		P = ((log(C) / log(2.0)) - (1.0/N)*sigma_log_U);

		sigma = 0.0;

		for (k = 0; k < N; k++)	{
			
			sigma += pow(U[k][j], 2) * pow(P, 2);

		}

		result += sigma/N;

	}

	result = result / (C);

	result = result * (SDmax() / sigmaD());

	return result;
}

double calculateDBS(int i, int mark[]) {

	int points_in_cluster;
	double sum;
	int k;

	points_in_cluster = 0;

	for (k = 0; k < N; k++)	{
		
		if(mark[k] == i) {
			points_in_cluster++;
		} 

	}

	// printf("P: %d\n", points_in_cluster);

	sum = 0.0;

	for (k = 0; k < N; k++) {
		
		if(mark[k] == i) {

			// printf("%lf\n", calculateDistance(k,i));

			sum += pow(calculateDistance(k,i),2);

		}
	}

	return sqrt(sum/(double)(points_in_cluster));


}

double calculateDBM(int i, int j) {

	int k;
	double sum;

	sum = 0.0;

	for (k = 0; k < D; k++) {
		
		sum += pow(V[i][k] - V[j][k], 2);

	}

	return sqrt(sum);

}

double DB_PFS() {

	int i,j,k;
	int *mark;
	double max, a;
	double A,B,E,D;

	mark = (int*)malloc(N*sizeof(int));

	for (i = 0; i < N; i++)	{
		
		max = U[i][0] * (2 - W[i][0]);

		k = 0;

		for (j = 0; j < C; j++)	{

			a = U[i][j] * (2 - W[i][j]);

			// printf("a: %lf\n", a);

			if(max < a) {
				max = a;
				k = j;
			}
		}

		mark[i] = k;
	}

	// printf("%d\n", mark[0]);
	// printf("%d\n", mark[1]);
	// printf("%d\n", mark[2]);

	A = 0;

	for (i = 0; i < C; i++)	{
		
		max = 0.0;

		B = calculateDBS(i, mark);

		for (j = 0; ((j < C) && (j != i) ); j++) {
			
			if(i != j) {
				E = calculateDBM(i,j);
				D = (B + calculateDBS(j, mark)) / E;
			}

			if(D >= max) {
				max = D;
			}

		}

		A += max;

	}

	return (A/C);

}

double SHE_PFS() {


	
	
}



int main(int argc, char const *argv[])
{

	int i,j;

	X[0][0] = 1.0;
	X[0][1] = 3.0;
	X[1][0] = 5.0;
	X[1][1] = 6.0;
	X[2][0] = 2.0;
	X[2][1] = 4.0;

	U[0][0] = 0.5;
	U[0][1] = 0.75;
	U[1][0] = 0.6;
	U[1][1] = 0.4;
	U[2][0] = 0.4;
	U[2][1] = 0.2;

	W[0][0] = 0.1;
	W[0][1] = 0.2;
	W[1][0] = 0.2;
	W[1][1] = 0.15;
	W[2][0] = 0.2;
	W[2][1] = 0.2;


	N = 3;
	C = 2;
	D = 2;

	M = 2.0;
	alpha = 0.5;
	
	updateV();
	updateU();
	updateT();
	updateW();

	printf("SDmax: %lf\n", SDmax());
	printf("SigmaD: %lf\n", sigmaD());
	printf("IFV: %lf\n", IFV_PFS());
	printf("DB: %lf\n", DB_PFS());

	return 0;
}