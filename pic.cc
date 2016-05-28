#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

int POINTS;
double *X, *Y;

int main(int argc, char *argv[]) {
    POINTS=atoi(argv[1]);
    double x,y;
    int n,m;
    struct timeval start, end;

    X = (double *)malloc(sizeof(double) * POINTS);
    Y = (double *)malloc(sizeof(double) * POINTS);
    for (int i = 0; i < POINTS; i++) {
        X[i]=(double)rand()/RAND_MAX;
        Y[i]=(double)rand()/RAND_MAX;
    }

    gettimeofday( &start, NULL );

    for(n=m=0;m<POINTS;m++){
        x=X[m], y=Y[m];
        n+=(x*x+y*y<1.0)?1:0;
    }

	gettimeofday(&end, NULL);
    printf("Number of points: %d/%d\n", n,m );
    printf("Value of π: %lf\n", 4.0*n/m );
    printf("Finished in %ld μs\n", 1000000 * ( end.tv_sec - start.tv_sec ) + (end.tv_usec - start.tv_usec) );
    return 0;
}
