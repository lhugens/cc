#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ROOT 0
#define TAG 0

int main(int argc, char *argv[]){
    int P, my_rank;
    int *data = (int*) malloc(2*sizeof(int));

    MPI_Status status;

    // MPI Inits
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    data[0] = data[1] = my_rank;

    // GRID COMM
    MPI_Comm grid_comm;
    int dimensions[2], wrap_around[2], reorder = 1;
    int coordinates[2], my_grid_rank;
    dimensions[0] = dimensions[1] = 2;
    wrap_around[0] = wrap_around[1] = 1;

    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, reorder, &grid_comm);
    MPI_Comm_rank(grid_comm, &my_grid_rank);
    MPI_Cart_coords(grid_comm, my_grid_rank, 2, coordinates);

    // ROW COL COMMS
    int varying_coords[2];
    int row_rank;
    int col_rank;
    // create row communicators
    MPI_Comm row_comm;
    varying_coords[0] = 0; varying_coords[1] = 1;
    MPI_Cart_sub(grid_comm, varying_coords, &row_comm);
    MPI_Cart_rank(row_comm, coordinates, &row_rank);
    // create column communicators
    MPI_Comm col_comm;
    varying_coords[0] = 1; varying_coords[1] = 0;
    MPI_Cart_sub(grid_comm, varying_coords, &col_comm);
    MPI_Cart_rank(col_comm, coordinates, &col_rank);

    //printf("Global Rank: %d, Row Rank: %d\n", my_rank, row_rank);

    MPI_Cart_coords(grid_comm, my_grid_rank, 2, coordinates);

    int dest_coordinates[2];
    int dest_rank;
    dest_coordinates[0] = coordinates[0]-1;
    dest_coordinates[1] = coordinates[1];
    MPI_Cart_rank(grid_comm, dest_coordinates, &dest_rank);

    printf("Send: Process %d, grid_rank=%d, sending data to process %d\n", my_rank, my_grid_rank, dest_rank);
    MPI_Send(data, 2, MPI_INT, dest_rank, TAG, grid_comm);
    printf("Recv: Process %d, grid_rank=%d, receiving data from process %d\n", my_rank, my_grid_rank, dest_rank);
    MPI_Recv(data, 2, MPI_INT, MPI_ANY_SOURCE, TAG, grid_comm, &status); // works
    
    printf("data=[%d,%d]\n", data[0], data[1]);
    

    MPI_Finalize();

    return 0;
}
