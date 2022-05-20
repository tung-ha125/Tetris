//
// Created by My PC on 01/05/2022.
//

#include "Header/common.h"

/***
    data: an array of matrix
    side: the side of matrix (length of data = side * side)
 ***/
Tetrimino tetrimino(const int *data, const int &side) {
    return {data,side};
}


/***
    data: an array of matrix
    width: the width of matrix
    row: the row location of the element we want to take value
    col: the column location of the element we want to take value
 ***/
int get_data_value(const int *data, const int &width,
                   const int &row, const int&col) {
    return data[width * row + col];
}


/***
    data: an array of matrix
    width: the width of matrix
    row: the row location of the element we want to take value
    col: the column location of the element we want to take value
    value: valuation we want to set for the element of matrix
 ***/
void set_data_value(int *data, const int &width,
                   const int &row, const int&col, const int &value) {
    data[width * row + col] = value;
}


/***
 matrix: Matrix we wan to copy
 copyMatrix: natrix which we want to copy to
 height: the height of matrix
 width: the width of matrix
 - Warning: 2 matrix have to has the same dimension(or the number of elements).
   If it is not, there might be some errors
 ****/
void copyMatrix(const int *matrix, int *copyMatrix, int height, int width) {
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i * width + j;
            copyMatrix[index] = matrix[index];
        }
    }
}


/***
    min: the bottom of limitation
    max: the top of the limitation
 ***/
int get_random_number(const int &min, const int &max) {
    return rand() % (max - min + 1) + min;
}

/***
    - render text on the screen
    str: text
    size: size of the word
    way: way to render it (CENTER or RIGHT)
    color: color of the word
    offset_x, offset_y: location to render text
    gFont: font of word
 ***/
void render_string(const std::string& str,
                           const int &size,
                           const int &way,
                           const SDL_Color& color,
                           const int &offset_x, const int &offset_y,
                           TTF_Font* gFont,
                           SDL_Renderer* renderer) {
    gFont = TTF_OpenFont("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Font\\ShortBaby-Mg2w.ttf",
                         size);
    if(gFont == nullptr) std::cout<<"Font could not be loaded. " <<TTF_GetError();
    SDL_Surface* textSurf = TTF_RenderText_Solid(gFont,str.c_str(),color);
    SDL_Texture* mText = SDL_CreateTextureFromSurface(renderer,textSurf);

    SDL_Rect rect;
    rect.y = offset_y;
    rect.h = textSurf->h;
    rect.w = textSurf->w;
    switch(way) {
        case CENTER:
            rect.x = offset_x - rect.w / 2;
            break;
        case RIGHT:
            rect.x = offset_x;
            break;
    }
    SDL_RenderCopy(renderer,mText,nullptr,&rect);
    SDL_FreeSurface(textSurf);
    SDL_DestroyTexture(mText);
}

