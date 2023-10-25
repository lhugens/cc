#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ROOT 0
#define TAG 0

int main(int argc, char *argv[]){
    int P, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Status status;

    int* data = (int*) malloc(2*sizeof(int));

    //printf("Process %d\n", my_rank);

    if(my_rank==0){
        data[0] = 32;
        data[1] = 47;
        for(int proc=1; proc<4; proc++){
            MPI_Send(data, 2, MPI_INT, proc, TAG, MPI_COMM_WORLD);
            //int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
            //MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, 0, MY_TAG, MPI_COMM_WORLD);
        }
    } else {
        //MPI_Recv(data, 1, MPI_INT, ROOT, TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(data, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        //MPI_Recv(msg, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }

    printf("Process %d, data=[%d,%d]\n", my_rank, data[0], data[1]);

    MPI_Finalize();

    return 0;
}
