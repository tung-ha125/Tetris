//
// Created by My PC on 03/05/2022.
//

#ifndef TETRIS_PROJECT_GAME_H
#define TETRIS_PROJECT_GAME_H

#include "Piece.h"
#include "LButton.h"
#include "fstream"

class Game {
public:
    int board[BOARD_WIDTH * BOARD_HEIGHT] = {0};
    GAME_STATUS status;
    Piece *piece;
    Piece *held_piece;
    int next_piece_index;

    double time;
    double next_drop_time;
    double highlight_line_end_time;
    double lighter_time;
    double render_game_over_string_end_time;
    SDL_Point light_block[4];

    int filled[BOARD_HEIGHT];
    int cleared_line_count;
    int current_cleared_lines;

    long long int point;
    int level;
    int start_level;

    bool drop;
    bool game_over;
    bool hard_drop;
    bool line_clear;
    bool lock;
    bool move;
    bool rotate;
//constructor___________________________________________________________________________________________________________
    Game();

//logic of the game_____________________________________________________________________________________________________
    long long int calculate_game_point();

    int get_lines_for_next_level();
    int count_filled_lines();

    bool check_piece_valid(Piece* back_up);
    bool soft_drop();
    void merge_piece_to_board();
    void spawn_piece();
    int pick_a_random_number_for_next_piece() const;

    void empty_rows_filled();
    int check_rows_filled();
    bool check_row_empty(const int &row);

    void set_light_block(int index, int current_row, int current_col);
    void set_game_lighter_time();
    void update_render_game_over_string_time();
    void update_next_drop_time();


};


#endif //TETRIS_PROJECT_GAME_H
