#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROOT 0
#define TAG 0

#define MAX 65536  // 2^16
typedef struct {
    int     S;
    #define Size(A) ((A)->S)                                      // defination with parameters
    int  entries[MAX];
    #define Entry(A,i,j) (*(((A)->entries) + ((A)->S)*(i) + (j)))  // defination with parameters, Array dereference
} SUBMAT_T;

typedef struct {
    int       P;             /* Total number of processes    */
    MPI_Comm  comm;          /* Communicator for entire grid */
    MPI_Comm  row_comm;      /* Communicator for my row      */
    MPI_Comm  col_comm;      /* Communicator for my col      */
    int       Q;             /* Size of grid                */
    int       my_row;        /* My row number                */
    int       my_col;        /* My column number             */
    int       my_rank;       /* My rank in the grid comm     */
} GRID_INFO_T;             

void print(int n, int *D){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
                printf("%d ", D[i * n + j]);
            }
        printf("\n");
    }
    printf("\n");
}

void Print_matrix(char* title, SUBMAT_T* local_A, GRID_INFO_T* grid, int n) {
    int mat_row, mat_col;
    int grid_row, grid_col;
    int source;
    int coords[2];
    int* temp;
    MPI_Status status;

    if (grid->my_rank == 0) {
        temp = (int*) malloc(Size(local_A)*sizeof(int));
        printf("%s\n", title);
        for (mat_row = 0;  mat_row < n; mat_row++) {
            grid_row = mat_row/Size(local_A);
            coords[0] = grid_row;
            for (grid_col = 0; grid_col < grid->Q; grid_col++) {
                coords[1] = grid_col;
                MPI_Cart_rank(grid->comm, coords, &source);
                if (source == 0) {
                    for(mat_col = 0; mat_col < Size(local_A); mat_col++)
                        printf("%d ", Entry(local_A, mat_row, mat_col));
                } else {
                    MPI_Recv(temp, Size(local_A), MPI_INT, source, 0, grid->comm, &status);
                    for(mat_col = 0; mat_col < Size(local_A); mat_col++)
                        printf("%d ", temp[mat_col]);
                }
            }
            printf("\n");
        }
        free(temp);
    } else {
        for (mat_row = 0; mat_row < Size(local_A); mat_row++) 
            MPI_Send(&Entry(local_A, mat_row, 0), Size(local_A), MPI_INT, 0, 0, grid->comm);
    }
                     
}  /* Print_matrix */

void Setup_grid(GRID_INFO_T*  grid /* out */) {
    int old_rank;
    int dimensions[2];
    int wrap_around[2];
    int coordinates[2];
    int varying_coords[2];

    /* Set up Global Grid Information */
    MPI_Comm_size(MPI_COMM_WORLD, &(grid->P));
    MPI_Comm_rank(MPI_COMM_WORLD, &old_rank);

    /* We assume p is a perfect square */     // but what if it's not a perfect square 
    int Q_candidate = sqrt(grid->P);
    grid->Q = (int)Q_candidate;

    if(Q_candidate != grid->Q){
        printf("The Fox algorithm cannot be applied to this number of processes.\n");
    }

    dimensions[0] = dimensions[1] = grid->Q; 

    /* We want a circular shift in second dimension. */ 
    /* Don't care about first                        */ 
    wrap_around[0] = wrap_around[1] = 1;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, 1, &(grid->comm));
    MPI_Comm_rank(grid->comm, &(grid->my_rank));
    MPI_Cart_coords(grid->comm, grid->my_rank, 2, coordinates);
    grid->my_row = coordinates[0];
    grid->my_col = coordinates[1];

    /* Set up row communicators */
    varying_coords[0] = 0; 
    varying_coords[1] = 1;
    MPI_Cart_sub(grid->comm, varying_coords, &(grid->row_comm));

    /* Set up column communicators */
    varying_coords[0] = 1; 
    varying_coords[1] = 0;
    MPI_Cart_sub(grid->comm, varying_coords, &(grid->col_comm));
} /* Setup_grid */

int main(int argc, char *argv[]){
    int P, my_rank;
    int N, Q, S;
    double Q_candidate;
    int* mat;

    GRID_INFO_T grid;

    MPI_Status status;
    
    // MPI Inits
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    Q_candidate = sqrt(P);
    Q = (int) Q_candidate;

    //printf("%d\n", grid.Q);

    if(my_rank == ROOT){
        scanf("%d", &N);

        if(Q_candidate != Q || N % Q != 0){
            printf("The Fox algorithm cannot be applied to this matrix size and number of processes.\n");
            return 1;
        }
    }

    Setup_grid(&grid);

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    S = N / Q;

    SUBMAT_T* submatA;

    submatA = (SUBMAT_T*) malloc(sizeof(SUBMAT_T));
    Size(submatA) = S;

    //printf("%d ", submatA->S);

    /////////////////////////// READ MATRIX ///////////////////////////

    //Read_matrix(submatA, &grid, n); 

    int mat_row, mat_col;
    int grid_row, grid_col;
    int dest;
    int coords[2];
    int* temp;

    if (grid.my_rank == 0) {  // Process 0 read matrix input from stdin and send them to other processess
        temp = (int*) malloc(Size(submatA)*sizeof(int));
        for (mat_row = 0;  mat_row < N; mat_row++) {
            grid_row = mat_row/Size(submatA);
            coords[0] = grid_row;
            for (grid_col = 0; grid_col < grid.Q; grid_col++) {
                coords[1] = grid_col;
                MPI_Cart_rank(grid.comm, coords, &dest);
                if (dest == 0) {
                    for (mat_col = 0; mat_col < Size(submatA); mat_col++)
                        scanf("%d", (submatA->entries)+mat_row*Size(submatA)+mat_col);
                } else {
                    for(mat_col = 0; mat_col < Size(submatA); mat_col++)
                        scanf("%f", temp + mat_col);
                    MPI_Send(temp, Size(submatA), MPI_INT, dest, 0, grid.comm);
                }
            }
        }
        free(temp);
    } else {  // Other processess receive matrix from process 0
        for (mat_row = 0; mat_row < Size(submatA); mat_row++) 
            MPI_Recv(&Entry(submatA, mat_row, 0), Size(submatA), MPI_INT, 0, 0, grid.comm, &status);
    }

    Print_matrix("SubmatA\n", submatA, &grid, N);

    free(submatA);

    MPI_Finalize();

    return 0;
}
