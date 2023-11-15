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
    int G;
    char* WORLD;
    char* NEW_WORLD;

    Ecosystem(){
        scanf("%d", &GEN_PROC_RABBITS);
        scanf("%d", &GEN_PROC_FOXES);
        scanf("%d", &GEN_FOOD_FOXES);
        scanf("%d", &N_GEN);
        scanf("%d", &R);
        scanf("%d", &C);
        scanf("%d", &N);

        WORLD = (char*) malloc(R*C*sizeof(char));
        NEW_WORLD = (char*) malloc(R*C*sizeof(char));

        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                WORLD[i*R+j] = '_';
                NEW_WORLD[i*R+j] = '_';
            }
        }

        char name[6];
        int x;
        int y;

        for(int n=0; n<N; n++){
            scanf("%s %d %d", name, &x, &y);
            if(strcmp(name, "ROCK") == 0){
                WORLD[x*R+y] = '*';
                NEW_WORLD[x*R+y] = '*';
            } else if(strcmp(name, "RABBIT") == 0){
                WORLD[x*R+y] = 'R';
                NEW_WORLD[x*R+y] = 'R';
            } else if(strcmp(name, "FOX") == 0){
                WORLD[x*R+y] = 'F';
                NEW_WORLD[x*R+y] = 'F';
            }
        }

        G = 0;
    }

    void print_world(){
        printf("Generation %d\n", G);
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                printf("%c ", WORLD[i*C+j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    void print_new_world(){
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                printf("%c ", NEW_WORLD[i*C+j]);
            }
            printf("\n");
        }
    }

    void move_rabbits(){
        int *coords;
        coords = (int*) malloc(2*sizeof(int));
        coords[0] = -1;
        coords[1] = -1;
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                if(WORLD[i*C+j] == 'R' && i!=coords[0] && j!=coords[1]){
                    if (select_adjacent_cell(i, j, '_', coords)){
                        NEW_WORLD[i*C+j] = '_';
                        NEW_WORLD[coords[0]*C+coords[1]] = 'R';
                        //printf("R @ (%d, %d), move to (%d, %d)\n", i, j, coords[0], coords[1]);
                        //print_new_world();
                        //printf("\n");
                    }
                }
            }
        }
    }

    void move_foxes(){
        int *coords;
        coords = (int*) malloc(2*sizeof(int));
        coords[0] = -1;
        coords[1] = -1;
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                if(WORLD[i*C+j] == 'F' && i!=coords[0] && j!=coords[1]){
                    //printf("F @ (%d, %d)\n", i, j);
                    if (select_adjacent_cell(i, j, 'R', coords)){
                        NEW_WORLD[i*C+j] = '_';
                        NEW_WORLD[coords[0]*C+coords[1]] = 'F';
                        //printf("F @ (%d, %d), ate R @ (%d, %d)\n", i, j, coords[0], coords[1]);
                        //print_new_world();
                        //printf("\n");
                    } else if (select_adjacent_cell(i, j, '_', coords)){
                        NEW_WORLD[i*C+j] = '_';
                        NEW_WORLD[coords[0]*C+coords[1]] = 'F';
                        //printf("F @ (%d, %d), move to (%d, %d)\n", i, j, coords[0], coords[1]);
                        //print_new_world();
                        //printf("\n");
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

        if(P>0){
            int chosen = (G + x + y) % P;
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

    void update_world(){
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                WORLD[i*C+j] = NEW_WORLD[i*C+j];
            }
        }
    }
};

int main(){
    Ecosystem E;
    E.print_world();

    for(int g=0; g<E.N_GEN; g++){
        E.move_rabbits();
        E.move_foxes();
        E.update_world();
        E.G++;
        E.print_world();
    }
    return 0;
}
