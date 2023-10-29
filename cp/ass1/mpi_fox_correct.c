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
                printf("%d ", D[i * n + j]);
            }
        printf("\n");
    }
    printf("\n");
}

int special_vector_mult(int n, int x[], int y[]){
    int c = -1;
    for(int k=0; k<n; k++){
        //printf("x[k]=%d, y[k]=%d\n", x[k], y[k]);
        if(x[k]!=-1 && y[k]!=-1){
            if(c != -1){
                c = min(c, x[k]+y[k]);
            } else {
                c = x[k]+y[k];
            }
        }
        //printf("c=%d\n", c);
    }
    return c;
}

void special_matrix_mult(int n, int* A, int* B, int* C){
    int *row = (int *) malloc(n * sizeof(int));
    int *col = (int *) malloc(n * sizeof(int));
    for(int i=0; i<n; i++){
        //printf("row %d\n", i);
        for(int j=0; j<n; j++){
            row[j] = A[i * n + j];
        }
        //for(int j=0; j<n; j++){
            //printf("%d ", row[j]);
        //}
        //printf("\n");
        for(int jp=0; jp<n; jp++){
            //printf("col %d\n", jp);
            for(int k=0; k<n; k++){
                col[k] = B[k * n + jp];
                //printf("%d ", col[k]);
            }
            //printf("\n");
            C[i * n + jp] = special_vector_mult(n, row, col);
            //printf("result\n");
            //printf("c = %d\n", C[i*N+jp]);
        }
    }
    free(row);
    free(col);
}

void special_matrix_min(int S, int* A, int* B, int* C){
    int a = -1;
    int b = -1;
    //printf("Min of:\n");
    //printf("Matrix A:\n");
    //print(S, A);
    //printf("Matrix B:\n");
    //print(S, B);
    for(int i=0; i<S; i++){
        for(int j=0; j<S; j++){
            a = A[i * S + j];
            b = B[i * S + j];
            if (a!=-1 && b!=-1){
                C[i*S+j] = min(a, b);
            } else if (a==-1){
                C[i*S+j] = b;
            } else if (b==-1){
                C[i*S+j] = a;
            } else {
                C[i*S+j] = -1;
            }
        }
    }
    //printf("Min Matrix C:\n");
    //print(S, C);
}

int main(int argc, char *argv[]){
    int P, my_rank;
    int N, Q, S;
    double Q_candidate;
    int* mat;
    int* submatA;
    int* temp_submatA;
    int* submatB;
    int* submatC;
    int* submatACC;

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

    mat = (int *) malloc(N * N * sizeof(int));
    submatA = (int*) malloc(S * S * sizeof(int));
    temp_submatA = (int*) malloc(S * S * sizeof(int));
    submatB = (int*) malloc(S * S * sizeof(int));
    submatC = (int*) malloc(S * S * sizeof(int));
    submatACC = (int*) malloc(S * S * sizeof(int));


    if(my_rank == ROOT){
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                scanf("%d", &mat[i * N + j]);
            }
        }

        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                if(i!=j && mat[i*N+j]==0){
                    mat[i * N + j] = -1;
                }
            }
        }
        printf("Process 0, mat:\n");
        print(N, mat);
        printf("\n");
    }

    MPI_Bcast(mat, N*N, MPI_INT, ROOT, MPI_COMM_WORLD);

    //if(my_rank!=ROOT){
    //    printf("Process %d, mat:\n", my_rank);
    //    print(N, mat);
    //    printf("\n");
    //}

    MPI_Comm grid_comm;
    int dimensions[2], wrap_around[2], reorder = 0;
    int coordinates[2], my_grid_rank;
    dimensions[0] = dimensions[1] = Q;
    wrap_around[0] = wrap_around[1] = 1;

    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, reorder, &grid_comm);
    MPI_Comm_rank(grid_comm, &my_grid_rank);
    MPI_Cart_coords(grid_comm, my_grid_rank, 2, coordinates);

    int i_init = coordinates[0]*S;
    int j_init = coordinates[1]*S;

    for(int i=0; i<S; i++){
        for(int j=0; j<S; j++){
            submatA[i * S + j] = mat[(i_init + i) * N + (j_init + j)];
            submatB[i * S + j] = submatA[i * S + j];
        }
    }

    //printf("Process %d\n", my_rank);
    //printf("Initial submatA\n");
    //print(S, submatA);
    //printf("\nInitial submatB\n");
    //print(S, submatB);
    //printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);

    // ROW COL COMMS
    int varying_coords[2];
    int row_rank;
    int col_rank;
    // create row communicators
    MPI_Comm row_comm;
    varying_coords[0] = 0; varying_coords[1] = 1;
    MPI_Cart_sub(grid_comm, varying_coords, &row_comm);
    MPI_Comm_rank(row_comm, &col_rank);
    // create column communicators
    MPI_Comm col_comm;
    varying_coords[0] = 1; varying_coords[1] = 0;
    MPI_Cart_sub(grid_comm, varying_coords, &col_comm);
    MPI_Comm_rank(col_comm, &row_rank);

    //printf("Process %d, row_rank=%d, col_rank=%d, coordinates=%d%d\n", my_rank, row_rank, col_rank, coordinates[0], coordinates[1]);

    for(int step=0; step<Q; step++){
        //////////////////////////////// STAGE 1 //////////////////////////////// 
        int chosen_root = (row_rank + step) % Q;
    
        //printf("Process %d, step2_root=%d, Bcast matrix %d%d\n", my_rank, step2_root, chosen_coords[0], chosen_coords[1]);
    
        //////////////////////////////// STAGE 2 & 3 //////////////////////////////// 
        if (chosen_root == col_rank) {
            MPI_Bcast(submatA, S*S, MPI_INT, chosen_root, row_comm);
            special_matrix_mult(S, submatA, submatB, submatC);
        } else {
            MPI_Bcast(temp_submatA, S*S, MPI_INT, chosen_root, row_comm);
            special_matrix_mult(S, temp_submatA, submatB, submatC);
        }
    
        //////////////////////////////// STAGE 4 //////////////////////////////// 
        int dest_rank;
        int dest_coordinates[2];
        dest_coordinates[0] = coordinates[0]-1;
        dest_coordinates[1] = coordinates[1];
        MPI_Cart_rank(grid_comm, dest_coordinates, &dest_rank);
        //MPI_Send(submatB, S*S, MPI_INT, dest_rank, TAG, grid_comm);
        int source_rank;
        int source_coordinates[2];
        source_coordinates[0] = coordinates[0]+1;
        source_coordinates[1] = coordinates[1];
        MPI_Cart_rank(grid_comm, source_coordinates, &source_rank);
        //MPI_Recv(submatB, S*S, MPI_INT, source_rank, TAG, grid_comm, &status); 

        source_rank = (row_rank + 1) % Q;
        dest_rank = (row_rank + Q - 1) % Q;

        MPI_Sendrecv_replace(submatB, S*S, MPI_INT, dest_rank, TAG, source_rank, TAG, col_comm, &status);

        if(step == 0){
            for(int i=0; i<S; i++)
                for(int j=0; j<S; j++)
                    submatACC[i * S + j] = submatC[i * S + j];
        } else {
            special_matrix_min(S, submatC, submatACC, submatACC);
        }
    }

    printf("Process %d, submatACC\n", my_rank);
    print(S, submatACC);
    printf("\n");


    MPI_Finalize();

    return 0;
}
