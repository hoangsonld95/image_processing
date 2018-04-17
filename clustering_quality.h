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

			// printf("%lf\n", subtract_V);

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