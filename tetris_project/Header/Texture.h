//
// Created by My PC on 26/04/2022.
//

#pragma once
#ifndef TETRIS_PROJECT_TEXTURE_H
#define TETRIS_PROJECT_TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

class Texture {
private:
    //The actual hardware texture

    //Image dimensions
    int width;
    int height;

public:

    SDL_Texture* mTexture;

    //Initialize variables
    Texture();

    //Deallocate memory
    void free();

    //Destructor
    ~Texture();

    //Loads image from specified path
    bool loadImage(std::string path,SDL_Renderer* renderer);

#if defined(SDL_TTF_MAJOR_VERSION)
    bool loadFromRendereredText(std::string textureText, SDL_Renderer* renderer, SDL_Color textColor, TTF_Font* gFont);
#endif

    //Renders Texture at given point
    void render(const int &x, const int &y, SDL_Renderer* renderer,SDL_Rect* clip);
    void render(const int &x, const int &y,SDL_Renderer* renderer);

    //Get Image demension
    int getWidth();

    int getHeight();

    //Set color of the texture
    void setColor(Uint8 &r, Uint8 &g, Uint8 &b);
    void setBlendMode(SDL_BlendMode blend);
    void setAlpha(Uint8 alpha);
};


#endif //TETRIS_PROJECT_TEXTURE_H
