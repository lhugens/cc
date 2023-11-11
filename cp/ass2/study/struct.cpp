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

    char* WORLD; // ecosystem state
    int G; // current generations

    void print_WORLD(){
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                printf("%c", WORLD[i*C+j]);
            }
            printf("\n");
        }
    }

    Ecosystem(){
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
                WORLD[i*C+j] = ' ';

        char* name = (char *) malloc(6*sizeof(char));
        int x, y;

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

        print_WORLD();

    }
};

int main(){
    Ecosystem E;
    return 0;
}
