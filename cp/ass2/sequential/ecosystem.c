#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct Info {
    int GEN_PROC_RABBITS;
    int GEN_PROC_FOXES;
    int GEN_FOOD_FOXES;
    int N_GEN;
    int R;
    int C;
    int N;
    int G;
};

struct Object {
    char name;
    char proc;
    char food;
    int x;
    int y;
    int moved;
};

void print_world(struct Info *I, struct Object *BOARD){
    for(int i=0; i<I->R; i++){
        printf(" | ");
        for(int j=0; j<I->C; j++)
            printf("%c ", BOARD[i*I->C+j].name);
        printf(" | ");
        for(int j=0; j<I->C; j++)
            printf("%c ", BOARD[i*I->C+j].proc);
        printf(" | ");
        for(int j=0; j<I->C; j++)
            printf("%c ", BOARD[i*I->C+j].food);
        printf(" | \n");
    }
}

void update_world(struct Info *I, struct Object *WORLD, struct Object *NEW_WORLD){
    for(int i=0; i<I->R; i++)
        for(int j=0; j<I->C; j++)
            WORLD[i*I->C+j] = NEW_WORLD[i*I->C+j];
}

int is_inside(int i, int j, struct Info *I){
    if(i!=-1 && i!=I->R && j!=-1 && j!=I->C){
        return 1;
    }
    return 0;
} 


int select_adj(int X, int Y, char obj, struct Info *I, struct Object *WORLD){
    int P=0;
    int x_p;
    int y_p;
    int possible_x[4];
    int possible_y[4];

    //printf("G=%d\n", I->G);
    // north
    //printf("north (%d,%d)\n", X, Y);
    x_p = X-1; 
    y_p = Y;
    if(is_inside(x_p, y_p, I) && WORLD[x_p*I->C+y_p].name == obj){
        //printf("(%d, %d) is_inside %d\n", x_p, y_p, is_inside(x_p, y_p, I));
        //printf("WORLD object is %c, object is %c\n", WORLD[x_p*I->C+y_p].name, obj);
        //printf("Found north\n");
        possible_x[P] = x_p; possible_y[P] = y_p; P++;
    }
    // east
    //printf("east (%d,%d)\n", X, Y);
    x_p = X; 
    y_p = Y+1;
    if(is_inside(x_p, y_p, I) && WORLD[x_p*I->C+y_p].name == obj){
        //printf("(%d, %d) is_inside %d\n", x_p, y_p, is_inside(x_p, y_p, I));
        //printf("WORLD object is %c, object is %c\n", WORLD[x_p*I->C+y_p].name, obj);
        //printf("Found east\n");
        possible_x[P] = x_p; possible_y[P] = y_p; P++;
    }
    // south
    //printf("south (%d,%d)\n", X, Y);
    x_p = X+1; 
    y_p = Y;
    if(is_inside(x_p, y_p, I) && WORLD[x_p*I->C+y_p].name == obj){
        //printf("(%d, %d) is_inside %d\n", x_p, y_p, is_inside(x_p, y_p, I));
        //printf("WORLD object is %c, object is %c\n", WORLD[x_p*I->C+y_p].name, obj);
        //printf("Found south\n");
        possible_x[P] = x_p; possible_y[P] = y_p; P++;
    }
    // west
    //printf("west (%d,%d)\n", X, Y);
    x_p = X; 
    y_p = Y-1;
    if(is_inside(x_p, y_p, I) && WORLD[x_p*I->C+y_p].name == obj){
        //printf("after (%d,%d)\n", x_p, y_p);
        //printf("(%d, %d) is_inside %d\n", x_p, y_p, is_inside(x_p, y_p, I));
        //printf("WORLD object is %c, object is %c\n", WORLD[x_p*I->C+y_p].name, obj);
        //printf("Found west\n");
        possible_x[P] = x_p; 
        possible_y[P] = y_p; 
        P++;
        //printf("(%d,%d)\n", X, Y, x_p, y_p);
    }

    if(P>0){
        //printf("P=%d\n", P);
        int chosen = (I->G + X + Y) % P;
        WORLD[X*I->C+Y].x = possible_x[chosen];
        WORLD[X*I->C+Y].y = possible_y[chosen];
        //printf("R @ (%d,%d) moves to (%d,%d)\n", X, Y, possible_x[chosen], possible_y[chosen]);
        //printf("R @ (%d,%d) moves to (%d,%d)\n", X, Y, WORLD[X*I->C+Y].x, WORLD[X*I->C+Y].y);
        return 1;
    }
    return 0;
}

void move_rabbits(struct Info *I, struct Object *WORLD, struct Object *NEW_WORLD){
    // move 
    for(int i=0; i<I->R; i++){
        for(int j=0; j<I->C; j++){
            if(WORLD[i*I->C+j].name == 'R'){
                //printf("Found rabbit at (%d, %d)\n", i, j);
                //printf("(%d, %d) is_inside=%d\n", i, j, is_inside(I, i, j));
                if (select_adj(i, j, '_', I, WORLD)){
                    printf("R @ (%d,%d) moves to (%d,%d)\n", i, j, WORLD[i*I->C+j].x, WORLD[i*I->C+j].y);
                    NEW_WORLD[i*I->C+j].name = '_';
                    NEW_WORLD[i*I->C+j].proc = '_';
                    NEW_WORLD[i*I->C+j].food = '_';
                    NEW_WORLD[WORLD[i*I->C+j].x*I->C+WORLD[i*I->C+j].y].name = 'R';
                    NEW_WORLD[WORLD[i*I->C+j].x*I->C+WORLD[i*I->C+j].y].food = 'R';
                    NEW_WORLD[WORLD[i*I->C+j].x*I->C+WORLD[i*I->C+j].y].moved = 1;
                }
            }
        }
    }
    // proc
    int x, y, count;
    char *r_proc, r_survivor;
    count = 0;
    r_proc = (char*) malloc(4*sizeof(char));

    for(int i=0; i<4; i++)
        r_proc[i] = '0';

    for(int i=0; i<I->R; i++){
        for(int j=0; j<I->C; j++){
            if(NEW_WORLD[i*I->C+j].name == 'R'){
                if(NEW_WORLD[i*I->C+j].moved){
                    printf("hey\n");
                    x = i-1; y = j; //north
                    if(is_inside(x, y, I) && WORLD[x*I->C+y].name=='R'){
                        r_proc[count] = WORLD[x*I->C+y].proc; count++;
                    }
                    x = i; y = j+1; //east
                    if(is_inside(x, y, I) && WORLD[x*I->C+y].name=='R'){
                        r_proc[count] = WORLD[x*I->C+y].proc; count++;
                    }
                    x = i+1; y = j; //south
                    if(is_inside(x, y, I) && WORLD[x*I->C+y].name=='R'){
                        r_proc[count] = WORLD[x*I->C+y].proc; count++;
                    }
                    x = i; y = j-1; //west
                    if(is_inside(x, y, I) && WORLD[x*I->C+y].name=='R'){
                        r_proc[count] = WORLD[x*I->C+y].proc; count++;
                    }
                    r_survivor = '0';
                    for(int k=0; k<4; k++){
                        r_survivor = max(r_proc[k], r_survivor);
                    }
                    //r_survivor++;
                    NEW_WORLD[i*I->C+j].proc = ++r_survivor;
                    printf("r_survivor %c\n", r_survivor);
                } else {
                    NEW_WORLD[i*I->C+j].proc++;
                }
            }
        }
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
    struct Object *NEW_WORLD;
    struct Info *I;

    I = (struct Info*) malloc(sizeof(struct Info));

    scanf("%d", &I->GEN_PROC_RABBITS);
    scanf("%d", &I->GEN_PROC_FOXES);
    scanf("%d", &I->GEN_FOOD_FOXES);
    scanf("%d", &I->N_GEN);
    scanf("%d", &I->R);
    scanf("%d", &I->C);
    scanf("%d", &I->N);
    I->G = 0;

    printf("%d ", I->GEN_PROC_RABBITS);
    printf("%d ", I->GEN_PROC_FOXES);
    printf("%d ", I->GEN_FOOD_FOXES);
    printf("%d ", I->N_GEN);
    printf("%d ", I->R);
    printf("%d ", I->C);
    printf("%d ", I->N);
    printf("\n");

    WORLD = (struct Object*) malloc(I->R*I->C*sizeof(struct Object));
    NEW_WORLD = (struct Object*) malloc(I->R*I->C*sizeof(struct Object));

    for(int i=0; i<I->R; i++){
        for(int j=0; j<I->C; j++){
            WORLD[i*I->C+j].name = '_';
            WORLD[i*I->C+j].proc = '_';
            WORLD[i*I->C+j].food = '_';
            WORLD[i*I->C+j].x = -1;
            WORLD[i*I->C+j].y = -1;
            WORLD[i*I->C+j].moved = 0;
            NEW_WORLD[i*I->C+j].name = '_';
            NEW_WORLD[i*I->C+j].proc = '_';
            NEW_WORLD[i*I->C+j].food = '_';
            NEW_WORLD[i*I->C+j].x = -1;
            NEW_WORLD[i*I->C+j].y = -1;
            NEW_WORLD[i*I->C+j].moved = 0;
        }
    }
    //struct Object WORLD[3];
    //WORLD[0].name = 'l';
    //printf("%c\n", WORLD[0].name);

    char* name = (char *) malloc(6*sizeof(char));
    int x;
    int y;

    for(int n=0; n<I->N; n++){
        scanf("%s %d %d", name, &x, &y);
        if(strcmp(name, "ROCK") == 0){
            WORLD[x*I->C+y].name = '*';
            WORLD[x*I->C+y].proc = '*';
            WORLD[x*I->C+y].food = '*';
            NEW_WORLD[x*I->C+y].name = '*';
            NEW_WORLD[x*I->C+y].proc = '*';
            NEW_WORLD[x*I->C+y].food = '*';
        } else if(strcmp(name, "RABBIT") == 0){
            WORLD[x*I->C+y].name = 'R';
            WORLD[x*I->C+y].proc = '0';
            WORLD[x*I->C+y].food = 'R';
            NEW_WORLD[x*I->C+y].name = 'R';
            NEW_WORLD[x*I->C+y].proc = '0';
            NEW_WORLD[x*I->C+y].food = 'R';
        } else if(strcmp(name, "FOX") == 0){
            WORLD[x*I->C+y].name = 'F';
            WORLD[x*I->C+y].proc = '0';
            WORLD[x*I->C+y].food = '0';
            NEW_WORLD[x*I->C+y].name = 'F';
            NEW_WORLD[x*I->C+y].proc = '0';
            NEW_WORLD[x*I->C+y].food = '0';
        } 
    }
    free(name);

    print_world(I, WORLD);

    move_rabbits(I, WORLD, NEW_WORLD);

    print_world(I, NEW_WORLD);
    
}
