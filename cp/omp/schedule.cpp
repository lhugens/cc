#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 2

int main(){
    int n, tid;
    n = NTHREADS;
    
    int *a, *b, *c, *d;
    int N = 4;
    a = (int*) malloc(N*sizeof(int));
    b = (int*) malloc(N*sizeof(int));
    c = (int*) malloc(N*sizeof(int));
    d = (int*) malloc(N*sizeof(int));

    int chunk = N/n;

    for(int i=0; i<4; i++){
        a[i] = i;
        d[i] = 0;
    }

    #pragma omp parallel num_threads(n) default(none) private(tid) shared(a,b,c,d,chunk,N)
    {
        tid = omp_get_thread_num();

        #pragma omp for schedule(static,chunk) nowait
        for(int i=0; i<N; i++)
            b[i] = 2*a[i];

        #pragma omp for schedule(dynamic,chunk) nowait
        for(int i=0; i<N; i++)
            c[i] = 3*a[i];

        #pragma omp for schedule(guided) nowait
        for(int i=0; i<N; i++)
            for(int j=0; j<N; j++)
                d[i] += 4*a[i];
    }

    for(int i=0; i<4; i++){
        printf("a[%d]=%d, b[%d]=%d, c[%d]=%d, d[%d]=%d\n", i, a[i], i, b[i], i, c[i], i, d[i]);
    }

    return 0;
}
