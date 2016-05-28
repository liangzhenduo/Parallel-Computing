#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

int SIZE, THREADS;
double ANS, **Matrix;
bool **lock;
pthread_mutex_t mutex;

double rec(int x,int y,int sum,double ans){
    if(y>=SIZE)
        return sum&1 ? -ans : ans;
    int i,j,cnt;
    double res=0;
    for(i=0; i<SIZE; i++){
        if(!lock[x][i]){
            for(j=i+1,cnt=0; j<SIZE; j++)
                cnt+=lock[x][j];
            lock[x][i]=1;
            res+=rec(x,y+1,sum+cnt,ans*Matrix[y][i]);
            lock[x][i]=0;
        }
    }
    return res;
}

void *calcDet(void *id){
    for(int i=(long)id; i<SIZE; i+=THREADS){
        double res=Matrix[0][i];
        lock[i][i]=1;
        res*=rec(i,1,0,1);
        pthread_mutex_lock(&mutex);
        ANS+=res;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(int argc,char **argv){
	SIZE=atoi(argv[1]);
    Matrix = (double **)malloc(sizeof(double *) * SIZE);
    lock = (bool **)malloc(sizeof(bool *) * SIZE);
    for (int i = 0; i < SIZE; i++) {
        Matrix[i] = (double *)malloc(sizeof(double) *  SIZE);
        lock[i] = (bool *)malloc(sizeof(bool) *  SIZE);
    }
    freopen(argv[2], "r", stdin);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            scanf("%lf", &Matrix[i][j]);
            lock[i][j]=0;
        }
    }
    THREADS=atoi(argv[3]);
    ANS=0;
    long i;
    struct timeval start, end;

    gettimeofday(&start, NULL);
    pthread_t threadList[THREADS];
    pthread_mutex_init(&mutex, NULL);

    for(i=0;i<THREADS;i++)
	    pthread_create(&threadList[i], NULL, calcDet, (void *)i);

    for(i=0;i<THREADS;i++)
	    pthread_join(threadList[i], NULL);

    gettimeofday(&end, NULL);

    printf("Number of threads: %d\n", THREADS );
    printf("Size of matrix: %d*%d\n", SIZE, SIZE );
    printf("Value of determinant: %lf\n", ANS );
    printf("Finished in %ld μs\n", 1000000 * ( end.tv_sec - start.tv_sec ) + (end.tv_usec - start.tv_usec) );
	return 0;
}