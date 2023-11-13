#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_world(char* mat, int r, int c){
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            printf("%c ", mat[i*c+j]);
        }
        printf("\n");
    }
}

void move_rabbits(char* mat, int r, int c){
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            if(mat[i*c+j] == 'R'){
                printf("Found R @ (%d, %d)\n", i, j);
            }
        }
    }
}

int main(){
    // INITIALIZE VARIABLES BY READING FROM STDIN
    int GEN_PROC_RABBITS;
    int GEN_PROC_FOXES;
    int GEN_FOOD_FOXES;
    int N_GEN;
    int R;
    int C;
    int N;
    char* WORLD;

    scanf("%d", &GEN_PROC_RABBITS);
    scanf("%d", &GEN_PROC_FOXES);
    scanf("%d", &GEN_FOOD_FOXES);
    scanf("%d", &N_GEN);
    scanf("%d", &R);
    scanf("%d", &C);
    scanf("%d", &N);

    WORLD = (char*) malloc(R*C*sizeof(char));

    for(int i=0; i<R; i++)
        for(int j=0; j<C; j++)
            WORLD[i*C+j] = '_';

    char* name = (char *) malloc(6*sizeof(char));
    int x;
    int y;

    for(int n=0; n<N; n++){
        scanf("%s %d %d", name, &x, &y);
        if(strcmp(name, "ROCK") == 0){
            WORLD[x*C+y] = '*';
        } else if(strcmp(name, "RABBIT") == 0){
            WORLD[x*C+y] = 'R';
        } else if(strcmp(name, "FOX") == 0){
            WORLD[x*C+y] = 'F';
        }
    }
    free(name);

    print_world(WORLD, R, C);

    move_rabbits(WORLD, R, C);



    free(WORLD);
    return 0;
}
