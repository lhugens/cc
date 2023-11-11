#include <stdio.h>
#include <stdlib.h>
#include <cstring>

struct Ecosystem {
    int GEN_PROC_RABBITS;
    int GEN_PROC_FOXES;
    int GEN_FOOD_FOXES;
    int N_GEN;
    int R;
    int C;
    int N;
    char* WORLD;

    Ecosystem(){
        scanf("%d", &GEN_PROC_RABBITS);
        scanf("%d", &GEN_PROC_FOXES);
        scanf("%d", &GEN_FOOD_FOXES);
        scanf("%d", &N_GEN);
        scanf("%d", &R);
        scanf("%d", &C);
        scanf("%d", &N);

        printf("%d ", GEN_PROC_RABBITS);
        printf("%d ", GEN_PROC_FOXES);
        printf("%d ", GEN_FOOD_FOXES);
        printf("%d ", N_GEN);
        printf("%d ", R);
        printf("%d ", C);
        printf("%d\n", N);

        WORLD = (char*) malloc(R*C*sizeof(char));

        for(int i=0; i<R; i++)
            for(int j=0; j<C; j++)
                WORLD[i*R+j] = ' ';

        char name[6];
        int x;
        int y;

        for(int n=0; n<N; n++){
            scanf("%s %d %d", name, &x, &y);
            if(strcmp(name, "ROCK") == 0){
                WORLD[x*R+y] = '*';
            } else if(strcmp(name, "RABBIT") == 0){
                WORLD[x*R+y] = 'R';
            } else if(strcmp(name, "FOX") == 0){
                WORLD[x*R+y] = 'F';
            }
        }

        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                printf("%c", WORLD[i*R+j]);
            }
            printf("\n");
        }

    }
};

int main(){
    Ecosystem E;
    return 0;
}
