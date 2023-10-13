#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int special_vector_mult(int n, int x[], int i, int y[], int j){
    int c;
    if(i == j){
        return 0;
    }
    for (int k=0; k<n; k++){
        if((x[k] == 0 && k != i) || (y[k] == 0 && k != j)){
                continue;
        } 
        else{
            c = fmin(c, x[k]+y[k]);
        } 
    }
    return c;
}

int main(int argc, char *argv[]){
    int P, my_rank;
    int N, Q, S;
    int i, j, *row, *col, *c, *mat, sp;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    scanf("%d", &N);

    double Q_candidate = sqrt(P);
    Q = (int)Q_candidate;

    if(Q_candidate != Q || N % Q != 0){
        printf("The Fox algorithm cannot be applied to this matrix size and number of processes.\n");
        return 1;
    } 

    S = N / Q;

    mat = (int *) malloc(N * N * sizeof(int));
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            scanf("%d", &mat[i * N + j]);

    c = (int *) malloc(N * N * sizeof(int));
    row = (int *) malloc(3 * sizeof(int));
    col = (int *) malloc(3 * sizeof(int));

    for(int s=0; s<N; s++){
        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++){
                row[j] = mat[i * N + j];
                col[j] = mat[j * N + i];
            }
            for (j = 0; j < N; j++){
                c[i * N + j] = special_vector_mult(N, row, i, col, j);
            }
        }
        mat = c;
    }

    //row[0] = 0; row[1] = 5; row[2] = 0; row[3] = 0; col[0] = 0; col[1] = 1; col[2] = 0; col[3] = 0;
    //sp = special_vector_mult(3, row, 0, col, 2);
    //
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            printf("%d ", c[i * N + j]);
            //printf("%d ", mat[i * N + j]);
        }
        printf("\n");
    }


    MPI_Finalize();

    return 0;
}
