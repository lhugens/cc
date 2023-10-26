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
            if(D[i * n + j]==-1){
                printf("%d ", 0);
            } else {
                printf("%d ", D[i * n + j]);
            }
        }
        printf("\n");
    }
}

int special_vector_mult(int n, int x[], int y[]){
    int c = -1;
    for(int k=0; k<n; k++){
        if(x[k]!=-1 && y[k]!=-1){
            if(c != -1){
                c = min(c, x[k]+y[k]);
            } else {
                c = x[k]+y[k];
            }
       }
    }
    return c;
}

int* special_matrix_mult(int n, int *A, int *B){
    int *C, *row, *col;
    C = (int *) malloc(n * n * sizeof(int));
    row = (int *) malloc(n * sizeof(int));
    col = (int *) malloc(n * sizeof(int));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            row[j] = A[i * n + j];
        }
        for(int jp=0; jp<n; jp++){
            for(int k=0; k<n; k++){
                col[k] = B[k * n + jp];
            }
            C[i * n + jp] = special_vector_mult(n, row, col);
        }
    }
    free(row);
    free(col);
    return C;
} 

int main(int argc, char *argv[]){
    int P, my_rank;
    int N, Q, S;
    double Q_candidate;
    int* submatB;
    int* submatA;
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

    submatB = (int*) malloc(S * S * sizeof(int));
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

    printf("Process %d\n", my_rank);
    printf("my_rank=%d, my_grid_rank=%d, coordinates=[%d,%d]\n", my_rank, my_grid_rank, coordinates[0], coordinates[1]);

    //////////////////////////////////////////////

    for(int i=0; i<S; i++){
        for(int j=0; j<S; j++){
            submatB[i * S + j] = mat[(i_init + i) * N + (j_init + j)];
        }
    }

    //////////////////////////////////////////////
    
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

    MPI_Barrier(MPI_COMM_WORLD);

    printf("Global Rank: %d, Row Rank: %d\n", my_rank, row_rank);

    int step=0;


    int r = row_rank;
    int u = (r + step) % Q;
    int* chosen_coords = (int*) malloc(2*sizeof(int));
    chosen_coords[0] = r;
    chosen_coords[1] = u;

    int step2_root;
    MPI_Cart_rank(grid_comm, chosen_coords, &step2_root);

    //int* submatC = (int*) malloc(S * S * sizeof(int));
    
    printf("r=%d, u=%d -> Chose submatrix in proc: step2_root=%d\n", r, u, step2_root);

    printf("Process before%d, submatB:\n", my_rank);
    print(S, submatB);

    MPI_Bcast(submatB, S*S, MPI_INT, step2_root, grid_comm);
    
    MPI_Barrier(MPI_COMM_WORLD);


    //////////////////////////////////////////////

    printf("Process after%d, submatB:\n", my_rank);
    print(S, submatB);
    //for(int i=0; i<S; i++){
    //    for(int j=0; j<S; j++){
    //        printf("%d ", submatB[i*S+j]);
    //    }
    //    printf("\n");
    //}
    //printf("\n");

    free(mat);
    free(submatB);
    free(submatA);

    MPI_Finalize();

    return 0;
}
