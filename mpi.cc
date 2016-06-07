#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

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

double subDet(double **matrix, int size, int id) {
    if (size == 1) {
        return matrix[0][0];
    }

    int subk;
    double result = 0.0;
    double **submatrix = (double **)malloc(sizeof(double *) * (size - 1));
    for (int i = 0; i < size - 1; i++) {
        submatrix[i] = (double *)malloc(sizeof(double) * (size - 1));
    }

    for (int j = 1; j < size; j++) {
        for (int k = 0; k < size; k++) {
            if (k < id) subk = k;
            else if (k > id) subk = k - 1;
            else continue;
            submatrix[j-1][subk] = matrix[j][k];
        }
    }
    if(id % 2 == 0)
        result = matrix[0][id] * calDet(submatrix, size - 1);
    else
        result = -matrix[0][id] * calDet(submatrix, size - 1);

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

    int i, group_size, my_rank;
    double tmp, ANS=0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&group_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    if(my_rank) {
        tmp = 0;
        for(i = my_rank; i < SIZE; i += group_size)
            tmp += subDet(matrix, SIZE, i);
        MPI_Send(&tmp, 1, MPI_DOUBLE, 0, my_rank, MPI_COMM_WORLD);
    }
    else {
        for(i = my_rank; i < SIZE; i += group_size)
            ANS += subDet(matrix, SIZE, i);
        for(i = 1; i < group_size; i++) {
            MPI_Recv(&tmp, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD, &status);
            ANS += tmp;
        }
        gettimeofday(&end, NULL);

        printf("Size of matrix: %d*%d\n", SIZE, SIZE );
        printf("Value of determinant: %lf\n", ANS );
        printf("Finished in %ld μs\n", 1000000 * ( end.tv_sec - start.tv_sec ) + (end.tv_usec - start.tv_usec) );
    }
    
    MPI_Finalize();
    return 0;
}
