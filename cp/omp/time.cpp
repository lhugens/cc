#include <omp.h>
#include <stdio.h>
#include <time.h>

#define NTHREADS 1

int main(){
    int n, tid;
    #ifdef _OPENMP
        double start, end, tick, cpu_time_used;
    #else
        clock_t start, end;
        double cpu_time_used;
    #endif

    #ifdef _OPENMP
        start = omp_get_wtime();
    #else
        start = clock();
    #endif

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

    #ifdef _OPENMP
        end = omp_get_wtime();
        cpu_time_used = end-start;
        tick = omp_get_wtick();
        printf("WTick %f\n", tick);
        printf("Execution time omp %f\n", cpu_time_used*1000);
    #else
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Execution time sequential %f\n", cpu_time_used*1000);
    #endif


    return 0;
}
