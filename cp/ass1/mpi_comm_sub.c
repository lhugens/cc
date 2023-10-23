#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int P, Q=2, my_rank;

    // MPI Inits
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // GRID COMMS
    MPI_Comm grid_comm;
    int dimensions[2], wrap_around[2], reorder = 1;
    dimensions[0] = dimensions[1] = Q;
    wrap_around[0] = wrap_around[1] = 1;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, reorder, &grid_comm);
    int coordinates[2], my_grid_rank;
    MPI_Comm_rank(grid_comm, &my_grid_rank); // necessary to call, because or reorder=1, order could have been changes by MPI
    MPI_Cart_coords(grid_comm, my_grid_rank, 2, coordinates);

    // ROW COL COMMS
    int varying_coords[2];
    // create row communicators
    MPI_Comm row_comm;
    varying_coords[0] = 0; varying_coords[1] = 1;
    MPI_Cart_sub(grid_comm, varying_coords, &row_comm);
    int row_rank;
    MPI_Cart_rank(row_comm, coordinates, &row_rank);
    // create column communicators
    MPI_Comm col_comm;
    varying_coords[0] = 1; varying_coords[1] = 0;
    MPI_Cart_sub(grid_comm, varying_coords, &col_comm);
    int col_rank;
    MPI_Cart_rank(col_comm, coordinates, &col_rank);

    // read N
    int N;
    scanf("%d", &N);
    // MATRIX TYPE
    MPI_Datatype matrix_type;
    MPI_Type_vector(N, N, 6, MPI_INT, &matrix_type);
    MPI_Type_commit(&matrix_type);
    // read mat
    int *mat = (int*) malloc(N*N*sizeof(int));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            scanf("%d", &mat[i*N+j]);


    printf("my_rank: %d, col_rank: %d, coordinates ", my_rank, col_rank);
    for(int i=0; i<2; i++){
        printf("%d ", coordinates[i]);
    }
    printf("\n");

    MPI_Bcast(mat, N * N, matrix_type, 0, col_comm);

    printf("Received\n");
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            printf("%d ", mat[i*N+j]);
        }
        printf("\n");
    }
    printf("\n");

    MPI_Finalize();

    return 0;
}
