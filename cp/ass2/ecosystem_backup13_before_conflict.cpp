#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#define min(a, b) a < b ? a : b

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
    char* PROC;
    char* FOOD;

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
        PROC = (char*) malloc(R*C*sizeof(char));
        FOOD = (char*) malloc(R*C*sizeof(char));

        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                WORLD[i*R+j] = '_';
                NEW_WORLD[i*R+j] = '_';
                PROC[i*R+j] = '_';
                FOOD[i*R+j] = '_';
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
                PROC[x*R+y] = '*';
                FOOD[x*R+y] = '*';
            } else if(strcmp(name, "RABBIT") == 0){
                WORLD[x*R+y] = 'R';
                NEW_WORLD[x*R+y] = 'R';
                PROC[x*R+y] = '0';
                FOOD[x*R+y] = 'R';
            } else if(strcmp(name, "FOX") == 0){
                WORLD[x*R+y] = 'F';
                NEW_WORLD[x*R+y] = 'F';
                PROC[x*R+y] = '0';
                FOOD[x*R+y] = '0';
            }
        }

        G = 0;
    }

    void print_world(){
        printf("WORLD\n");
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

    void print_proc(){
        printf("PROC\n");
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                printf("%c ", PROC[i*C+j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    void print_food(){
        printf("FOOD\n");
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                printf("%c ", FOOD[i*C+j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    
    //void print_all(){
    //    print_world();
    //    print_proc();
    //    print_food();
    //}

    void print_all(){
        for(int i=0; i<R; i++){
            printf("  |  ");
            for(int j=0; j<C; j++){
                printf("%c ", WORLD[i*C+j]);
            }
            printf("  |  ");
            for(int j=0; j<C; j++){
                printf("%c ", PROC[i*C+j]);
            }
            printf("  |  ");
            for(int j=0; j<C; j++){
                printf("%c ", FOOD[i*C+j]);
            }
            printf("  |  ");
            printf("\n");
        }
        printf("\n");

    }

    void move_rabbits(){
        int *coords;
        coords = (int*) malloc(2*sizeof(int));
        coords[0] = -1;
        coords[1] = -1;
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                if(WORLD[i*C+j] == 'R' && (i!=coords[0] || j!=coords[1])){
                    PROC[i*C+j]++;
                    //printf("R @ (%d, %d)\n", i, j);
                    if (select_adjacent_cell(i, j, '_', coords)){
                        NEW_WORLD[i*C+j] = '_';
                        NEW_WORLD[coords[0]*C+coords[1]] = 'R';
                        PROC[coords[0]*C+coords[1]] = PROC[i*C+j];
                        PROC[i*C+j] = '_';
                        FOOD[coords[0]*C+coords[1]] = FOOD[i*C+j];
                        FOOD[i*C+j] = '_';
                        if(PROC[coords[0]*C+coords[1]] - '0' == GEN_PROC_RABBITS+1){
                            //printf("R moved to (%d, %d), and left R @ (%d, %d)\n", coords[0], coords[1], i, j);
                            NEW_WORLD[i*C+j] = 'R';
                            PROC[i*C+j] = '0';
                            PROC[coords[0]*C+coords[1]] = '0';
                            FOOD[i*C+j] = 'R';
                        }
                        //printf("R @ (%d, %d), move to (%d, %d)\n", i, j, coords[0], coords[1]);
                        //print_new_world();
                        //printf("\n");
                    }
                }
            }
        }
        free(coords);
    }

    void move_foxes(){
        int *coords;
        coords = (int*) malloc(2*sizeof(int));
        coords[0] = -1;
        coords[1] = -1;
        bool selected_rabbit = false;
        bool selected_empty = false;
        bool died;
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                //printf("%c @ (%d, %d), coords=(%d, %d)\n", WORLD[i*C+j], i, j, coords[0], coords[1]);
                if(WORLD[i*C+j] == 'F' && (i!=coords[0] || j!=coords[1])){
                    died = false;
                    PROC[i*C+j]++;
                    FOOD[i*C+j]++;
                    if ((selected_rabbit = select_adjacent_cell(i, j, 'R', coords))){
                        // eat the rabbit
                        NEW_WORLD[i*C+j] = '_';
                        NEW_WORLD[coords[0]*C+coords[1]] = 'F';
                        PROC[coords[0]*C+coords[1]] = PROC[i*C+j];
                        PROC[i*C+j] = '_';
                        FOOD[coords[0]*C+coords[1]] = '0';
                        FOOD[i*C+j] = '_';
                        //printf("F @ (%d, %d), ate R @ (%d, %d)\n", i, j, coords[0], coords[1]);
                        //print_new_world();
                        //printf("\n");
                    } else if (FOOD[i*C+j] - '0' == GEN_FOOD_FOXES){
                        printf("F @ (%d, %d) died of hunger %c>%d\n", i, j, FOOD[i*C+j], GEN_FOOD_FOXES);
                        died = true;
                        NEW_WORLD[i*C+j] = '_';
                        PROC[i*C+j] = '_';
                        FOOD[i*C+j] = '_';
                    } else if ((selected_empty = select_adjacent_cell(i, j, '_', coords))){
                        if(NEW_WORLD[coords[0]*C+coords[1]] == 'F'){
                            NEW_WORLD[i*C+j] = '_';
                            //NEW_WORLD[coords[0]*C+coords[1]] = 'F';
                            PROC[coords[0]*C+coords[1]] = PROC[i*C+j];
                            PROC[i*C+j] = '_';
                            FOOD[coords[0]*C+coords[1]] = FOOD[i*C+j];
                            FOOD[i*C+j] = '_';
                            //printf("F @ (%d, %d), move to (%d, %d)\n", i, j, coords[0], coords[1]);
                            //print_new_world();
                            //printf("\n");
                        } else {
                            NEW_WORLD[i*C+j] = '_';
                            NEW_WORLD[coords[0]*C+coords[1]] = 'F';
                            PROC[coords[0]*C+coords[1]] = PROC[i*C+j];
                            PROC[i*C+j] = '_';
                            FOOD[coords[0]*C+coords[1]] = FOOD[i*C+j];
                            FOOD[i*C+j] = '_';
                            //printf("F @ (%d, %d), move to (%d, %d)\n", i, j, coords[0], coords[1]);
                            //print_new_world();
                            //printf("\n");
                        }
                    }
                    //if((selected_rabbit || selected_empty) && !died && PROC[coords[0]*C+coords[1]] - '0' == GEN_PROC_FOXES+1){
                    if(!died && PROC[coords[0]*C+coords[1]] - '0' == GEN_PROC_FOXES+1){
                        printf("F moved to (%d, %d), and left F @ (%d, %d)\n", coords[0], coords[1], i, j);
                        NEW_WORLD[i*C+j] = 'F';
                        PROC[i*C+j] = '0';
                        PROC[coords[0]*C+coords[1]] = '0';
                        FOOD[i*C+j] = '0';
                    }
                }
            }
        }
        free(coords);
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
            //printf("Found north\n");
            possible_x[P] = x_p; possible_y[P] = y_p; P++;
        }
        // east
        x_p = x; y_p = y+1;
        if(is_inside(x_p, y_p) && WORLD[x_p*C+y_p] == object){
            //printf("Found east\n");
            possible_x[P] = x_p; possible_y[P] = y_p; P++;
        }
        // south
        x_p = x+1; y_p = y;
        if(is_inside(x_p, y_p) && WORLD[x_p*C+y_p] == object){
            //printf("Found south\n");
            possible_x[P] = x_p; possible_y[P] = y_p; P++;
        }
        // west
        x_p = x; y_p = y-1;
        if(is_inside(x_p, y_p) && WORLD[x_p*C+y_p] == object){
            //printf("Found west\n");
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
        if(i!=-1 && i!=R && j!=-1 && j!=C){
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
    printf("Generation %d\n\n", E.G);
    E.print_all();
    //E.print_world();
    //E.print_proc();
    //E.print_food();
    printf("\n__________________________\n");

    for(int g=0; g<E.N_GEN; g++){
    //for(int g=0; g<98; g++){
        E.move_rabbits();
        E.update_world();
        E.move_foxes();
        E.update_world();
        E.G++;
        printf("Generation %d\n\n", E.G);
        //E.print_world();
        E.print_all();
        printf("\n__________________________\n");
    }
    return 0;
}
