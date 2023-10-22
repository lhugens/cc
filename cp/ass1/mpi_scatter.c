#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROOT 0
#define M 6

void create_submatrix(int n, int *D, int s, int *sub){
    for(int i=0; i<s; i++){
        for(int j=0; j<s; j++){
            sub[i*s+j] = D[i*n+j];
        }
    }
}

int main(int argc, char *argv[]){
    int P, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int Q, N, S;
    
    // Q handling
    double Q_candidate = sqrt(P);
    Q = (int)Q_candidate;

    scanf("%d", &N);

    if(Q_candidate != Q || N % Q != 0){
        printf("The Fox algorithm cannot be applied to this matrix size and number of processes.\n");
        return 1;
    }

    S = N / Q;

    int *mat;
    mat = (int *) malloc(N * N * sizeof(int));

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            scanf("%d", &mat[i*N+j]);
            //scanf("%d", &mat[i * N + j]);
    
    //for (int i = 0; i < N; i++){
    //    for (int j = 0; j < N; j++){
    //        printf("%d ", mat[i][j]);
    //    }
    //    printf("\n");
    //}
    //printf("\n");
    
    int n = 3; // Size of the matrix
    int blocklen = 1; // Number of elements in each block
    int stride = 3; // Stride between matrix elements
    
    MPI_Datatype matrix_type;
    MPI_Type_vector(n, n*blocklen, stride, MPI_INT, &matrix_type);
    MPI_Type_commit(&matrix_type);

    //MPI_Bcast(&N, 3, MPI_INT, ROOT, MPI_COMM_WORLD);

    //if(my_rank == 1){
    //    N++;
    //}
    //printf("Process %d, N=%d\n", my_rank, N);

    int *submatrix;
    submatrix = (int*) malloc(3*3*sizeof(int));
    create_submatrix(6, mat, 3, submatrix);

    MPI_Bcast(submatrix, 3*3, MPI_INT, 0, MPI_COMM_WORLD);

    if(my_rank == 1){
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++)
                submatrix[i*3+j] = 55;
    }

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            printf("%d ", submatrix[i*3+j]);
        }
        printf("\n");
    }

    MPI_Type_free(&matrix_type);
    MPI_Finalize();

    return 0;
}
