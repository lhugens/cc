#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define ROOT 0

int* special_matrix_multiply(){}

int* all_pais_shortest_paths(){}

int main(int argc, char *argv[]){
    int i, j, my_rank, N, P, Q, *mat;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    if (my_rank == ROOT){

        double infinity = INFINITY;
        double x = 5.0;
        double min = fmin(x, infinity);
        printf("min=%f\n", min);

        scanf("%d", &N);
        printf("N=%d\n", N);

        // P, Q difinition
        printf("P=%d\n", P);

        double Q_candidate = sqrt(P);
        Q = (int)Q_candidate;

        if(Q_candidate != Q){
            printf("P does not have an integer square root Q.\n");
            return 1;
        }
        printf("Q=%d\n", Q);
        ///////////////////

        mat = (int *) malloc(N * N * sizeof(int));

        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                scanf("%d", &mat[i * N + j]);

        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++)
                printf("%d ", mat[i * N + j]);
            printf("\n");
        }

        clock_t start, end;
        double cpu_time;

        start = clock();
        end = clock();
        cpu_time = ((double) (end-start)) / CLOCKS_PER_SEC;
        printf("CPU Time: %f seconds.\n", cpu_time);
    }


    MPI_Finalize();

    return 0;
}
