/*
	
	1. Giá trị 1 chiều được bound bởi ma trận 2 chiều low_high --> để lưu low và high values cho mỗi chiều
	2. Mảng 2 chiều degree_of_membership --> để lưu lại "degree of membership" mà 1 điểm dữ liệu thuộc về 1 cluster

		Tổng "degree of membership" của 1 điểm dữ liệu trên tất cả các cluster = 1

	epsilon --> measure of accuracy --> determines the num of iterations before the program terminates 
	fuzziness : m --> tolerance in clustering --> affects clustering accuracy (1 < m < maxsize+)

	init() --> truyển dl từ data file vào 

		1.	Number of data points, clusters, dimensions ??? 
		2.	Fuzziness coefficient ??? 
		3.	Required accuracy ???
		4.	Khởi tạo điểm DL 
		5. 	Khởi tạo ma trận degree of membership --> dùng hàm rand() 
	

	Khởi tạo ma trận membership --> (???) constraints 

	Tính degree of membership của ma trận 
	Tính toán các vector trung tâm --> Tính epsilon (điều kiện dừng)

	Tính norm --> độ "gần" của 1 phần tử với tâm cụm 

	Update lại degree of membershíp	

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define MAX_DATA_POINTS 10000 
#define MAX_CLUSTERS	100

#define MAX_DATA_DIMENSIONS 5



int number_of_data_points;
int number_of_clusters;
int number_of_dimensions;
double fuzziness_coefficient;
double epsilon;

double minValue_maxValue[MAX_DATA_DIMENSIONS][2];
double data_points[MAX_DATA_POINTS][MAX_DATA_DIMENSIONS];
double degree_of_membership[MAX_DATA_POINTS][MAX_CLUSTERS];
double cluster_center[MAX_CLUSTERS][MAX_DATA_DIMENSIONS];


int readFile(char fileName[]) {

	FILE *filePointer;

	filePointer = fopen(fileName, "r");

	if(filePointer == NULL) {
		printf("File cannot be opened\n");
		return -1;
	}

	fscanf(filePointer, "%d\t%d\t%d\t%lf\t%lf", &number_of_data_points, &number_of_clusters, &number_of_dimensions, &fuzziness_coefficient, &epsilon);

	// Read the required arguments of fuzzy clustering

	if(number_of_data_points > MAX_DATA_POINTS) {
		printf("Number of data points exceeded\n");
		return 0;
	}

	if(number_of_clusters > MAX_CLUSTERS) {
		printf("Number of clusters exceeded\n");
		return 0;
	}

	if(number_of_dimensions > MAX_DATA_DIMENSIONS) {
		printf("Number of data dimensions exceeded\n");
		return 0;
	}

	// Fuzziness coefficient --> tolerance in clustering 
	if(fuzziness_coefficient <= 1.0) {
		printf("Fuzziness coefficient should be bigger than 1.0\n");
		return 0;
	}

	// Epsilon --> required accuracy 
	if( (epsilon <= 0.0) || (epsilon) > 1.0 ) {
		printf("Termination requirements should be > 0.0 and <= 1.0\n");
		return 0;
	}

	printf("%d\t%d\t%d\t%lf\t%lf\n", number_of_data_points, number_of_clusters, number_of_dimensions, fuzziness_coefficient, epsilon);


	// -----------------------------------------

	int i = 0;
	int j = 0;

	for(i = 0; i < number_of_data_points; i++) {	

		for(j = 0; j < number_of_dimensions; j++) {
			
			fscanf(filePointer, "%lf", &data_points[i][j]);

			if(data_points[i][j] < minValue_maxValue[j][0]) {
				minValue_maxValue[j][0] = data_points[i][j];
			}

			if(data_points[i][j] > minValue_maxValue[j][0]) {
				minValue_maxValue[j][0] = data_points[i][j];
			}	
				
		}

	}
	

	fclose(filePointer);

	return 1;

}

// generate degree of membership matrix
int generateDegreeOfMembershipMatrix() {

	int i, j;
	int randomValue;
	int sum[MAX_DATA_POINTS];


	for (i = 0; i < number_of_data_points; i++) {

		for (j = 0; j < number_of_clusters; j++) {

			randomValue = rand()%101;
			degree_of_membership[i][j] += randomValue;
			sum[i] += randomValue; 

		}
		
	}

	for(i = 0; i < number_of_data_points; i++) {

		for (j = 0; j < number_of_clusters; j++) {

			degree_of_membership[i][j] /= sum[i];

			printf("%lf\n", degree_of_membership[i][j]);

		}

	}


}

//	Distance from 1 specific point to the 1 specific cluster center
// 	Using Euclidean distance (norm - 1) 

float getNorm(int i, int j) {

	int k;
	float distance;
	float sum = 0.0;


	for (k = 0; k < number_of_dimensions; k++) {

		sum += pow(data_points[i][k] - cluster_center[j][k], 2);

	}

	distance = sqrt(sum);

	return distance;

}



float getNewDegreeOfMembershipValue(int i, int j) {

	int k, x;
	float powers_coefficient;

	powers_coefficient = 2/(fuzziness_coefficient - 1);
	float sum = 0.0;

	for(k = 0; k < number_of_clusters; k++) {

		x = getNorm(i, j) / getNorm(i, k);
		x = pow(x, powers_coefficient);
		sum += x;
	}

	return 1.0/(sum);

}

float updateDegreeOfMembershipValue() {

	int i,j;
	float new_value;
	float max_difference = 0.0, difference;

	for(i = 0; i < number_of_data_points; i++) {

		for(j = 0; j < number_of_clusters; j++) {

			new_value = getNewDegreeOfMembershipValue(i, j);
			difference = new_value - degree_of_membership[i][j];
			
			if(difference > max_difference) {
				max_difference = difference;
			}

			degree_of_membership[i][j] = new_value;

		}
 
	}

	printf("%f\n", max_difference);

	return max_difference;

}

// Calculate center vectors of cluster : 
int calculateCenterVectors() {

	int i,j,k;
	double x;

	double numerator, denominator;

	float degree_of_membership_powers[MAX_DATA_POINTS][MAX_CLUSTERS];

	printf("aaa\n");

	for(i = 0; i < number_of_data_points; i++) {

		for(j = 0; j < number_of_clusters; j++) {

			printf("%lf\n", degree_of_membership[i][j]);
			degree_of_membership_powers[i][j] = pow(degree_of_membership[i][j], fuzziness_coefficient);
			printf("%lf\n", degree_of_membership_powers[i][j]);

			printf("xxxxxxxx\n");

		}

	}

	for(j = 0; j < number_of_clusters; j++) {

		for(k = 0; k < number_of_dimensions; k++) {

			numerator = 0.0;
			denominator = 0.0;

			for(i = 0; i < number_of_data_points; i++) {

				numerator += degree_of_membership_powers[i][j] * data_points[i][k];
				denominator += degree_of_membership_powers[i][j];

			}

			cluster_center[j][k] = numerator/denominator;
			printf("111\n");
			printf("%lf\n", cluster_center[j][k]);

		}

	}

}


int fuzzy_c_means(char fileName[]) {

	readFile(fileName);
	generateDegreeOfMembershipMatrix();

	printf("llllllllllllllllllll\n");

	int number_of_iterations = 0;

	float max_difference = 0.0;

	do {

		calculateCenterVectors();
		max_difference = updateDegreeOfMembershipValue();
		number_of_iterations++;

	} while(max_difference > epsilon);

	printf("Number of iterations : %d\n", number_of_iterations);

	return 0;

}



int main(int argc, char *argv[])
{

	// read the required arguments and the data point matrix
	// update the minValue_maxValue[] matrix - the min and max value of each feature of data
	// readFile(argv[1]);

	// generate degree of membership matrix
	// with the constraint
	// sum of value of degree of membership in a row (all the clusters) equal 1 

	//readFile(argv[1]);

	printf("--------------\n");

	fuzzy_c_means(argv[1]);

	return 0;
}
