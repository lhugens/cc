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
        for(int j=0; j<n; j++){
            //printf("%d ", row[j]);
        }
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

    //printf("Process %d\n", my_rank);
    //printf("my_rank=%d, my_grid_rank=%d, coordinates=[%d,%d]\n", my_rank, my_grid_rank, coordinates[0], coordinates[1]);

    //////////////////////////////////////////////

    for(int i=0; i<S; i++){
        for(int j=0; j<S; j++){
            submatA[i * S + j] = mat[(i_init + i) * N + (j_init + j)];
            submatB[i * S + j] = submatA[i * S + j];
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

    //printf("Global Rank: %d, Row Rank: %d\n", my_rank, row_rank);

    int* chosen_coords = (int*) malloc(2*sizeof(int));
    int dest_coordinates[2];
    int source_coordinates[2];
    int dest_rank;
    int source_rank;

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
            chosen_coords[0] = row_rank;
            chosen_coords[1] = (row_rank + step) % Q;
    
            int step2_root;
            MPI_Cart_rank(row_comm, chosen_coords, &step2_root);
    
            //printf("Bcast from proc %d\n", step2_root);

            MPI_Barrier(MPI_COMM_WORLD);
            
            //////////////////////////////// STAGE 2 //////////////////////////////// 
            //printf("I am proc %d, Step %d, Process %d Broadcasting submatA to row procs\n", my_rank, step, step2_root);
            int bcast_coordinates[2];
            MPI_Cart_coords(row_comm, step2_root, 2, bcast_coordinates);
            //printf("I am Process %d, Bcast submatA%d%d\n", my_rank, bcast_coordinates[0], bcast_coordinates[1]);
            if(step>0){
                printf("I am Process %d, Bcast submatA%d%d\n", my_rank, chosen_coords[0], chosen_coords[1]);
            }
            MPI_Bcast(submatA, S*S, MPI_INT, step2_root, row_comm);
            //printf("Process %d, submatA:\n", my_rank);
            //print(S, submatA);
            
            MPI_Barrier(MPI_COMM_WORLD);
    
            //////////////////////////////// STAGE 3 //////////////////////////////// 
            //printf("Process %d, Special matrix mult:\n", my_rank);
            //printf("submatA\n");
            //print(S, submatA);
            //printf("submatB\n");
            //print(S, submatB);
            //printf("\n");

            special_matrix_mult(S, submatA, submatB, submatC);

            MPI_Barrier(MPI_COMM_WORLD);
    
            //printf("Process %d\n", my_rank);
            //printf("SubmatA\n");
            //print(S, submatA);
            //printf("SubmatB\n");
            //print(S, submatB);
            //printf("SubmatC\n");
            //print(S, submatC);
            //printf("\n");
            //for(int i=0; i<S; i++){
            //    for(int j=0; j<S; j++){
            //        printf("%d ", submatB[i*S+j]);
            //    }
            //    printf("\n");
            //}
            //printf("\n");
    
            //////////////////////////////// STAGE 4 //////////////////////////////// 
            dest_coordinates[0] = coordinates[0]-1;
            dest_coordinates[1] = coordinates[1];
            MPI_Cart_rank(grid_comm, dest_coordinates, &dest_rank);
            //printf("Send: Process %d sending submatB to process %d\n", my_grid_rank, dest_rank);
            MPI_Send(submatB, S*S, MPI_INT, dest_rank, TAG, grid_comm);
            source_coordinates[0] = coordinates[0]+1;
            source_coordinates[1] = coordinates[1];
            MPI_Cart_rank(grid_comm, source_coordinates, &source_rank);
            //printf("Recv: Process %d receiving submatB from process %d\n", my_grid_rank, source_rank);
            MPI_Recv(submatB, S*S, MPI_INT, source_rank, TAG, grid_comm, &status); 
            //print(S, submatB);
            //printf("\n");
    
            if(step == 0){
                //submatACC = submatC;
                for(int i=0; i<S; i++)
                    for(int j=0; j<S; j++)
                        submatACC[i * S + j] = submatC[i * S + j];
            } else {
                special_matrix_min(S, submatC, submatACC, submatACC);
                //printf("Step %d, Process %d, submatACC:\n", step, my_rank);
                //print(S, submatACC);
            }
            //printf("Finished step %d\n", step);
            MPI_Barrier(MPI_COMM_WORLD);
        }
        m = m*2;
        break;
    }

    //printf("Process %d, submatACC:\n", my_rank);
    //print(S, submatACC);
    //printf("\n");
    //printf("Process %d, submatACC:\n", my_rank);
    //print(S, submatACC);
    //printf("\n");

    free(mat);
    free(submatA);
    free(submatB);
    free(submatC);
    free(submatACC);

    MPI_Finalize();

    return 0;
}
