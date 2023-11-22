#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Object {
    char name;
    int proc;
    int food;
};

void print_world(struct Object *WORLD, int r, int c){
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            printf("%c ", WORLD[i*c+j].name);
        }
        printf("\n");
    }
}

int main(){
    int GEN_PROC_RABBITS;
    int GEN_PROC_FOXES;
    int GEN_FOOD_FOXES;
    int N_GEN;
    int R;
    int C;
    int N;
    struct Object *WORLD;

    scanf("%d", &GEN_PROC_RABBITS);
    scanf("%d", &GEN_PROC_FOXES);
    scanf("%d", &GEN_FOOD_FOXES);
    scanf("%d", &N_GEN);
    scanf("%d", &R);
    scanf("%d", &C);
    scanf("%d", &N);

    WORLD = (struct Object*) malloc(R*C*sizeof(struct Object));

    for(int i=0; i<R; i++){
        for(int j=0; j<C; j++){
            WORLD[i*C+j].name = '_';
            WORLD[i*C+j].proc = '_';
            WORLD[i*C+j].food = '_';
        }
    }
    //struct Object WORLD[3];
    //WORLD[0].name = 'l';
    //printf("%c\n", WORLD[0].name);

    char* name = (char *) malloc(6*sizeof(char));
    int x;
    int y;

    for(int n=0; n<N; n++){
        scanf("%s %d %d", name, &x, &y);
        if(strcmp(name, "ROCK") == 0){
            WORLD[x*C+y].name = '*';
            WORLD[x*C+y].proc = '*';
            WORLD[x*C+y].food = '*';
        } else if(strcmp(name, "RABBIT") == 0){
            WORLD[x*C+y].name = 'R';
            WORLD[x*C+y].proc = '0';
            WORLD[x*C+y].food = 'R';
        } else if(strcmp(name, "FOX") == 0){
            WORLD[x*C+y].name = 'F';
            WORLD[x*C+y].proc = '0';
            WORLD[x*C+y].food = '0';
        } 
    }
    free(name);

    print_world(WORLD, R, C);
    
}
