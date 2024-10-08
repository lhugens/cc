#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#define NTHREADS 4

char min(char a, char b){
    if(a - '0' < b - '0'){
        return a;
    }
    return b;
}

char max(char a, char b){
    if(a - '0' > b - '0'){
        return a;
    }
    return b;
}

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
        bool selected;
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                if(WORLD[i*C+j] == 'R' && (i!=coords[0] || j!=coords[1])){
                    selected = false;
                    PROC[i*C+j]++;
                    //printf("R @ (%d, %d)\n", i, j);
                    if ((selected = select_adjacent_cell(i, j, '_', coords))){
                        if(NEW_WORLD[coords[0]*C+coords[1]] == 'R'){
                            //printf("NEW_WORLD[%d, %d]=%c\n", coords[0], coords[1], NEW_WORLD[coords[0]*C+coords[1]]);
                            //printf("R @ (%d, %d), PROC %c, found R @ (%d, %d)\n", i, j, PROC[i*C+j], coords[0], coords[1]);
                            NEW_WORLD[i*C+j] = '_';
                            PROC[coords[0]*C+coords[1]] = max(PROC[coords[0]*C+coords[1]], PROC[i*C+j]);
                            PROC[i*C+j] = '_';
                            FOOD[coords[0]*C+coords[1]] = FOOD[i*C+j];
                            FOOD[i*C+j] = '_';
                            //printf("R @ (%d, %d), move to (%d, %d)\n", i, j, coords[0], coords[1]);
                            //print_new_world();
                            //printf("\n");
                        } else {
                            //printf("selected = %d, NEW_WORLD[%d, %d]=%c\n", selected, coords[0], coords[1], NEW_WORLD[coords[0]*C+coords[1]]);
                            //printf("R @ (%d, %d), PROC %c, not fond R @ (%d, %d)\n", i, j, PROC[i*C+j], coords[0], coords[1]);
                            NEW_WORLD[i*C+j] = '_';
                            NEW_WORLD[coords[0]*C+coords[1]] = 'R';
                            PROC[coords[0]*C+coords[1]] = PROC[i*C+j];
                            PROC[i*C+j] = '_';
                            FOOD[coords[0]*C+coords[1]] = FOOD[i*C+j];
                            FOOD[i*C+j] = '_';
                            //printf("R @ (%d, %d), move to (%d, %d), PROC=%c\n", i, j, coords[0], coords[1], PROC[coords[0]*C+coords[1]]);
                            //printf("R @ (%d, %d), move to (%d, %d)\n", i, j, coords[0], coords[1]);
                            //print_new_world();
                            //printf("\n");
                        }
                        //if(selected && PROC[coords[0]*C+coords[1]] - '0' == GEN_PROC_RABBITS+1){
                        if(PROC[coords[0]*C+coords[1]] - '0' > GEN_PROC_RABBITS){
                            //printf("R @ (%d, %d), moved to (%d, %d), and left R @ (%d, %d)\n", i, j, coords[0], coords[1], i, j);
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
        bool selected_rabbit;
        bool selected_empty;
        bool died;
        for(int i=0; i<R; i++){
            for(int j=0; j<C; j++){
                //printf("%c @ (%d, %d), coords=(%d, %d)\n", WORLD[i*C+j], i, j, coords[0], coords[1]);
                if(WORLD[i*C+j] == 'F' && (i!=coords[0] || j!=coords[1])){
                    died = false;
                    selected_rabbit = false;
                    selected_empty = false;
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
                        //printf("F @ (%d, %d) died of hunger %c>%d\n", i, j, FOOD[i*C+j], GEN_FOOD_FOXES);
                        died = true;
                        NEW_WORLD[i*C+j] = '_';
                        PROC[i*C+j] = '_';
                        FOOD[i*C+j] = '_';
                    } else if ((selected_empty = select_adjacent_cell(i, j, '_', coords))){
                        if(NEW_WORLD[coords[0]*C+coords[1]] == 'F'){
                            //printf("F @ (%d, %d), FOOD %c, fond F @ (%d, %d)\n", i, j, FOOD[i*C+j], coords[0], coords[1]);
                            NEW_WORLD[i*C+j] = '_';
                            PROC[coords[0]*C+coords[1]] = PROC[i*C+j];
                            PROC[i*C+j] = '_';
                            FOOD[coords[0]*C+coords[1]] = min(FOOD[coords[0]*C+coords[1]], FOOD[i*C+j]);
                            FOOD[i*C+j] = '_';
                            //printf("F @ (%d, %d), move to (%d, %d)\n", i, j, coords[0], coords[1]);
                            //print_new_world();
                            //printf("\n");
                        } else {
                            //printf("F @ (%d, %d), FOOD %c, not fond F @ (%d, %d)\n", i, j, FOOD[i*C+j], coords[0], coords[1]);
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
                    if((selected_rabbit || selected_empty) && !died){
                        if(PROC[coords[0]*C+coords[1]] - '0' > GEN_PROC_FOXES){
                            //printf("F moved to (%d, %d), and left F @ (%d, %d)\n", coords[0], coords[1], i, j);
                            NEW_WORLD[i*C+j] = 'F';
                            PROC[i*C+j] = '0';
                            PROC[coords[0]*C+coords[1]] = '0';
                            FOOD[i*C+j] = '0';
                        }
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
        int tid;
        #pragma omp parallel num_threads(NTHREADS) default(none) private(tid)
        {
            //#pragma single nowait
            //printf("R=%d, C=%d\n", R, C);

            ////////////////tid = omp_get_thread_num();
            #pragma omp for
            for(int i=0; i<R; i++){
                //printf("T%d, updating row%d\n", tid, i);
                for(int j=0; j<C; j++){
                    WORLD[i*C+j] = NEW_WORLD[i*C+j];
                }
                //printf("T%d terminated\n", tid);
            }
        }
        //printf("FINISHED UPDATE\n\n");
    }
};

int main(){
    int n=NTHREADS;
    int tid;
    Ecosystem E;
    printf("Generation %d\n\n", E.G);
    E.print_all();
    //E.print_world();
    //E.print_proc();
    //E.print_food();
    printf("\n__________________________\n");

    for(int g=0; g<1; g++){

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
