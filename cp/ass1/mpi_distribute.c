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

    //printf("Process %d, N=%d, Q=%d, S=%d\n", my_rank, N, Q, S);

    if(my_rank == ROOT){
        printf("Process %d\n", 0);
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                scanf("%d", &mat[i * N + j]);
                printf("%d ", mat[i * N + j]);
            }
            printf("\n");
        }
        printf("\n");

        int proc = 0;
        for(int i_init=0; i_init<=S; i_init+=S){
            for(int j_init=0; j_init<=S; j_init+=S){
                //printf("proc=%d\n", proc);
                for(int i=0; i<S; i++){
                    for(int j=0; j<S; j++){
                        submat[i * S + j] = mat[(i_init + i) * N + (j_init + j)];
                        //printf("%d ", submat[i * S + j]);
                    }
                    //printf("\n");
                }
                //MPI_Send(submat, S*S, MPI_INT, proc, proc, MPI_COMM_WORLD);
                MPI_Send(submat, S*S, MPI_INT, proc, TAG, MPI_COMM_WORLD);
                //MPI_Send(data, 2, MPI_INT, proc, TAG, MPI_COMM_WORLD);
                proc++;
            }
        }
    }
    //MPI_Bcast(mat, N*N, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Recv(submat, S*S, MPI_INT, ROOT, TAG, MPI_COMM_WORLD, &status); // works

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
