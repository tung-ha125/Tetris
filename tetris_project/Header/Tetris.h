//
// Created by My PC on 01/05/2022.
//

#ifndef TETRIS_PROJECT_TETRIS_H
#define TETRIS_PROJECT_TETRIS_H


#include "Tetrimino.h"
#include "Tetris_menu.h"

class Tetris {
public:
//game public elements__________________________________________________________________________________________________
    Game_input input;
    Game game;
    Tetris_menu* menu = nullptr;
    const char* title = "Tetris";
    LButton pause_button;
    LButton audio_button;
    LButton exit_button;

//logic_________________________________________________________________________________________________________________

    //update status and related things
    void update_game();
    void update_game_play();
    void update_game_over();
    void update_game_start();
    void update_game_line();
    void update_game_menu();
    void update_game_pause();
    void update_hold_piece();

    void handle_keyboard();
    void handle_button(SDL_Event* e);

    void highlight_line(const int& row);




//render________________________________________________________________________________________________________________
    void render_game();
    void draw_piece(Piece* piece);
    void draw_shadow_piece(Piece* piece);
    void render_lighter_effect(Game* game);
    void render_block(std::string path, int board_row, int board_col);
    void render_block_outside_board(const std::string &path,
                                    const int &x,
                                    const int &y);


    void render_hold_piece();
    void render_score();
    void render_background();
    void render_board();
    void render_predicted_piece_table();

//Music_and_chunk_effect________________________________________________________________________________________________
    void play_music();

    Mix_Music* gBackground;
    enum SOUND_EFFECT{
        DROP,
        GAME_OVER,
        LINE_CLEAR,
        MOVE,
        ROTATE,
        HARD_DROP,
        LOCK,
        TOTAL_CHUNK,
    };

    Mix_Chunk* sound_effect[TOTAL_CHUNK];

    void load_media(Mix_Chunk** sound_effect, const std::string &path);
    void load_media(Mix_Music** music, const std::string &path);


    void free_music();

//Other_________________________________________________________________________________________________________________
    //constructor
    Tetris(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *gFont);

    //destructor
    ~Tetris();

    bool init();
    void setup();
    void free();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* gFont = nullptr;




};


#endif //TETRIS_PROJECT_TETRIS_H
