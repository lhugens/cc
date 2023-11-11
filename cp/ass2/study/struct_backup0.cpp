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
        printf("N=%d\n", N);

        WORLD = (char*) malloc(R*C*sizeof(char));

        char name[6];
        int x;
        int y;

        for(int n=0; n<N; n++){
            scanf("%s %d %d", name, &x, &y);
            if(strcmp(name, "ROCK") == 0){
                printf("hey\n");
            }
        }

    }
};

int main(){
    Ecosystem E;
    return 0;
}
