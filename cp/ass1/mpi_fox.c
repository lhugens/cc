#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define min(a, b) a < b ? a : b

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

void squaring(int n, int* D){
    int *c, *row, *col;
    c = (int *) malloc(n * n * sizeof(int));
    row = (int *) malloc(n * sizeof(int));
    col = (int *) malloc(n * sizeof(int));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            row[j] = D[i * n + j];
        }
        for(int jp=0; jp<n; jp++){
            for(int k=0; k<n; k++){
                col[k] = D[k * n + jp];
            }
            c[i * n + jp] = special_vector_mult(n, row, col);
        }
    }
    D=c;
} 

int main(int argc, char *argv[]){
    int P, my_rank;
    int N, Q, S;
    int i, j, *row, *col, *C, *mat, sp;

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

    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            if(i!=j && mat[i*N+j]==0){
                mat[i*N+j] = -1;
            }
        }
    }



    C = (int *) malloc(N * N * sizeof(int));
    row = (int *) malloc(N * sizeof(int));
    col = (int *) malloc(N * sizeof(int));

    int m = 1;
    while(m<N-1){
        for(i=0; i<N; i++){
            for(j=0; j<N; j++){
                row[j] = mat[i * N + j];
            }
            for(int jp=0; jp<N; jp++){
                for(int k=0; k<N; k++){
                    col[k] = mat[k * N + jp];
                }
                C[i * N + jp] = special_vector_mult(N, row, col);
            }
        }
        mat=C;
        m = m*2;
    }

    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            if(C[i * N + j]==-1){
                printf("%d ", 0);
            } else {
                printf("%d ", C[i * N + j]);
            }
        }
        printf("\n");
    }


    MPI_Finalize();

    return 0;
}
