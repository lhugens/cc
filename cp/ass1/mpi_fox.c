#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROOT 0
#define TAG 0
#define min(a, b) a < b ? a : b

void print(int n, int *D){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
                printf("%d ", D[i * n + j]);
            }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    int P, my_rank;
    int N, Q, S;
    double Q_candidate;
    int* mat;
    int* submatA;
    int* submatB;
    int* submatC;
    int* submatACC;

    MPI_Status status;

    // MPI Inits
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    Q_candidate = sqrt(P);
    Q = (int)Q_candidate;

    if(my_rank == ROOT){
        scanf("%d", &N);

        if(Q_candidate != Q || N % Q != 0){
            printf("The Fox algorithm cannot be applied to this matrix size and number of processes.\n");
            return 1;
        }
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    S = N / Q;

    mat = (int *) malloc(N * N * sizeof(int));
    submatA = (int*) malloc(S * S * sizeof(int));
    submatB = (int*) malloc(S * S * sizeof(int));
    submatC = (int*) malloc(S * S * sizeof(int));
    submatACC = (int*) malloc(S * S * sizeof(int));


    if(my_rank == ROOT){
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                scanf("%d", &mat[i * N + j]);
            }
        }

        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                if(i!=j && mat[i*N+j]==0){
                    mat[i * N + j] = -1;
                }
            }
        }
        printf("Process 0, mat:\n");
        print(N, mat);
        printf("\n");
    }

    MPI_Bcast(mat, N*N, MPI_INT, ROOT, MPI_COMM_WORLD);

    //if(my_rank!=ROOT){
    //    printf("Process %d, mat:\n", my_rank);
    //    print(N, mat);
    //    printf("\n");
    //}

    MPI_Comm grid_comm;
    int dimensions[2], wrap_around[2], reorder = 0;
    int coordinates[2], my_grid_rank;
    dimensions[0] = dimensions[1] = Q;
    wrap_around[0] = wrap_around[1] = 1;

    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, reorder, &grid_comm);
    MPI_Comm_rank(grid_comm, &my_grid_rank);
    MPI_Cart_coords(grid_comm, my_grid_rank, 2, coordinates);

    int i_init = coordinates[0]*S;
    int j_init = coordinates[1]*S;

    for(int i=0; i<S; i++){
        for(int j=0; j<S; j++){
            submatA[i * S + j] = mat[(i_init + i) * N + (j_init + j)];
            submatB[i * S + j] = submatA[i * S + j];
        }
    }

    //printf("Process %d\n", my_rank);
    //printf("Initial submatA\n");
    //print(S, submatA);
    //printf("\nInitial submatB\n");
    //print(S, submatB);
    //printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);

    // ROW COL COMMS
    int varying_coords[2];
    int row_rank;
    int col_rank;
    // create row communicators
    MPI_Comm row_comm;
    varying_coords[0] = 0; varying_coords[1] = 1;
    MPI_Cart_sub(grid_comm, varying_coords, &row_comm);
    MPI_Comm_rank(row_comm, &col_rank);
    // create column communicators
    MPI_Comm col_comm;
    varying_coords[0] = 1; varying_coords[1] = 0;
    MPI_Cart_sub(grid_comm, varying_coords, &col_comm);
    MPI_Comm_rank(col_comm, &row_rank);

    //printf("Process %d, row_rank=%d, col_rank=%d, coordinates=%d%d\n", my_rank, row_rank, col_rank, coordinates[0], coordinates[1]);

    int step = 1;

    int chosen_root = (row_rank + step) % Q;

    //printf("Process %d, step2_root=%d, Bcast matrix %d%d\n", my_rank, step2_root, chosen_coords[0], chosen_coords[1]);

    MPI_Bcast(submatA, S*S, MPI_INT, chosen_root, row_comm);

    printf("Process %d, row rank - chosen root = %d\n", my_rank, chosen_root);
    printf("submatA\n");
    print(S, submatA);
    printf("\n");

    MPI_Finalize();

    return 0;
}
