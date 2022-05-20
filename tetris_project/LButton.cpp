//
// Created by My PC on 11/05/2022.
//

#include "Header/LButton.h"


/***
    - set up the image from the path
        over_button_image: path to button image when mouse is outside.
        inside_button_image: path to button image when mouse is inside.
        x,y: location to render button's image
        renderer_: renderer of the program
 ***/
void LButton::set_up(const std::string &over_button_image,
                 const std::string &inside_button_image,
                 const int &x, const int &y,
                 SDL_Renderer* renderer_)
{
    outside_text.loadImage(over_button_image,renderer_);
    inside_text.loadImage(inside_button_image,renderer_);
    mPosition.x = x;
    mPosition.y = y;
    BUTTON_WIDTH = outside_text.getWidth();
    BUTTON_HEIGHT = outside_text.getHeight();
}


/***
    - Check the button has been clicked or not
    - Base on the current status and previous status of the button
***/
bool LButton::be_clicked() {
    if(cur_status != MOUSE_DOWN && pre_status == MOUSE_DOWN ) return true;
    return false;
}

/***
    - update the previous button status to put new status to current status
***/
void LButton::update_button_status() {
        pre_status = cur_status;
}


//constructor
LButton::LButton() {
    mPosition.x = 0; mPosition.y = 0;
    BUTTON_WIDTH = 0; BUTTON_HEIGHT = 0;
    cur_status = MOUSE_OUT;
    pre_status = MOUSE_OUT;
}


/***
    - Handle mouse event of the button
***/
void LButton::handleEvent( SDL_Event* e )
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }
            //Mouse is right of the button
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }
            //Mouse above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }
            //Mouse below the button
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }

        //Mouse is outside button
        if( !inside )
        {
            cur_status = MOUSE_OUT;
        }
            //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                    cur_status = MOUSE_OVER_MOTION;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    cur_status = MOUSE_DOWN;
                    break;

                case SDL_MOUSEBUTTONUP:
                    cur_status = MOUSE_UP;
                    break;
            }
        }
    }
}


/***
    - Render button on the screen
***/
void LButton::render(SDL_Renderer* renderer)
{

    if (cur_status == MOUSE_OUT) outside_text.render(mPosition.x,mPosition.y,renderer);
    else if(cur_status == MOUSE_DOWN ||
            cur_status == MOUSE_UP ||
            cur_status == MOUSE_OVER_MOTION)
        inside_text.render(mPosition.x,mPosition.y,renderer);

}