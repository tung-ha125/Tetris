//
// Created by My PC on 26/04/2022.
//

#include "Header/Texture.h"

Texture::Texture()
{
    mTexture = nullptr;
    width = 0;
    height = 0;
}



Texture::~Texture()
{
    free();
}



void Texture::free()
{
    if(mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        width = 0;
        height = 0;
    }
}



bool Texture::loadImage(std::string path,SDL_Renderer* renderer){
    //get rid from preexsting texture
    free();

    //Image is loaded from specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    SDL_Texture* newText = nullptr;
    if(loadedSurface == nullptr){
        std::cout<<"Image could not be loaded from path: " <<path <<" " <<IMG_GetError();
    }
    else{
        //SDL_SetColorKey(loadedSurface,SDL_TRUE, SDL_MapRGB(loadedSurface->format,0xFF,0xFF,0xFF));
        newText = SDL_CreateTextureFromSurface(renderer,loadedSurface);
        if(newText == nullptr){
            std::cout<<"Texture could not be created from Surface. " <<SDL_GetError();
        }
        else{
            width = loadedSurface->w;
            height = loadedSurface->h;
        }
        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    //COPY newText to mText
    mTexture = newText;

    return (mTexture != nullptr );
}


#if defined(SDL_TTF_MAJOR_VERSION)
bool Texture::loadFromRendereredText(std::string textureText, SDL_Renderer* renderer, SDL_Color textColor, TTF_Font* gFont){
    //get rid from preexsting texture
    free();

    //Image is loaded from specified path
    SDL_Surface* loadedSurface = TTF_RenderText_Solid(gFont,textureText.c_str(),textColor);
    SDL_Texture* newText = nullptr;
    if(loadedSurface == nullptr){
        std::cout<<"Image could not be loaded from path: " <<textureText <<" " <<IMG_GetError();
    }
    else{
        //SDL_SetColorKey(loadedSurface,SDL_TRUE, SDL_MapRGB(loadedSurface->format,0,0xFF,0xFF));
        newText = SDL_CreateTextureFromSurface(renderer,loadedSurface);
        if(newText == nullptr){
            std::cout<<"Texture could not be created from Surface. " <<SDL_GetError();
        }
        else{
            width = loadedSurface->w;
            height = loadedSurface->h;
        }
        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    //COPY newText to mText
    mTexture = newText;

    return (mTexture != nullptr );
}
#endif


void Texture::render(const int &x, const int &y, SDL_Renderer* renderer,SDL_Rect* clip)
{
    SDL_Rect renderQuad = {x,y,width,height};
    if(clip != nullptr){
        renderQuad.w = clip->w ;
        renderQuad.h = clip->h ;
    }
    SDL_RenderCopy(renderer,mTexture,clip,&renderQuad);
}


void Texture::render(const int &x, const int &y,SDL_Renderer* renderer)
{

    SDL_Rect rect = {x,y,width,height};
    SDL_Rect drect = {0,0,width,height};
    //SDL_RenderSetViewport(renderer,&rect);
    SDL_RenderCopy(renderer,mTexture,&drect,&rect);
}



int Texture::getWidth(){
    return width;
}

int Texture::getHeight(){
    return height;
}

void Texture::setColor(Uint8 &r, Uint8 &g, Uint8 &b){
    //Modulates the color
    SDL_SetTextureColorMod(mTexture,r,g,b);
}

void Texture::setBlendMode(SDL_BlendMode blend){
    SDL_SetTextureBlendMode(mTexture,blend);
}

void Texture::setAlpha(Uint8 alpha){
    SDL_SetTextureAlphaMod(mTexture,alpha);
}