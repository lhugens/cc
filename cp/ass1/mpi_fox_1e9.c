#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define min(a, b) a < b ? a : b
#define inf 1e9

int special_vector_mult(int n, int x[], int i, int y[], int j){
    if(i == j){
        return 0;
    }
    double c = inf;
    for(int k=0; k<n; k++){
        //printf("x[k]+y[k]=%lf\n", x[k]+y[k]);
        c = min(c, x[k]+y[k]);
        //printf("c=%lf\n", c);
    }
    return c;
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
                mat[i*N+j] = inf;
            }
        }
    }

    C = (int *) malloc(N * N * sizeof(int));
    row = (int *) malloc(3 * sizeof(int));
    col = (int *) malloc(3 * sizeof(int));

    for(i=0; i<N; i++){
        //printf("row %d\n", i);
        for(j=0; j<N; j++){
            row[j] = mat[i * N + j];
            //printf("%d ", row[j]);
        }
        //printf("\n");
        for(int jp=0; jp<N; jp++){
            //printf("col %d\n", jp);
            for(int k=0; k<N; k++){
                col[k] = mat[k * N + jp];
                //printf("%d ", col[k]);
            }
            //printf("\n");
            C[i * N + jp] = special_vector_mult(N, row, i, col, jp);
            //printf("result\n");
            //printf("c = %d\n", C[i*N+jp]);
        }
    }

    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            if(C[i * N + j]==inf){
                printf("%d ", 0);
            } else {
                printf("%d ", C[i * N + j]);
                //printf("%d ", mat[i * N + j]);
            }
        }
        printf("\n");
    }


    MPI_Finalize();

    return 0;
}

//int special_vector_mult(int n, int x[], int i, int y[], int j){
//    int c;
//        return 0;
//    }
//    for (int k=0; k<n; k++){
//        if((x[k] == 0 && k != i) || (y[k] == 0 && k != j)){
//                continue;
//        } 
//        else{
//            c = fmin(c, x[k]+y[k]);
//        } 
//    }
//    return c;
//}
