#include "read_file.h"

double calculateDistance(int k, int j) {

	int i;
	double sum = 0.0;

	for (i = 0; i < D; i++) {
		
		sum += pow((X[k][i] - V[j][i]), 2);

	}

	return sqrt(sum);

} 

double updateV() {
	int i,j,k,t;
	double sigma;
	double P, Q, sum;

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
		}
	}
}

double updateV_optimize() {

	int i,j,k;
	double sum, Q, R;

	for (j = 0; j < C; j++)	{

		for (i = 0; i < D; i++)	{
			
			sum = 0.0;

			for (k = 0; k < (N+P); k++)	{				
				sum += pow((U[k][j]*(2 - W[k][j])), M);
			}

			Q = 0.0;

			for (k = N; k < (N+P); k++)	{
				
				R = pow((U[k][j]*(2 - W[k][j])), M);
				R = R * X[k][i];

				Q += R; 

			}

			V[j][i] = V[j][i] + Q/sum;

		}

	}

}

double updateU() {

	int i,j,k;
	double R, sum;
	double U_old, diffU;

	diffU = 0.0;

	for (k = 0; k < N; k++) {
		
		for (j = 0; j < C; j++)	{
			
			sum = 0.0;

			for (i = 0; i < C; i++)	{
				
				R = 0.0;

				R = calculateDistance(k,j) / calculateDistance(k,i);
				R = pow(R, (2/(M-1)));
				R = (2 - W[k][j])*R;

				sum += R;					

			}

			U_old = U[k][j];
			U[k][j] = 1.0 / sum;
			diffU += pow((U[k][j] - U_old), 2);

		}

	}

	return sqrt(diffU);

}

double updateT() {

	int i,j,k;
	double e_sigma, sigma;
	double T_old;
	double diffT;

	diffT = 0.0;

	for (k = 0; k < N; k++) {
		
		for (j = 0; j < C; j++)	{
			
			sigma = 0.0;
			e_sigma = 0.0;

			for (i = 0; i < C; i++) {
				
				sigma += W[k][i];
				e_sigma += exp(-W[k][i]);

			}


			T_old = T[k][j];
			T[k][j] = (1.0 - (1.0/C)*sigma) * (exp(-W[k][j])/e_sigma); 
			diffT += pow((T[k][j] - T_old), 2);
		}
	}

	return sqrt(diffT);

}

double updateW() {

	int j,k;
	double P;
	double W_old;
	double diffW;

	diffW = 0.0;

	for (k = 0; k < N; k++) {
		for (j = 0; j < C; j++) {
			P = U[k][j] + T[k][j];
			W_old = W[k][j];
			W[k][j] = 1 - P - pow((1 - pow(P, alpha)), (1/alpha));
			diffW += pow((W[k][j] - W_old), 2); 
		}
	}

	return sqrt(diffW);

}