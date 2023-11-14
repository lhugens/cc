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

        WORLD = (char*) malloc(R*C*sizeof(char));

        for(int i=0; i<R; i++)
            for(int j=0; j<C; j++)
                WORLD[i*R+j] = '_';

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
    }

    void print_world(){
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                printf("%c ", WORLD[i*C+j]);
            }
            printf("\n");
        }
    }

    void move_rabbits(){
        int *coords;
        coords = (int*) malloc(2*sizeof(int));
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                if(WORLD[i*C+j] == 'R'){
                    if (select_adjacent_cell(i, j, '_', coords)){
                        printf("chosen (%d, %d)\n", coords[0], coords[1]); 
                    }
                }
            }
        }
    }

    bool select_adjacent_cell(int x, int y, char object, int *chosen_coords){
        int P = 0;
        int x_p;
        int y_p;
        int possible_x[4];
        int possible_y[4];

        // north
        x_p = x-1; y_p = y;
        if(is_inside(x_p, y_p) && WORLD[x_p*C+y_p] == object){
            possible_x[P] = x_p; possible_y[P] = y_p; P++;
        }
        // east
        x_p = x; y_p = y+1;
        if(is_inside(x_p, y_p) && WORLD[x_p*C+y_p] == object){
            possible_x[P] = x_p; possible_y[P] = y_p; P++;
        }
        // south
        x_p = x+1; y_p = y;
        if(is_inside(x_p, y_p) && WORLD[x_p*C+y_p] == object){
            possible_x[P] = x_p; possible_y[P] = y_p; P++;
        }
        // west
        x_p = x; y_p = y-1;
        if(is_inside(x_p, y_p) && WORLD[x_p*C+y_p] == object){
            possible_x[P] = x_p; possible_y[P] = y_p; P++;
        }

        int chosen = (0 + x + y) % P;
        printf("chosen %d, move to (%d, %d)\n", chosen, possible_x[chosen], possible_y[chosen]);
        if(P>0){
            chosen_coords[0] = possible_x[chosen];
            chosen_coords[1] = possible_y[chosen];
            return true;
        }
        return false;
    }

    bool is_inside(int i, int j){
        if(i!=-1 && i!=R+1 && j!=-1 && j!=C+1){
            return true;
        }
        return false;
    }
};

int main(){
    Ecosystem E;
    E.print_world();
    E.move_rabbits();
    return 0;
}
