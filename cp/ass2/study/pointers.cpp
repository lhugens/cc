#include <stdio.h>
#include <stdlib.h>
#include <cstring>

bool f(int *coords){
    coords[0] = 0;
    coords[1] = 1;
    return true;
}

int main(){
    int *chosen;
    chosen = (int*) malloc(2*sizeof(int));

    if(f(chosen)){
        printf("chosen=(%d, %d)\n", chosen[0], chosen[1]);
    }


    return 0;
}
