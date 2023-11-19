#include <omp.h>
#include <stdio.h>

#define NTHREADS 4

int main(){
    int n, tid;
    n = NTHREADS;
    #pragma omp parallel num_threads(n) default(none) private(tid)
    {
        tid = omp_get_thread_num();

        #pragma omp for
        for(int i=0; i<4; i++)
            printf("T%d, i=%d\n", tid, i);
        #pragma omp single nowait
        printf("T%d, finished first cycle\n", tid);

        #pragma omp for
        for(int j=0; j<4; j++)
            printf("T%d, j=%d\n", tid, j);
        #pragma omp single nowait
        printf("T%d, finished second cycle\n", tid);
    }

    return 0;
}
