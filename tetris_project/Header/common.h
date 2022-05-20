//
// Created by My PC on 01/05/2022.
//

#ifndef TETRIS_PROJECT_COMMON_H
#define TETRIS_PROJECT_COMMON_H


#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>

//files
#include "Tetrimino.h"

#define SCREEN_WIDTH 30
#define SCREEN_HEIGHT 26
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 22

#define VISIBLE_HEIGHT 20
#define BLOCKW 30
#define BLOCKH 30

//some colors
const SDL_Color white = {255,255,255,255};
const SDL_Color black = {0,0,0,255};


/* ways to render string:
  - render string to the center of the position is given
  - render to the right side og the position
 */
enum RENDER_STRING_WAY {
    CENTER,
    RIGHT,
};


//time to the next drop for each level
const double DROP_FRAMES_PER_LEVEL[] = {
        48,
        43,
        38,
        33,
        28,
        23,
        18,
        13,
        8,
        6,
        5,5,5,
        4,4,4,
        3,3,3,
        2,2,2,2,2,2,2,2,2,2,
        1
};



//status of the game
enum GAME_STATUS  {
        GAME_PLAY,
        GAME_HIGHLIGHT_LINE,
        GAME_OVER,
        GAME_START,
        GAME_ON_MENU,
        GAME_ON_PAUSE,
        GAME_EXIT,
};


//input class for handling keyboard
class Game_input {
public:
    int left;
    int right;
    int up;
    int down;
    int space;
    int mEnter;

    //while the value of status is greater than 0, it means they are pressed
    int left_status;
    int right_status;
    int down_status;
    int up_status;
    int space_status;
    int mEnter_status;

    //constructor
    Game_input() {
        int left = 0;
        int right = 0;
        int up = 0;
        int down = 0;
        int space = 0;
        int shift = 0;

        int left_status = 0;
        int right_status = 0;
        int down_status = 0;
        int up_status = 0;
        int space_status = 0;
        int shift_status = 0;
    }
};


//piece matrix
const int TETRIMINO_1[] = {
        0,0,0,0,
        1,1,1,1,
        0,0,0,0,
        0,0,0,0
};

const int TETRIMINO_2[] = {
        2,2,
        2,2
};

const int TETRIMINO_3[] = {
        0,0,0,
        3,3,3,
        0,3,0,
};

const int TETRIMINO_4[] = {
        0,4,4,
        4,4,0,
        0,0,0
};

const int TETRIMINO_5[] = {
        5,5,0,
        0,5,5,
        0,0,0
};

const int TETRIMINO_6[] = {
        6,0,0,
        6,6,6,
        0,0,0
};

const int TETRIMINO_7[] = {
        0,0,7,
        7,7,7,
        0,0,0
};


//Tetrimino includes matrix of the piece and the side of it
Tetrimino tetrimino(const int *data, const int&side);
const Tetrimino TETRIMINOS[] = {
        tetrimino(TETRIMINO_1,4),
        tetrimino(TETRIMINO_2,2),
        tetrimino(TETRIMINO_3,3),
        tetrimino(TETRIMINO_4,3),
        tetrimino(TETRIMINO_5,3),
        tetrimino(TETRIMINO_6,3),
        tetrimino(TETRIMINO_7,3),
};


/***
    blocks' path:
    - These string paths are used to render the blocks in board game.
    - These string are about the path to images
 ***/
const std::string path[7] = {
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Single Blocks\\LightBlue.png",
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Single Blocks\\Yellow.png",
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Single Blocks\\Purple.png",
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Single Blocks\\Green.png",
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Single Blocks\\Red.png",
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Single Blocks\\Orange.png",
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Single Blocks\\Blue.png"
};


/***
  outside block's path:
 - For every image about piece which is rendered outside the board game.
 - In this case, program use it to render piece on hold board and next piece board
 ***/
const std::string outside_block_path[7] = {
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Shape Blocks\\I.png",
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Shape Blocks\\O.png",
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Shape Blocks\\T.png",
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Shape Blocks\\S.png",
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Shape Blocks\\Z.png",
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Shape Blocks\\L.png",
        "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Shape Blocks\\J.png",
};


/***
     this string path is used to render shadow piece feature.
***/
const std::string shadow_block = "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Single Blocks\\TileEmpty.png";


/***
     this path is used to render the disappeared feature
     when the line is cleared result in player has filled entire of the row
 ***/
const std::string duplicate_star_path = "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Single Blocks\\duplicate_star.png";


/***
     Some funtion related to handle matrix because instead of using 2 dimensions matrix,
     program uses 1 dimensions and need some functions to present it like 2 dimensions matrix

    `get_date_value(...): get the element's value of the matrix
     set_date_value(...): set the element's value of the matrix
     get_date_value(...): copy matrix to another matrix
***/
int get_data_value(const int *data, const int &width,
                    const int &row, const int&col);
void set_data_value(int *data, const int &width,
                    const int &row, const int &colm ,const int &value );
void copyMatrix(const int *matrix, int *copyMatrix, int height, int width);


/***
    Basic function to get a random number from the limitation is given
***/
int get_random_number(const int &min, const int &max);

//render string on the screen
void render_string(const std::string& str,
                   const int &size,
                   const int &way,
                   const SDL_Color& color,
                   const int &offset_x, const int &offset_y,
                   TTF_Font* gFont,
                   SDL_Renderer* renderer);


#endif //TETRIS_PROJECT_COMMON_H


