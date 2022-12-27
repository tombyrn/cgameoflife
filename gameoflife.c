#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

#define true 1
#define false 0
#define bool int

#define SLEEP_TIME 1000000 * 1 // microseconds

typedef struct cell{
    int value;
    bool alive;
    // cells neighbors (null if out of bounds);
    /*
        tl tm tr
        ml __ mr
        bl bm br
    */
    struct cell *tl, *tm, *tr, *ml, *mr, *bl, *bm, *br;
} cell;

#define WIDTH 220
#define HEIGHT 70
#define SEED_AMT (WIDTH*HEIGHT)/8

cell cell_matrix[HEIGHT][WIDTH] = {0};
char buffer[HEIGHT][WIDTH] = {0};

const char bgChar = '#';

void bring_cell_to_life(cell* c){
    int rand_char = rand() % 3;

    if(rand_char == 0)
        (*c).value = 'O';
    if(rand_char == 1)
        (*c).value = '#';
    if(rand_char == 2)
        (*c).value = '$';
    (*c).alive = true;
}

void kill_cell(cell* c){
    (*c).value = ' ';
    (*c).alive = false;
}

void handle_neighbors(cell* c, int i, int j){

    // set top left neighbor cell
    if(i-1 < 0 && j-1 < 0)
        (*c).tl  = NULL;
    else
        (*c).tl = &(cell_matrix[i-1][j-1]);

    // set top middle neighbor cell
    if(i-1 < 0)
        (*c).tm  = NULL;
    else
        (*c).tm = &(cell_matrix[i-1][j]);

    // set top right neighbor cell
    if(i-1 < 0 && j+1 >= WIDTH)
        (*c).tr  = NULL;
    else
        (*c).tr = &(cell_matrix[i-1][j+1]);

    // set middle left neighbor cell
    if(j-1 < 0)
        (*c).ml  = NULL;
    else
        (*c).ml = &(cell_matrix[i][j-1]);

    // set middle right neighbor cell
    if(j+1 >= WIDTH)
        (*c).mr  = NULL;
    else
        (*c).mr = &(cell_matrix[i][j+1]);

    // set bottom left neighbor cell
    if(i+1 >= HEIGHT && j-1 < 0)
        (*c).bl  = NULL;
    else
        (*c).bl = &(cell_matrix[i+1][j-1]);

    // set top middle neighbor cell
    if(i+1 >= HEIGHT)
        (*c).bm  = NULL;
    else
        (*c).bm = &(cell_matrix[i+1][j]);

    // set top right neighbor cell
    if(i+1 >= HEIGHT && j+1 >= WIDTH)
        (*c).br  = NULL;
    else
        (*c).br = &(cell_matrix[i+1][j+1]);
    
}

int living_neighbors(cell* c, int i, int j){
    int count =  0;

    // set top left neighbor cell
    if((i-1 >= 0 || j-1 >= 0) && cell_matrix[i-1][j-1].alive)
        count++;

    // set top middle neighbor cell
    if((i-1 >= 0) && cell_matrix[i-1][j].alive)
        count++;

    // set top right neighbor cell
    if((i-1 >= 0 || j+1 < WIDTH) && cell_matrix[i-1][j+1].alive)
        count++;

    // set middle left neighbor cell
    if((j-1 >= 0) && cell_matrix[i][j-1].alive)
        count++;

    // set middle right neighbor cell
    if((j+1 < WIDTH) && cell_matrix[i][j+1].alive)
        count++;
    // set bottom left neighbor cell
    if((i+1 < HEIGHT || j-1 >= 0) && cell_matrix[i+1][j-1].alive)
        count++;

    // set top middle neighbor cell
    if((i+1 < HEIGHT) && cell_matrix[i+1][j].alive)
        count++;

    // set top right neighbor cell
    if((i+1 < HEIGHT || j+1 < WIDTH) && cell_matrix[i+1][j+1].alive)
        count++;
        // (*c).br = &(cell_matrix[i+1][j+1]);

    return count;
    
}

void init_grid(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            kill_cell(&(cell_matrix[i][j]));
            handle_neighbors(&(cell_matrix[i][j]), i, j);
        }
    }
}

void draw_grid(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(&(cell_matrix[i][j].value) !=  NULL)
                buffer[i][j] = cell_matrix[i][j].value;
            putchar(buffer[i][j]);
        }
        putchar('\n');
    }
    usleep(SLEEP_TIME);
}

void seed(){
    for(int c = 0; c < SEED_AMT; c++){
        int i = rand() % HEIGHT;
        int j = rand() % WIDTH;
        // printf("%d\t%d\n", i, j);
        bring_cell_to_life(&(cell_matrix[i][j]));
    }
}

void next_generation(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            int live_neighbors = living_neighbors(&(cell_matrix[i][j]), i, j);
            if(cell_matrix[i][j].alive){
                if(live_neighbors < 2 || live_neighbors > 3)
                    kill_cell(&(cell_matrix[i][j]));
                // if(live_neighbors == 2 ||  live_neighbors == 3)
                //     bring_cell_to_life(&(cell_matrix[i][j]));
                // if(live_neighbors > 3)
            }
            else{
                if(live_neighbors == 3)
                    bring_cell_to_life(&(cell_matrix[i][j]));
            }
        }
    }
}

int main() {

    srand(time(NULL));

    init_grid();
    seed();


    while(1){
        printf("\x1b[2J\x1b[H");
        draw_grid();
        next_generation();
    }

}