#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define ROOT 0

void special_vector_multiply(int s, int x[], int y[], int partial[]){
    int i, j;
    for (i=0; i<s; i++){
        partial[i] = x[i] * y[i];
    }
}

int* all_pais_shortest_paths(){}

int main(int argc, char *argv[]){
    int i, j, my_rank, N, P, Q, S, *mat, *row, *col, *c;

    row[0] = 0;
    row[1] = 2;
    row[2] = 0;

    col[0] = 2;
    col[1] = 0;
    col[2] = 2;

    double start, finish;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

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

        if(Q_candidate != Q || N % Q != 0){
            printf("The Fox algorithm cannot be applied to this matrix size and number of processes.\n");
            return 1;
        } 
        printf("Q=%d\n", Q);

        S = N / Q;
        ///////////////////
        
        special_vector_multiply(S, row, col, c);
        for (int i=0; i<S; i++)
            printf("%d ", c[i]);
        printf("\n");

        mat = (int *) malloc(N * N * sizeof(int));

        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                scanf("%d", &mat[i * N + j]);

        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++)
                printf("%d ", mat[i * N + j]);
            printf("\n");
        }

        double cpu_time;

    }
    MPI_Barrier(MPI_COMM_WORLD);
    finish = MPI_Wtime();
    if(my_rank==ROOT){
        printf("CPU Time: %f seconds.\n", finish-start);
    }


    MPI_Finalize();

    return 0;
}
