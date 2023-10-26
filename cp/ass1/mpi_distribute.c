#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROOT 0
#define TAG 0

int main(int argc, char *argv[]){
    int P, my_rank;
    int N, Q, S;
    double Q_candidate;
    int* submat;
    int* mat;

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

    submat = (int*) malloc(S * S * sizeof(int));
    mat = (int *) malloc(N * N * sizeof(int));

    if(my_rank == ROOT){
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                scanf("%d", &mat[i * N + j]);
            }
        }
    }

    MPI_Bcast(mat, N*N, MPI_INT, ROOT, MPI_COMM_WORLD);

    //////////////////////////////////////////////

    MPI_Comm grid_comm;
    int dimensions[2], wrap_around[2], reorder = 1;
    int coordinates[2], my_grid_rank;
    dimensions[0] = dimensions[1] = Q;
    wrap_around[0] = wrap_around[1] = 1;

    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, reorder, &grid_comm);

    MPI_Comm_rank(grid_comm, &my_grid_rank);

    MPI_Cart_coords(grid_comm, my_grid_rank, 2, coordinates);

    int i_init = coordinates[0]*S;
    int j_init = coordinates[1]*S;

    printf("my_rank=%d, my_grid_rank=%d, coordinates=[%d,%d]\n", my_rank, my_grid_rank, coordinates[0], coordinates[1]);

    //////////////////////////////////////////////

    for(int i=0; i<S; i++){
        for(int j=0; j<S; j++){
            submat[i * S + j] = mat[(i_init + i) * N + (j_init + j)];
        }
    }

    printf("Process %d, submat:\n", my_rank);
    for(int i=0; i<S; i++){
        for(int j=0; j<S; j++){
            printf("%d ", submat[i*S+j]);
        }
        printf("\n");
    }
    printf("\n");

    free(submat);
    free(mat);

    MPI_Finalize();

    return 0;
}
