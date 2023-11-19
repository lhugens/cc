#include <omp.h>
#include <stdio.h>

#define NTHREADS 1

int main(){
    int n, tid;

    tid = -1;

    n = NTHREADS;

    #pragma omp parallel num_threads(n) default(none) private(tid) shared(n)
    {
        #ifdef _OPENMP
            tid = omp_get_thread_num();
        #else
            tid = 1;
        #endif
        printf("Thread %d, hello\n", tid);
        #ifdef _OPENMP
        if (n != omp_get_num_threads())
            printf("Error: NTHREADS\n");
        #endif
    }
    printf("Thread %d, bye\n", tid);
    #ifdef _OPENMP
        n = omp_get_num_threads();
        printf("Thread %d, n=%d\n", tid, n);
    #endif
    return 0;
}
