#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int P, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Comm my_row_comm;
    int Q = 2, my_row;

    my_row = my_rank / Q;

    MPI_Comm_split(MPI_COMM_WORLD, my_row, my_rank, &my_row_comm);

    int my_row_rank, my_row_size;
    MPI_Comm_rank(my_row_comm, &my_row_rank);
    MPI_Comm_size(my_row_comm, &my_row_size);

    printf("Global Rank: %d, Row Rank: %d, Row Size: %d\n", my_rank, my_row_rank, my_row_size);

    MPI_Finalize();

    return 0;
}
