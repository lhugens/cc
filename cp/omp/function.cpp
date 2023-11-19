#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4
#define N 5

int sum(int tid, int *a){
    int s=0;
    #pragma omp for reduction(+:s)
    for(int i=0; i<N; i++){
       s += a[i]; 
    }
    return s;
}

int main(){
    int n, tid;
    n = NTHREADS;
    int *a, S;
    a = (int*) malloc(N*sizeof(int));
    for(int i=0; i<N; i++){
        a[i] = i;
        printf("%d ", a[i]);
    }
    printf("\n");

    #pragma omp parallel num_threads(n) default(none) private(tid) shared(S,a)
    {
        tid = omp_get_thread_num();
        S = sum(tid, a);
    }
    printf("S=%d\n", S);

    return 0;
}
