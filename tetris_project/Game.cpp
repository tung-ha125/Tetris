//
// Created by My PC on 03/05/2022.
//

#include "Header/Game.h"

Game::Game() {
    status = GAME_ON_MENU;
    piece = nullptr;
    held_piece = nullptr;
    next_piece_index = -1;

    cleared_line_count = 0;
    current_cleared_lines = 0;
    point = 0;
    level = 0;
    start_level = 0;

    for(auto &i : filled) {
        i = 0;
    }

    drop = false;
    game_over = false;
    hard_drop = false;
    line_clear = false;
    lock = false;
    move = false;
    rotate = false;

}

/***
    update game point when player clears lines
    return the point player is given
 ***/
long long int Game::calculate_game_point() {
    if(current_cleared_lines == 1) return 40 * (level+1);
    if(current_cleared_lines == 2) return 100 * (level+1);
    if(current_cleared_lines == 3) return 300 * (level+1);
    if(current_cleared_lines == 4) return 1200 * (level+1);
    return 0;
}

/***
    return a number of lines people have to clear to go to next level
***/
int Game::get_lines_for_next_level() {
    int temp = std::max(10 * start_level - 50, 100);
    int first_level_up = std::min(start_level * 10 + 10,
                                  temp);
    if(level == start_level) return first_level_up;

    int next_level_up = (level - start_level) * 10 + first_level_up;
    return next_level_up;
}


/***
    return a number of lines people has just filled at the moment
***/
int Game::count_filled_lines() {
    int count = 0;
    for(int i : filled) {
        if( i > 0 ) count++;
    }
    return count;
}




/***
    - check the collision of blocks on board,
    - if not
         return true
      else return false
***/
bool Game::check_piece_valid(Piece* back_up) {
    const Tetrimino *tetrimino = &TETRIMINOS[back_up->tetrimino_index];

    for(int i = 0; i < tetrimino->side; i++) {
        for(int j = 0; j < tetrimino->side; j++) {
            int value = tetrimino->get_data_value(i,j,back_up->rotation);
            if(value) { //We just render data's value which is greater than 0 so we don't have to worry about the elements have value = 0
                int current_row = back_up->offset_row + i;
                int current_col = back_up->offset_col + j;
                if(current_row < 0 || current_col < 0) return false;
                if(current_col >= BOARD_WIDTH || current_row >= BOARD_HEIGHT) return false;
                if( get_data_value(board,BOARD_WIDTH,current_row,current_col) ) return false; //the board already has piece in current position
            }
        }
    }

    return true;
}


/***
    `- drop piece after a few second
     - if the soft drop pass the check_piece_valid() (means check_piece_valid() return true),
     it will return true, else there is a collision and the soft drop activity is failed
 ***/
bool Game::soft_drop() {
    piece->offset_row++;
    if (!check_piece_valid(piece)) {
        piece->offset_row--;
        return false;
    }
    return true;
}


/***
    - Function merges piece to board by update the value of the matrix
    and set the position of blocks we will render the lock effect
 ***/
void Game::merge_piece_to_board() {
    const Tetrimino *tetrimino = &TETRIMINOS[piece->tetrimino_index];
    int count = 0;
    for(int i = 0; i < tetrimino->side; i++) {
        for(int j = 0; j < tetrimino->side; j++) {
            int value = tetrimino->get_data_value(i,j,piece->rotation);
            if(value) { //We just render data's value which is greater than 0 so we don't have to worry about the elements have value = 0
                int current_row = piece->offset_row + i;
                int current_col = piece->offset_col + j;
                set_data_value(board,BOARD_WIDTH,current_row,current_col,value);
                set_light_block(count,current_row,current_col);
                count++;
            }
        }
    }
}


/***
    spawn the piece
 ***/
void Game::spawn_piece() {
    if(next_piece_index == -1) {
        int random_piece = get_random_number(0,6);
        piece = new Piece;
        piece->tetrimino_index = random_piece;
        next_piece_index = pick_a_random_number_for_next_piece();
    }
    else {
        int next_index = pick_a_random_number_for_next_piece();
        delete [] piece;
        piece = new Piece;
        piece->tetrimino_index = next_piece_index;
        next_piece_index = next_index;
    }
    piece->offset_row = 0;
    piece->offset_col = BOARD_WIDTH / 2;
}


/***
    algorithm to choose a new piece
***/
int Game::pick_a_random_number_for_next_piece() const {
    int index = piece->tetrimino_index;
    int result = get_random_number(0,6);
    if( result == index)
        result = ( result + get_random_number(0,6) ) % 7;

    return result;
}


/***
    erase rows which are full
 ***/
void Game::empty_rows_filled() {
    if( check_rows_filled() ) {
        int temp_board[BOARD_HEIGHT * BOARD_WIDTH] = {0};

        int src_row = BOARD_HEIGHT - 1;
        for(int i = BOARD_HEIGHT - 1; i >= BOARD_HEIGHT - VISIBLE_HEIGHT; i--) {
            if( !filled[i] ) {
                for(int j = 0; j < BOARD_WIDTH; j++) {
                    int value = get_data_value(board,BOARD_WIDTH,i,j);
                    if (value) set_data_value(temp_board,BOARD_WIDTH,src_row,j,value);
                }
                src_row--;
            }
        }

        copyMatrix(temp_board, board, BOARD_HEIGHT, BOARD_WIDTH);
    }
}


/***
    - filled: matrix present status of row, 1 is filled row, 0 is unfilled row
    - return number of rows has filled
 ***/
int Game::check_rows_filled() {
    int filled_line_count = 0;
    for(int i = BOARD_HEIGHT - 1; i >= (BOARD_HEIGHT - VISIBLE_HEIGHT); i--)
    {
        int count = 0;
        for(int j = BOARD_WIDTH - 1; j>= 0; j--) {
            if( get_data_value(board,BOARD_WIDTH,i,j) ) count++;
            else break;
        }
        if(count == BOARD_WIDTH) {
            filled[i] = 1;
            filled_line_count++;
        }
        else filled[i] = 0;
    }

    return filled_line_count;
}


/***
    - check the row is empty or not
    - return true if no block in row, else return false
 ***/
bool Game::check_row_empty(const int &row) {
    for(int i = 0; i < BOARD_WIDTH; i++) {
        if( get_data_value(board,BOARD_WIDTH,row,i) ) return false;
    }

    return true;
}


/***
    - set the blocks' position we want to render lock effect
 ***/
void Game::set_light_block(int index, int current_row, int current_col){
    light_block[index] = {current_col,current_row};
}


/***
    - set end time for rendering lock effect
 ***/
void Game::set_game_lighter_time() {
    lighter_time = time + 0.1;
}


/***
    - update end time for rendering game_over string
 ***/
void Game::update_render_game_over_string_time(){
    render_game_over_string_end_time = time + 3.0;
}

/***
    - update next time to drop the piece
 ***/
void Game::update_next_drop_time() {
    float second_per_frame = 1.0 / 60.0;
    next_drop_time = time + second_per_frame * DROP_FRAMES_PER_LEVEL[level];
}








