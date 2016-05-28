#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double calDet(double **matrix, int size) {
	if (size == 1) {
		return matrix[0][0];
	}

	int subk;
	double result = 0.0;
	double **submatrix = (double **)malloc(sizeof(double *) * (size - 1));
	for (int i = 0; i < size - 1; i++) {
		submatrix[i] = (double *)malloc(sizeof(double) * (size - 1));
	}
	for (int i = 0; i < size; i++) {
		for (int j = 1; j < size; j++) {
			for (int k = 0; k < size; k++) {
				if (k < i) subk = k;
				else if (k > i) subk = k - 1;
				else continue;
				submatrix[j-1][subk] = matrix[j][k];
			}
		}
		if (i % 2 == 0) {
			result += matrix[0][i] * calDet(submatrix, size - 1);
		}
		else {
			result -= matrix[0][i] * calDet(submatrix, size - 1);
		}
	}
	for (int i = 0; i < size - 1; i++) {
		free(submatrix[i]);
	}
	free(submatrix);
	return result;
}

int main(int argc, char *argv[]) {

	int SIZE = atoi(argv[1]);
	char *input = argv[2];
	double **matrix = (double **)malloc(sizeof(double *) * SIZE);
	for (int i = 0; i < SIZE; i++) {
		matrix[i] = (double *)malloc(sizeof(double) *  SIZE);
	}

	freopen(input, "r", stdin);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			scanf("%lf", &matrix[i][j]);
		}
	}
	struct timeval start, end;

	gettimeofday(&start, NULL);
	double ANS=calDet(matrix, SIZE);
	gettimeofday(&end, NULL);

	printf("Size of matrix: %d*%d\n", SIZE, SIZE );
    printf("Value of determinant: %lf\n", ANS );
    printf("Finished in %ld μs\n", 1000000 * ( end.tv_sec - start.tv_sec ) + (end.tv_usec - start.tv_usec) );
	
	return 0;
}
