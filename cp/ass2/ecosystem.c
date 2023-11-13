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

void select_adjacent_cell(char* mat, int r, int c, int x, int y, char object){
    int P = 0;
    int possible_x[3];
    int possible_y[3];

    int x_p, y_p;

    // north
    x_p = x-1;
    y_p = y;
    if(x_p!=-1 && mat[x_p*c+y_p] == object){
        possible_x[P] = x_p;
        possible_y[P] = y_p;
        P++;
    }

    // east
    x_p = x;
    y_p = y+1;
    if(y_p!=c+1 && mat[x_p*c+y_p] == object){
        possible_x[P] = x_p;
        possible_y[P] = y_p;
        P++;
    }

    // south
    x_p = x+1;
    y_p = y;
    if(x_p!=r+1 && mat[x_p*c+y_p] == object){
        possible_x[P] = x_p;
        possible_y[P] = y_p;
        P++;
    }

    // west
    x_p = x;
    y_p = y-1;
    if(y_p!=-1 && mat[x_p*c+y_p] == object){
        possible_x[P] = x_p;
        possible_y[P] = y_p;
        P++;
    }

    int chosen = (0 + x + y) % P;
    //printf("possible chosen: (%d, %d)\n", possible_x[0], possible_y[0]);
    printf("x+y=%d, P=%d\n", x+y, P);
    printf("chosen %d\n", chosen);
    printf("move to (%d, %d)\n", possible_x[chosen], possible_y[chosen]);
}

void move_rabbits(char* mat, int r, int c){
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            if(mat[i*c+j] == 'R'){
                //printf("Found R @ (%d, %d)\n", i, j);
                select_adjacent_cell(mat, r, c, i, j, '_');
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
