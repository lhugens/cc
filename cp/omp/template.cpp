#include <omp.h>
#include <stdio.h>

#define NTHREADS 4

int main(){
    int n, tid;
    n = NTHREADS;
    #pragma omp parallel num_threads(n) default(none) private(tid)
    {
        tid = omp_get_thread_num();
        printf("T%d\n", tid);
    }

    return 0;
}
