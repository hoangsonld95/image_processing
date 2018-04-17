int N, D, C, P;
int maxIters;
int batch_data;
int FEED_TIME;

double M, epsilon, alpha;
double **U, **T, **W, **V, **X;

double minX, maxX;

void readX(FILE *f) {

	int i,j;
	char temp[100];

	if(X == NULL) 
		X = (double**)malloc(N*sizeof(double*));

	for (i = 0; i < N; i++) {
		
		X[i] = (double*)malloc(D*sizeof(double));

		for (j = 0; j < D; j++) {
			
			fscanf(f, "%lf ", &X[i][j]);

		}

	}

}

void normalizeX() {

	int i,j;

	for (i = 0; i < N; i++) {
		
		for (j = 0; j < D; j++)	{
			
			X[i][j] = (X[i][j] - minX) / (maxX - minX);

		}


	}

}

void readU(FILE *f) {

	int i,j;

	if(U == NULL) 
		U = (double**)malloc(N*sizeof(double*));

	for (i = 0; i < N; i++) {
		
		U[i] = (double*)malloc(C*sizeof(double));

		for (j = 0; j < C; j++)	{
			
			fscanf(f, "%lf\t", &U[i][j]);

		}


	}
}

void readT(FILE *f) {

	int i,j;

	if(T == NULL) 
		T = (double**)malloc(N*sizeof(double*));

	for (i = 0; i < N; i++) {
		
		T[i] = (double*)malloc(C*sizeof(double));

		for (j = 0; j < C; j++)	{
			
			fscanf(f, "%lf\t", &T[i][j]);

		}

	}

}

void readW(FILE *f) {

	int i,j;

	if(W == NULL) 
		W = (double**)malloc(N*sizeof(double*));

	for (i = 0; i < N; i++) {
		
		W[i] = (double*)malloc(C*sizeof(double));

		for (j = 0; j < C; j++)	{
			
			fscanf(f, "%lf\t", &W[i][j]);

		}

	}

}

void input(char fileName[]) {

	FILE *f;
	char temp[100];

	minX = 1000.0;
	maxX = 30000.0;

	f = fopen(fileName, "r");

	if(!f) {
		printf("file cannot be opened\n");
		return;
	}

	fscanf(f, "N=%d\n", &N);
	fscanf(f, "R=%d\n", &D);
	fscanf(f, "C=%d\n", &C);
	fscanf(f, "m=%lf\n", &M);
	fscanf(f, "anpha=%lf\n", &alpha);
	fscanf(f, "eps=%lf\n", &epsilon);
	fscanf(f, "%s\n", temp);

	printf("%d %d %d %lf %lf %.12lf", N, D, C, M, alpha, epsilon);
	printf("\n");

	// Read X 
	readX(f);

	// Normalize X to range [0,1]
	normalizeX();

	printf("****************\n");

	readU(f);

	printf("****************\n");

	readT(f);

	printf("****************\n");

	readW(f);

	fclose(f);

}

void output(char fileOutput[], int batch, double exec_time, int iters, double ifv, double db) {

	FILE *f_output;

	f_output = fopen(fileOutput, "a");

	if(f_output == NULL) printf("aaaa\n");

	fprintf(f_output, "-------------\n");
	fprintf(f_output, "Batch: %d\n", batch);
	fprintf(f_output, "Execution time: %.10lf\n", exec_time);
	fprintf(f_output, "Iters: %d\n", iters);
	fprintf(f_output, "IFV: %.15lf\n", ifv);
	fprintf(f_output, "DB: %.15lf\n", db);

	fclose(f_output);

}