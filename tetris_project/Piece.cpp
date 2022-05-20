//
// Created by My PC on 03/05/2022.
//

#include "Header/Piece.h"

Piece::Piece() {
    tetrimino_index = -1;
    offset_row = 0;
    offset_col = 0;
    rotation = 0;
}

void Piece::operator=(Piece* other_piece) {
    if(other_piece != nullptr) {
        tetrimino_index = other_piece->tetrimino_index;
        offset_row = other_piece->offset_row;
        offset_col = other_piece->offset_col;
        rotation = other_piece->rotation;
    }
}
