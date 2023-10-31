#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define min(a, b) a < b ? a : b

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

int main(int argc, char *argv[]){
    int N, Q, S;
    int i, j, *row, *col, *C, *mat, sp;
    clock_t start, end;
    double cpu_time_used;

    scanf("%d", &N);

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

    start = clock();

    int m = 1;
    while(m<N-1){
        for(i=0; i<N; i++){
            //printf("row %d\n", i);
            for(j=0; j<N; j++){
                row[j] = mat[i * N + j];
            }
            for(j=0; j<N; j++){
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
                C[i * N + jp] = special_vector_mult(N, row, col);
                //printf("result\n");
                //printf("c = %d\n", C[i*N+jp]);
            }
        }
        //mat=C;
        for(i=0; i<N; i++){
            for(j=0; j<N; j++){
                mat[i*N+j] = C[i*N+j];
            }
        }
        //for(i=0; i<N; i++){
        //    for(j=0; j<N; j++){
        //        printf("%d ", mat[i*N+j]);
        //    }
        //    printf("\n");
        //}
        m = m*2;
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU time used: %f milliseconds\n", cpu_time_used*1000);

    //for (i = 0; i < N; i++){
    //    for (j = 0; j < N; j++){
    //        if(C[i * N + j]==-1){
    //            printf("%d ", 0);
    //        } else {
    //            printf("%d ", C[i * N + j]);
    //            //printf("%d ", mat[i * N + j]);
    //        }
    //        //printf("%d ", C[i * N + j]);
    //        //printf("%d ", mat[i * N + j]);
    //    }
    //    printf("\n");
    //}

    free(row);
    free(col);
    free(C);
    free(mat);

    return 0;
}
