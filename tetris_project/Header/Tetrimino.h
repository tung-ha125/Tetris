//
// Created by My PC on 03/05/2022.
//

#ifndef TETRIS_PROJECT_TETRIMINO_H
#define TETRIS_PROJECT_TETRIMINO_H

class Tetrimino {
public:
    const int *data; //matrix of the tetrimino
    const int side;  //side of the tetrimino (length of data = side * side)

    /***
        return value of data's element
    ***/
    int get_data_value( const int& row, const int& col,
                        const int &rotation) const;

    //desturtor
    ~Tetrimino();
};




#endif //TETRIS_PROJECT_TETRIMINO_H
