#include <stdio.h>
#include <stdlib.h>  // malloc

struct Tuple{
    int first;
    int second;
};

void dfs(int k1, int visited[], int V[], int lengthV, struct Tuple *E, int lengthE){
    visited[k1] = 1;
    for(int e=0; e<lengthE; e++){
        if(E[e].first-1 == k1){
            int p = E[e].second-1; 
            if(visited[p] == 0){
                dfs(p, visited, V, lengthV, E, lengthE);
            }
        }
        if (E[e].second - 1 == k1) { 
            int p = E[e].first - 1;
            if (visited[p] == 0) {
                dfs(p, visited, V, lengthV, E, lengthE);
            }
        }
    }
}

int my_no_connected_components(int V[], int lengthV, struct Tuple* E, int lengthE){
    int ncc = 0;

    // create visited array
    int *visited = malloc(lengthV * sizeof(int)); // Allocate memory for the visited array
    if (visited == NULL) { // Handle memory allocation error if necessary
        return -1; // Indicate an error
    }
    for(int i=0; i<lengthV; i++){
        visited[i] = 0;
    }
    // 
    for(int k=0; k<lengthV; k++){
        if(visited[k] == 0){
            dfs(k, visited, V, lengthV, E, lengthE);
            ncc++;
        }
    }
    return ncc;
}
