//
// Created by My PC on 03/05/2022.
//

#ifndef TETRIS_PROJECT_PIECE_H
#define TETRIS_PROJECT_PIECE_H


#include "common.h"
class Piece {
public:
    int tetrimino_index; //the tetrimino's index will assign what the type of
                         //pieces it is (I,O,T,S,Z,L,J)
    int offset_row; //the real row's position on the board matrix
    int offset_col; //the real col's position on the board matrix
    int rotation; //rotation just allows value from 0 to 3, it is the status of the piece,
                  //the piece has 4 status when we rotate it


    //constructor
    Piece();

    //overloading =
    void operator=(Piece* other_piece);
};


#endif //TETRIS_PROJECT_PIECE_H
