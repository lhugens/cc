#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

int main(){
    int n, tid;
    n = NTHREADS;

    int *a, *b;

    int R = 4, C = 3;

    a = (int*) malloc(R*C*sizeof(int)); 
    b = (int*) malloc(R*C*sizeof(int)); 

    for(int i=0; i<R; i++){
        for(int j=0; j<C; j++){
            a[i*C+j] = i*C+j;
            b[i*C+j] = 0;
        }
    }

    printf("A\n");
    for(int i=0; i<R; i++){
        for(int j=0; j<C; j++){
            printf("%d ", a[i*C+j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("B\n");
    for(int i=0; i<R; i++){
        for(int j=0; j<C; j++){
            printf("%d ", b[i*C+j]);
        }
        printf("\n");
    }
    printf("\n");

    #pragma omp parallel num_threads(n) default(none) private(tid) shared(a,b,R,C)
    {
        tid = omp_get_thread_num();

        #pragma omp for
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                b[i*C+j] = a[i*C+j];
            }
        }
    }

    printf("B\n");
    for(int i=0; i<R; i++){
        for(int j=0; j<C; j++){
            printf("%d ", b[i*C+j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}
