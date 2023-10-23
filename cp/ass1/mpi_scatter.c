#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROOT 0
#define M 6

void print_submatrix(int s, int *sub){
    for(int i=0; i<s; i++){
        for(int j=0; j<s; j++){
            printf("%d ", sub[i*s+j]);
        }
        printf("\n");
    }

}

void create_submatrix(int ia, int ja, int n, int *D, int s, int *sub){
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
                    
    int my_grid_rank, dims[2], wrap_around[2];
    dims[0] = dims[1] = Q;
    wrap_around[0] = wrap_around[1] = 1;

    MPI_Comm grid_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, wrap_around, 1, &grid_comm);
    MPI_Comm_rank(grid_comm, &my_grid_rank);

    int varying_coords[2];

    // create row communicators
    MPI_Comm row_comm;
    varying_coords[0] = 0; varying_coords[1] = 1;
    MPI_Cart_sub(grid_comm, varying_coords, &row_comm);

    // create column communicators
    MPI_Comm col_comm;
    varying_coords[0] = 1; varying_coords[1] = 0;
    MPI_Cart_sub(grid_comm, varying_coords, &col_comm);
    
    MPI_Datatype matrix_type;
    MPI_Type_vector(n, n*blocklen, stride, MPI_INT, &matrix_type);
    MPI_Type_commit(&matrix_type);

    int *submatrix;
    submatrix = (int*) malloc(3*3*sizeof(int));

    if(my_rank==0){
        // Bcast the 00 submatrix
        create_submatrix(0, 0, 6, mat, 3, submatrix);
    
        printf("Submatrix, Process %d\n", my_rank);
        print_submatrix(3, submatrix);
    
        MPI_Bcast(submatrix, 3*3, MPI_INT, 0, row_comm);
    }

    if(my_rank==2){
        create_submatrix(2, 0, 6, mat, 3, submatrix);
        printf("Submatrix, Process %d\n", my_rank);
        print_submatrix(3, submatrix);
        //MPI_Bcast(submatrix, 3*3, MPI_INT, 2, row_comm);
    }

    MPI_Type_free(&matrix_type);
    MPI_Finalize();

    return 0;
}
