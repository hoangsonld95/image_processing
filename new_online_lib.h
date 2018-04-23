#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "update.h"
#include "clustering_quality.h"


int endLoop(int *iters, double diff) {

	if((*iters) > maxIters || diff < epsilon) {

		return 1;

	}

	return 0;

}

void initMemForV() {

	int i,j;

	if(V == NULL) 
		V = (double**)malloc(C*sizeof(double*));

	for (i = 0; i < D; i++)	{
		
		V[i] = (double*)malloc(D*sizeof(double));

	}

}

double PFS(int *iters) {

	double diff, diffU, diffT, diffW;
	maxIters = 1000;
	double ifv;
	time_t start, end;

	start = clock();
	

	do {
		updateV();
		diffU = updateU();
		diffT = updateT();
		diffW = updateW();
		diff = diffU + diffT + diffW;
		(*iters)++;

	} while(!endLoop(iters, diff));

	end = clock();

	// printf("iterations: %d\n", iters);
	// printf("ifv: %lf\n", IFV_PFS());
	return ((double)(end - start)/CLOCKS_PER_SEC);
}


double PFS_Optimize(int *iters) {

	double diff, diffU, diffT, diffW;
	maxIters = 1000;
	double ifv;
	time_t start, end;

	start = clock();

	updateV_optimize();
	diffU = updateU();
	diffT = updateT();
	diffW = updateW();
	printf("*********************\n");
	printf("diffU: %10.15lf\n", diffU);
	printf("diffT: %10.15lf\n", diffT);
	printf("diffW: %10.15lf\n", diffW);
	diff = diffU + diffT + diffW;
	printf("*********************\n");

	(*iters)++;

	N = N + P;

	do {
		// updateV();
		diffU = updateU();
		diffT = updateT();
		diffW = updateW();
		printf("diffU: %10.15lf\n", diffU);
		printf("diffT: %10.15lf\n", diffT);
		printf("diffW: %10.15lf\n", diffW);
		diff = diffU + diffT + diffW;
		(*iters)++;
		printf("\n");
	
	} while(!endLoop(iters, diff));

	end = clock();

	printf("/////////////////\n");

	return ((double)(end-start)/CLOCKS_PER_SEC);

}
