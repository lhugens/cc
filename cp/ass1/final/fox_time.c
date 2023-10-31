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

void special_matrix_mult(int n, int* A, int* B, int* C){
    int *row = (int *) malloc(n * sizeof(int));
    int *col = (int *) malloc(n * sizeof(int));
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
}

void special_matrix_min(int S, int* A, int* B, int* C){
    int a = -1;
    int b = -1;
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
    double start, finish;

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
        //printf("Process 0, mat:\n");
        //print(N, mat);
        //printf("\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    MPI_Bcast(mat, N*N, MPI_INT, ROOT, MPI_COMM_WORLD);

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

    int m=1;

    while(m<N-1){
        if(m>1){
            for(int i=0; i<S; i++){
                for(int j=0; j<S; j++){
                    submatA[i * S + j] = submatACC[i * S + j];
                    submatB[i * S + j] = submatACC[i * S + j];
                }
            }
        }
        for(int i=0; i<S; i++)
            for(int j=0; j<S; j++)
                submatACC[i * S + j] = -1;

        for(int step=0; step<Q; step++){
            //////////////////////////////// STAGE 1 //////////////////////////////// 
            int chosen_root = (row_rank + step) % Q;
        
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
            int source_rank;
            int source_coordinates[2];
            source_coordinates[0] = coordinates[0]+1;
            source_coordinates[1] = coordinates[1];
            MPI_Cart_rank(grid_comm, source_coordinates, &source_rank);
    
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
        m = m*2;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    finish = MPI_Wtime();

    if(my_rank==0){
        printf("Execution time = %f seconds\n", finish - start);
    }
    
    //MPI_Send(submatACC, S*S, MPI_INT, ROOT, TAG, MPI_COMM_WORLD);

    //if(my_rank==0){
    //    for(int i=0; i<S; i++)
    //        for(int j=0; j<S; j++)
    //            mat[i * N + j] = submatACC[i * S + j];
    //    for(int proc=1; proc<P; proc++){
    //        MPI_Recv(submatACC, S*S, MPI_INT, proc, TAG, MPI_COMM_WORLD, &status);

    //        MPI_Cart_coords(grid_comm, proc, 2, coordinates);
    //        i_init = coordinates[0]*S;
    //        j_init = coordinates[1]*S;
    //        for(int i=0; i<S; i++)
    //            for(int j=0; j<S; j++)
    //                mat[(i_init + i) * N + (j_init + j)] = submatACC[i * S + j];
    //    }

    //    for (int i = 0; i < N; i++){
    //        for (int j = 0; j < N; j++){
    //            if(mat[i*N+j]==-1){
    //                mat[i * N + j] = 0;
    //            }
    //        }
    //    }
    //    print(N, mat);
    //}
    
    MPI_Finalize();

    return 0;
}
