#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

#define true 1
#define false 0
#define bool int

#define SLEEP_TIME 1
#define WIDTH 200
#define HEIGHT 60
#define SEED_AMT (WIDTH*HEIGHT)/20

typedef struct cell{
    char value;
    bool alive;
    // cells neighbors (null if out of bounds);
    /*
        tl tm tr
        ml __ mr
        bl bm br
    */
    struct cell *tl, *tm, *tr, *ml, *mr, *bl, *bm, *br;
} cell;



cell cell_matrix[HEIGHT][WIDTH] = {0};

const char bgChar = '#';

// set cell to alive
void bring_cell_to_life(cell* c){
    int rand_char = (rand() % 93)+33; // generate 33-126 in ascii

    (*c).value = rand_char;
    (*c).alive = true;
}

// set cell to dead
void kill_cell(cell* c){
    (*c).value = ' ';
    (*c).alive = false;
}

// checks if neigboring cells around *c exist, if they don't set their pointer to NULL
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

    // count upper row of live neighbors
    if((i-1 >= 0 && j-1 >= 0) && cell_matrix[i-1][j-1].alive)
        count++;


    if((i-1 >= 0) && cell_matrix[i-1][j].alive)
        count++;


    if((i-1 >= 0 && j+1 < WIDTH) && cell_matrix[i-1][j+1].alive)
        count++;



    // count live cells to left or right
    if((j-1 >= 0) && cell_matrix[i][j-1].alive)
        count++;

    if((j+1 < WIDTH) && cell_matrix[i][j+1].alive)
        count++;



    // count bottom row of live neighbors
    if((i+1 < HEIGHT && j-1 >= 0) && cell_matrix[i+1][j-1].alive)
        count++;

    if((i+1 < HEIGHT) && cell_matrix[i+1][j].alive)
        count++;

    if((i+1 < HEIGHT && j+1 < WIDTH) && cell_matrix[i+1][j+1].alive)
        count++;

    return count;
    
}

// initialize every cell in cell_matrix to dead and set their neighbors
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
            // if(&(cell_matrix[i][j].value) !=  NULL)
            //     buffer[i][j] = cell_matrix[i][j].value;
            putchar(cell_matrix[i][j].value);
        }
        putchar('\n');
    }
    sleep(SLEEP_TIME);
}

void seed(){
    for(int c = 0; c < SEED_AMT; c++){
        int i = rand() % HEIGHT;
        int j = rand() % WIDTH;

        bring_cell_to_life(&(cell_matrix[i][j]));
    }
}
/*
    1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    2. Any live cell with two or three live neighbours lives on to the next generation.
    3. Any live cell with more than three live neighbours dies, as if by overpopulation.
    4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/
void next_generation(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            int live_neighbors = living_neighbors(&(cell_matrix[i][j]), i, j);
            if(cell_matrix[i][j].alive){
                // Rules 1-3
                if(live_neighbors < 2 || live_neighbors > 3)
                    kill_cell(&(cell_matrix[i][j]));
            }
            // Rule 4s
            else if(live_neighbors == 3){
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