//
// Created by My PC on 12/05/2022.
//

#ifndef TETRIS_PROJECT_TETRIS_MENU_H
#define TETRIS_PROJECT_TETRIS_MENU_H

#include "Game.h"
class Tetris_menu {
public:

    //Button in tetris menu
    LButton play;
    LButton level; int level_value;
    LButton Audio_off, Audio_on;
    Texture menu_background;
    Texture tetris_logo;
    Texture highest_score_board;

    int highest_point;
    //Constructor
    Tetris_menu();
    Tetris_menu(SDL_Renderer* renderer);

    //Destructor
    ~Tetris_menu();

    //Handle event of the menu, it will handle all button's status in menu
    void handle_event(SDL_Event* e);

    //Update data for all button in menu
    void update_Tetris_menu(SDL_Renderer* renderer, Game* game);

    //Transform current audio to another audio
    void transform_audio(LButton* cur_audio, LButton* next_audio);\

    //Render tetris menu
    void render(SDL_Renderer* renderer, TTF_Font* gFont);

    void render_highest_score_board(SDL_Renderer* renderer, TTF_Font* gFont);

    void update_highest_score_board();

    bool is_higher(const int &point);

    void change_highest_point(const int& point);

    //Status of the audio, true: audio is off, false: audio is on
    bool audio_off_status;
};











#endif //TETRIS_PROJECT_TETRIS_MENU_H
