//
// Created by My PC on 11/05/2022.
//

#ifndef TETRIS_PROJECT_LBUTTON_H
#define TETRIS_PROJECT_LBUTTON_H

#include "Texture.h"
class LButton
{
public:

    enum MOUSE_STATUS
    {
        MOUSE_OUT ,
        MOUSE_OVER_MOTION,
        MOUSE_DOWN,
        MOUSE_UP,
        MOUSE_TOTAL,
    };

    //constructor
    LButton();

    //Initializes internal variables
    void set_up(const std::string &over_button_image,
            const std::string &inside_button_image,
            const int &x, const int &y,
            SDL_Renderer* renderer_);

    //check the button has been clicked or not
    bool be_clicked();

    //update the pre_status
    void update_button_status();

    //Handles mouse event
    void handleEvent( SDL_Event* e );

    //Shows button sprite
    void render(SDL_Renderer* renderer);



    //Currently used global sprite
    MOUSE_STATUS cur_status;
    MOUSE_STATUS pre_status;

private:
    //Top left position
    SDL_Point mPosition;


    //Texture
    Texture outside_text;
    Texture inside_text;

    //button dimension
    int BUTTON_WIDTH;
    int BUTTON_HEIGHT;

};


#endif //TETRIS_PROJECT_LBUTTON_H
