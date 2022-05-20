//
// Created by My PC on 03/05/2022.
//

#include "Header/Tetrimino.h"

/***
    row: the row position which element is in
    col: the column position which element is in
    rotation: status of the piece
***/
int Tetrimino::get_data_value(const int &row, const int &col,
                              const int &rotation) const {
    switch(rotation) {
        case 0:
            return data[side * row + col];
        case 1:
            return data[side * (side - col - 1) + row];
        case 2:
            return data[side * (side - row - 1) + (side - col - 1)];
        case 3:
            return data[side * col + (side - row - 1)];
    }

    return 0;
}

Tetrimino::~Tetrimino() {
    //do nothing
}
