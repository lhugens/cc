#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int P, Q = 2, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Group MPI_GROUP_WORLD;
    MPI_Group first_row_group;
    MPI_Comm first_row_comm;

    int row_size, *process_ranks;
    process_ranks = (int*) malloc(Q*sizeof(int));
    for(int proc=0; proc<Q; proc++){
        process_ranks[proc] = proc;
    }

    MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD);

    MPI_Group_incl(MPI_GROUP_WORLD, Q, process_ranks, &first_row_group);

    MPI_Comm_create(MPI_COMM_WORLD, first_row_group, &first_row_comm);

    int *A;
    A = (int*) malloc(2*sizeof(int));

    int my_rank_in_first_row;
    if (my_rank < Q){
        MPI_Comm_rank(first_row_comm, &my_rank_in_first_row);
        if(my_rank_in_first_row == 0){
            A[0] = 32;
            A[1] = 48;
        }
        MPI_Bcast(A, 2, MPI_INT, 0, first_row_comm);
    }

//    if(my_rank_in_first_row == 1){
//
//    }

    MPI_Finalize();

    return 0;
}
