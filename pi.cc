#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

int THREADS,POINTS;
int n,m;
pthread_mutex_t mutex;
double *X, *Y;


void *calcPi(void *id) {
    int i,j,k;
    double x,y;
    for (i=(long)id,j=k=0; i<POINTS; i+=THREADS,j++) {
        x=X[i], y=Y[i];
		k+=(x*x+y*y<1.0)?1:0;
    }
	pthread_mutex_lock(&mutex);
    n+=k;
    m+=j;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  	n=0,m=0;
    POINTS=atoi(argv[1]);
    THREADS=atoi(argv[2]);
    long i;
    struct timeval start, end;
    X = (double *)malloc(sizeof(double) * POINTS);
    Y = (double *)malloc(sizeof(double) * POINTS);
    for (int i = 0; i < POINTS; i++) {
        X[i]=(double)rand()/RAND_MAX;
        Y[i]=(double)rand()/RAND_MAX;
    }

    pthread_t threads[THREADS];
    pthread_mutex_init(&mutex, NULL);

    gettimeofday( &start, NULL );
    
    for(i=0; i<THREADS; i++)
        pthread_create(&threads[i], NULL, calcPi, (void *)i);

    for(i=0; i<THREADS; i++)
        pthread_join(threads[i], NULL);

	gettimeofday(&end, NULL);
    printf("Number of threads: %d\n", THREADS );
    printf("Number of points: %d/%d\n", n,m );
    printf("Value of π: %lf\n", 4.0*n/m );
    printf("Finished in %ld μs\n", 1000000 * ( end.tv_sec - start.tv_sec ) + (end.tv_usec - start.tv_usec) );
    return 0;
}
