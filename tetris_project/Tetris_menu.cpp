//
// Created by My PC on 12/05/2022.
//

#include "Header/Tetris_menu.h"

/***
    - renderer: renderer of the program
***/
Tetris_menu::Tetris_menu(SDL_Renderer* renderer) {

    audio_off_status = true;
    play.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\play_button.png",
            "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\pressing_play_button.png",
            100,185,
            renderer);

    level_value = 0;
    level.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\level_1.png",
             "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\pressing_level_1.png",
             100,317,
             renderer);

    Audio_off.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\audio_off.png",
                 "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\pressing_audio_off.png",
                 100,449,
                 renderer);

    Audio_on.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\audio_on.png",
                "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\pressing_audio_on.png",
                100,449,
                renderer);

    audio_off_status = true;

    menu_background.loadImage("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\background_2.png",
                              renderer);
    tetris_logo.loadImage("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\tetris_logo.png",renderer);

    highest_score_board.loadImage("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\highest_score_board.png",renderer);
    std::ifstream file("C:\\Users\\My PC\\CLionProjects\\tetris_project\\highest_score.txt");
    file >>highest_point;
    file.close();
}


/***
    - e: Event of the program to handle
***/
void Tetris_menu::handle_event(SDL_Event* e) {
    play.handleEvent(e);
    level.handleEvent(e);
    if(audio_off_status) Audio_off.handleEvent(e);
    else Audio_on.handleEvent(e);
}


/***
    - renderer: renderer of the program
    - game: Game variable of the program to update
***/
void Tetris_menu::update_Tetris_menu(SDL_Renderer* renderer, Game* game) {

    //update start level
    if(level.be_clicked()) {
        level_value  = (level_value + 1) % 3;
    }
    //update level board
    switch(level_value) {
        case 0:
            level.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\level_1.png",
                         "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\pressing_level_1.png",
                         100,317,
                         renderer);
            break;
        case 1:
            level.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\level_2.png",
                         "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\pressing_level_2.png",
                         100,317,
                         renderer);
            break;
        case 2:
            level.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\level_3.png",
                         "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\pressing_level_3.png",
                         100,317,
                         renderer);
            break;
    }

    //update play
    if(play.be_clicked()) {
        game->status = GAME_START;
    }

    //update audio
    if(audio_off_status && Audio_off.be_clicked()) {
        audio_off_status = false;
        transform_audio(&Audio_off,&Audio_on);
    }
    else if (!audio_off_status && Audio_on.be_clicked()) {
        audio_off_status = true;
        transform_audio(&Audio_on,&Audio_off);
    }

    play.update_button_status();
    level.update_button_status();
    if(audio_off_status) Audio_off.update_button_status();
    else Audio_on.update_button_status();

    //update highest score
    change_highest_point(game->point);
}


/***
    - cur_audio: current audio
    - next_audio: audio program want to transform to
***/
void Tetris_menu::transform_audio(LButton* cur_audio, LButton* next_audio) {
    next_audio->cur_status = cur_audio->cur_status;
    next_audio->pre_status = cur_audio->pre_status;
}


/***
    renderer: renderer of the program
***/
void Tetris_menu::render(SDL_Renderer* renderer, TTF_Font* gFont) {
    menu_background.render(0,0,renderer);
    tetris_logo.render(50,50,renderer);
    play.render(renderer);
    level.render(renderer);
    if(audio_off_status) Audio_off.render(renderer);
    else Audio_on.render(renderer);
    render_highest_score_board(renderer,gFont);
}


void Tetris_menu::render_highest_score_board(SDL_Renderer* renderer, TTF_Font* gFont) {
    highest_score_board.render(600,100,renderer);
    std::stringstream str_score;
    str_score.str("");
    str_score <<highest_point;
    render_string(str_score.str(),50,CENTER,black,
                  600 + highest_score_board.getWidth() / 2,100 + highest_score_board.getHeight() / 2,gFont,renderer);
}

Tetris_menu::Tetris_menu() {
    //do nothing
}


Tetris_menu::~Tetris_menu() {
    //do nothing
}


bool Tetris_menu::is_higher(const int &point) {
    return point > highest_point;
}

void Tetris_menu::change_highest_point(const int& point) {
    if( is_higher(point) ) {
        highest_point = point;
        std::ofstream file("C:\\Users\\My PC\\CLionProjects\\tetris_project\\highest_score.txt");
        file << highest_point;
        file.close();
    }
}




