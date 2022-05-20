//
// Created by My PC on 01/05/2022.
//

#include "Header/Tetris.h"

/***
    update everything related to the status of the game
 ***/
void Tetris::update_game() {
    switch(game.status) {
        case GAME_STATUS::GAME_PLAY:
            update_game_play();
            break;
        case GAME_STATUS::GAME_OVER:
            update_game_over();
            break;
        case GAME_STATUS::GAME_START:
            update_game_start();
            break;
        case GAME_STATUS::GAME_HIGHLIGHT_LINE:
            update_game_line();
            break;
        case GAME_STATUS::GAME_ON_MENU:
            update_game_menu();
            break;
        case GAME_STATUS::GAME_ON_PAUSE:
            update_game_pause();
            break;
    }
}

/***
    The game will have 6 status :
       - GAME_PLAY: Player are playing game, it will update every data about status, piece, music, effect, button
                    in a game
       - GAME_OVER: Player loses, it will set the time to render the game over on the monitor (3 seconds) and
                    return you to the menu of the tetris game
       - GAME_START: Player begins playing game, it will reset the game, spawn a new piece
       - GAME_HIGHLIGHT_LINE: When player fills line, it will set the time for playing cleared music and effect
       - GAME_ON_MENU: When people in menu, it will update data about render, button, status of the game
       - GAME_ON_PAUSE: update data when people press paused button in game
***/

/***
    update data while player are playing game
***/

void Tetris::update_game_play() {
    Piece back_up = *game.piece;
    if(input.right_status > 0) {
        back_up.offset_col++;
        game.move = true;
    }
    if(input.left_status > 0) {
        back_up.offset_col--;
        game.move = true;
    }
    if(input.down_status > 0) {
        back_up.offset_row++;
        game.move = true;
    }
    if(input.up_status > 0) {
        back_up.rotation = (back_up.rotation + 1) % 4; //it will guarantee that rotation is always
                                      //between 0 and 3
        game.rotate = true;
    }
    if( game.check_piece_valid(&back_up) ) {
        //apply the current piece
        *game.piece = back_up;
    }
    else {
        if(game.rotate) game.rotate = false;
        if(game.move) game.move = false;
    }

    if (input.space_status > 0) {
        while(game.soft_drop());
        game.merge_piece_to_board();
        game.empty_rows_filled();
        game.spawn_piece();
        game.set_game_lighter_time();
        game.hard_drop = true;
        game.lock = true;
    }

    if(game.time >= game.next_drop_time) {
        if ( !game.soft_drop() ) {
            game.lock = true;
            game.merge_piece_to_board();
            game.empty_rows_filled();
            game.set_game_lighter_time();
            game.spawn_piece();
        }
        else game.update_next_drop_time();
        game.drop = true;

    }


    game.current_cleared_lines = game.count_filled_lines();
    if(game.current_cleared_lines > 0) {
        game.status = GAME_HIGHLIGHT_LINE;
        game.highlight_line_end_time = game.time + 0.2;
        game.line_clear = true;
    }


    if( !game.check_row_empty(0 )) {
        game.status = GAME_STATUS::GAME_OVER;
        game.update_render_game_over_string_time();
        game.game_over = true;
    }

    if( pause_button.be_clicked() ) {
        game.status = GAME_STATUS::GAME_ON_PAUSE;
        pause_button.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\continue_button.png",
                                 "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\continue_button.png",
                                 0, SCREEN_HEIGHT * BLOCKH - 60,
                                 this->renderer);
    }

    if( audio_button.be_clicked() ) {
        if(menu->audio_off_status) {
            menu->audio_off_status = false;
        }
        else {
            if (Mix_PlayingMusic()) {
                Mix_PauseMusic();
                menu->audio_off_status = true;
            }
            else if (Mix_PausedMusic()) {
                Mix_ResumeMusic();
                menu->audio_off_status = false;
            }
        }

    }

    if( exit_button.be_clicked() ) {
        game.status = GAME_STATUS::GAME_OVER;
        if(Mix_PlayingMusic()) Mix_HaltMusic();
    }

    pause_button.update_button_status();
    audio_button.update_button_status();
    exit_button.update_button_status();

    update_hold_piece();

}


/***
    update data while player loses
***/
void Tetris::update_game_over() {
    //if (input.space_status > 0) game.status = GAME_START;
    if(game.time >= game.render_game_over_string_end_time) {
        game.status = GAME_ON_MENU;
    }
}

/***
    update data while player begins to play
***/
void Tetris::update_game_start() {
        if(game.piece) delete [] game.piece;
        if(game.held_piece) delete [] game.held_piece;
        game = {};
        game.status = GAME_PLAY;
        game.level = menu->level_value;
        game.spawn_piece();
        game.update_next_drop_time();
}

/***
    update data while player fills rows
***/
void Tetris::update_game_line() {
    if(game.time >= game.highlight_line_end_time) {
        game.empty_rows_filled();
        game.cleared_line_count += game.current_cleared_lines;
        if (game.cleared_line_count >= game.get_lines_for_next_level() && game.level < 29) game.level++;

        game.point += game.calculate_game_point();

        game.current_cleared_lines = 0;
        game.status = GAME_PLAY;
    }
}

/***
    update data while player are in game menu
***/
void Tetris::update_game_menu() {
    menu->update_Tetris_menu(renderer,&game);
}


/***
    update data when player has been pressed the pause button in game
***/
void Tetris::update_game_pause() {
    if(pause_button.be_clicked()) {
        pause_button.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\pause_button.png",
                            "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\pause_button.png",
                            0, SCREEN_HEIGHT * BLOCKH - 60,
                            this->renderer);
        game.status = GAME_STATUS::GAME_PLAY;
    }
    pause_button.update_button_status();
}


/***
    update hold piece board
 ***/
void Tetris::update_hold_piece() {
    if(input.mEnter_status > 0) {
        if (game.held_piece) {
            Piece temp = *game.piece;
            game.piece->tetrimino_index = game.held_piece->tetrimino_index;
            game.piece->rotation = game.held_piece->rotation;
            *game.held_piece = temp;
        } else {
            game.held_piece = new Piece;
            *game.held_piece = *game.piece;
            game.spawn_piece();
        }
    }
}


/***
    handle status of keyboard which need for the game
 ***/
void Tetris::handle_keyboard() {
    //Get Key Board State to update into input;
    Game_input pre_input = input;

    int key_number;
    const Uint8 *key_states  = SDL_GetKeyboardState(&key_number);

    input.left = key_states[SDL_SCANCODE_LEFT];
    input.right = key_states[SDL_SCANCODE_RIGHT];
    input.up = key_states[SDL_SCANCODE_UP];
    input.down = key_states[SDL_SCANCODE_DOWN];
    input.space = key_states[SDL_SCANCODE_SPACE];
    input.mEnter = key_states[SDL_SCANCODE_RETURN];

    input.left_status = input.left - pre_input.left;
    input.right_status = input.right - pre_input.right;
    input.up_status = input.up - pre_input.up;
    input.down_status = input.down - pre_input.down;
    input.space_status = input.space - pre_input.space;
    input.mEnter_status = input.mEnter - pre_input.mEnter;
}


/***
    handle all button in game
 ***/
void Tetris::handle_button(SDL_Event* e) {
    if(game.status == GAME_STATUS::GAME_ON_MENU) menu->handle_event(e);
    if(game.status == GAME_STATUS::GAME_PLAY ||
       game.status == GAME_STATUS::GAME_ON_PAUSE) pause_button.handleEvent(e);
    if(game.status == GAME_STATUS::GAME_PLAY) {
        exit_button.handleEvent(e);
        audio_button.handleEvent(e);
    }
}


/***
    highlight the clear line effect
***/
void Tetris::highlight_line(const int& row) {
    int x = (SCREEN_HEIGHT - BOARD_HEIGHT) / 2 - 1;
    int y = (SCREEN_WIDTH - BOARD_WIDTH) / 2;
    double percent = 1 - (game.highlight_line_end_time - game.time) / 0.2;
    if(percent > 0.85) percent = 1.0;
    SDL_Rect rect = { 0, 0, int(BOARD_WIDTH * BLOCKW * percent), BLOCKH};
    Texture mText;
    mText.loadImage(duplicate_star_path,renderer);
    mText.render(y * BLOCKW, (x + row) * BLOCKH,renderer,&rect);
}


/***
    - render game (image and effect) base on the status of the game
 ***/

void Tetris::render_game() {

    if(game.status == GAME_STATUS::GAME_PLAY) {
        SDL_SetRenderDrawColor(renderer,0,0,0,0xff);
        SDL_RenderClear(renderer);
        render_background();
        render_score();
        render_board();
        draw_piece(game.piece);
        render_predicted_piece_table();
        render_hold_piece();

        //create a shadow
        Piece piece = *game.piece;
        while(game.check_piece_valid(&piece)) {
            piece.offset_row++;
        }
        piece.offset_row--;
        draw_shadow_piece(&piece);

        //render lighter effect
        if(game.time <= game.lighter_time) render_lighter_effect(&game);

        //render some button if player is playing game
        pause_button.render(renderer);
        if(menu->audio_off_status){
            audio_button.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\off_audio_in_game.png",
                                "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\off_audio_in_game.png",
                                60,SCREEN_HEIGHT * BLOCKH - 60,
                                renderer);
        }
        else {
            audio_button.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\on_audio_in_game .png",
                                "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\on_audio_in_game .png",
                                60,SCREEN_HEIGHT * BLOCKH - 60,
                                renderer);
        }
        audio_button.render(renderer);
        exit_button.render(renderer);
    }
    else if(game.status == GAME_STATUS::GAME_OVER) {
        SDL_SetRenderDrawColor(renderer,0,0,0,0xff);
        SDL_RenderClear(renderer);
        render_background();
        SDL_Color color = {0xff,0xff,0xff,0xff};
        render_string("Game Over",40,CENTER,color,
                      SCREEN_WIDTH / 2 * BLOCKW, SCREEN_HEIGHT / 2 * BLOCKH, gFont,renderer);
    }
    else if(game.status == GAME_STATUS::GAME_ON_MENU) {
        SDL_SetRenderDrawColor(renderer,0,0,0,0xff);
        SDL_RenderClear(renderer);
        menu->render(renderer,gFont);
    }
    else if(game.status == GAME_STATUS::GAME_START) {
        SDL_SetRenderDrawColor(renderer,0,0,0,0xff);
        SDL_RenderClear(renderer);
    }
    else if(game.status == GAME_STATUS::GAME_ON_PAUSE) {
        pause_button.render(renderer);
    }


    if(game.status == GAME_STATUS::GAME_HIGHLIGHT_LINE) {
        for(int i = 0; i < BOARD_HEIGHT; i++) {
            if (game.filled[i] ) highlight_line(i);
        }

        switch( game.count_filled_lines() ) {
            case 2:
                render_string("DOUBLE KILL",30,
                              CENTER, white,
                              SCREEN_WIDTH / 2 * BLOCKW, SCREEN_HEIGHT / 2 * BLOCKH,
                              gFont, renderer);
                break;
            case 3:
                render_string("TRIPLE KILL",30,
                              CENTER, white,
                              SCREEN_WIDTH / 2 * BLOCKW, SCREEN_HEIGHT / 2 * BLOCKH,
                              gFont, renderer);
                break;
            case 4:
                render_string("QUADRA KILL",30,
                              CENTER, white,
                              SCREEN_WIDTH / 2 * BLOCKW, SCREEN_HEIGHT / 2 * BLOCKH,
                              gFont, renderer);
                break;
        }

    }

    SDL_RenderPresent(renderer);
}

/***
    - render the piece on the board game
 ***/
void Tetris::draw_piece(Piece* piece) {
    int value = piece->tetrimino_index;
    Texture mText;
    mText.loadImage(path[value],renderer);
    Tetrimino tetri = TETRIMINOS[value];
    for(int i = 0; i < tetri.side; i++) {
        for(int j = 0; j < tetri.side; j++) {
            if(tetri.get_data_value(i,j,piece->rotation)) {
                int board_row = piece->offset_row + i;
                int board_col = piece->offset_col + j;
                if(board_row >= BOARD_HEIGHT - VISIBLE_HEIGHT)
                    render_block(path[value],board_row,board_col);
            }
        }
    }

}

/***
    - render shadow piece on the board
 ***/
void Tetris::draw_shadow_piece(Piece* piece) {
    int value = piece->tetrimino_index;
    int x = (SCREEN_HEIGHT - BOARD_HEIGHT) / 2 - 1;
    int y = (SCREEN_WIDTH - BOARD_WIDTH) / 2;
    Texture mText;
    mText.loadImage(shadow_block,renderer);
    Tetrimino tetri = TETRIMINOS[value];
    for(int i = 0; i < tetri.side; i++) {
        for(int j = 0; j < tetri.side; j++) {
            if(tetri.get_data_value(i,j,piece->rotation)) {
                int board_row = piece->offset_row + i;
                int board_col = piece->offset_col + j;
                if(board_row >= BOARD_HEIGHT - VISIBLE_HEIGHT)
                    mText.render((y + board_col) * BLOCKW, ( x + board_row) * BLOCKH, renderer);
            }
        }
    }

}

/***
    render lighter effect on block if it was locked
 ***/
void Tetris::render_lighter_effect(Game *game) {
    for(int i = 0; i < 4; i++) {
        int board_row = game->light_block[i].y;
        int board_col = game->light_block[i].x;
        if(board_row >= BOARD_HEIGHT - VISIBLE_HEIGHT)
            render_block("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\BLock\\Single Blocks\\block_locked.png",board_row,board_col);
    }
}


/***
    render a block
 ***/
void Tetris::render_block(std::string path, int board_row, int board_col) {
    Texture piece;
    int x = (SCREEN_WIDTH - BOARD_WIDTH) / 2;
    int y = (SCREEN_HEIGHT - BOARD_HEIGHT) / 2 - 1;
    piece.loadImage(path,renderer);
    piece.render((board_col + x) * BLOCKW,(board_row + y) * BLOCKH,renderer);
}

/***
    render block outside the board
 ***/
void Tetris::render_block_outside_board(const std::string &path,
                                        const int &x, const int &y){
    Texture piece;
    piece.loadImage(path,renderer);
    piece.render(x,y,renderer);
}


/***
    render hold piece board
 ***/
void Tetris::render_hold_piece() {
    Texture hold_board;
    hold_board.loadImage("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\hold_frame.png",
                         renderer);

    int x = 0;
    int y = 100;
    hold_board.render(x, y, renderer);
    if (game.held_piece) {
        Tetrimino tetri = TETRIMINOS[game.held_piece->tetrimino_index];
        Texture piece;
        piece.loadImage(outside_block_path[game.held_piece->tetrimino_index], renderer);
        piece.render(x + hold_board.getWidth() / 2 - piece.getWidth() / 2,
                     y + hold_board.getHeight() / 2 - piece.getHeight() / 3,
                     renderer);
    }
}


/***
    render score board
 ***/
void Tetris::render_score() {

    int x = (SCREEN_HEIGHT - BOARD_HEIGHT) / 2 + 1;
    int y = (SCREEN_WIDTH - BOARD_WIDTH) / 2;

    Texture mText;
    mText.loadImage("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\score_frame.png",renderer);
    mText.render((y + BOARD_WIDTH + 2) * BLOCKW,x * BLOCKH, renderer);

    std::stringstream str_score; str_score.str("");
    str_score <<game.point;
    int size = 40;
    //if(game.point >= 10000) size = 25;
    SDL_Color black = {0,0,0,255};
    render_string(str_score.str(),size,CENTER,black,(y + BOARD_WIDTH + 2) * BLOCKW + mText.getWidth() / 2,x * BLOCKH + mText.getHeight() * 1 / 2,gFont, renderer);
}


/***
    render backgroung in game
 ***/
void Tetris::render_background() {
    Texture mText;
    mText.loadImage("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\background_2.png",renderer);
    mText.render(0,0,renderer);


    int x = (SCREEN_HEIGHT - BOARD_HEIGHT) / 2 + 1;
    int y = (SCREEN_WIDTH - BOARD_WIDTH) / 2;
    SDL_Rect rect = {y * BLOCKW,x * BLOCKH,BOARD_WIDTH * BLOCKW,VISIBLE_HEIGHT * BLOCKH};
    SDL_Color color = {100,149,237,255};
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    SDL_RenderFillRect(renderer,&rect);
}


/***
    render the piece is on the board
 ***/
void Tetris::render_board() {
    for(int i = BOARD_HEIGHT - VISIBLE_HEIGHT ; i < BOARD_HEIGHT; i++) {
        for(int j = 0; j < BOARD_WIDTH; j++) {
            int value = get_data_value(game.board,BOARD_WIDTH,i,j);
            if( value ) {
                if( i >= BOARD_HEIGHT - VISIBLE_HEIGHT)
                    render_block(path[value-1],i,j);
            }
        }
    }
}


/***
    render predicted board
 ***/
void Tetris::render_predicted_piece_table() {

    int x = (SCREEN_HEIGHT - BOARD_HEIGHT) / 2 + 1;
    int y = (SCREEN_WIDTH - BOARD_WIDTH) / 2;

    Texture mText;
    mText.loadImage("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\next_frame.png",renderer);
    mText.render((y + BOARD_WIDTH + 2) * BLOCKW,x * BLOCKH + 160, renderer);

    //render next block
    if(game.next_piece_index != -1) {
        Tetrimino tetri = TETRIMINOS[game.next_piece_index];
        for(int i = 0; i < tetri.side; ++i) {
            for(int j = 0; j < tetri.side; ++j) {
                int value = get_data_value(tetri.data,
                                           tetri.side,i,j);
                if( value ) {
                    int minus_y = 0;
                    int minus_x = 0;
                    if(game.next_piece_index == 0) {
                        minus_x += (BLOCKW / 10);
                    }
                    else if(game.next_piece_index == 1){
                        minus_x -= (BLOCKW / 2);
                        minus_y -= (BLOCKW / 2);
                    }
                    else if(game.next_piece_index == 2) {
                        minus_x += (BLOCKW / 6);
                        minus_y = BLOCKH;
                    }
                    else if(game.next_piece_index == 4) {
                        minus_x += (BLOCKW / 6);
                        minus_y += (BLOCKH / 6);
                    }

                    render_block_outside_board(path[game.next_piece_index],
                                               (y + BOARD_WIDTH + 2) * BLOCKW + mText.getWidth() / 2 + j * BLOCKW - minus_x - 47,
                                               x * BLOCKH + mText.getHeight() * 1 / 2 + 145 + i * BLOCKH - minus_y);
                }
            }
        }
    }

//    SDL_Color black = {0,0,0,255};
//    render_string(str_score.str(),size,CENTER,black,(y + BOARD_WIDTH + 2) * BLOCKW + mText.getWidth() / 2,x * BLOCKH + mText.getHeight() * 1 / 2,gFont);
}




/***
    play music in game
 ***/
void Tetris::play_music() {
    if( !menu->audio_off_status) {
        if (game.status == GAME_STATUS::GAME_ON_PAUSE) Mix_PauseMusic();
        else if (game.status == GAME_STATUS::GAME_PLAY && Mix_PausedMusic()) Mix_ResumeMusic();
        else if (!Mix_PlayingMusic() && game.status == GAME_STATUS::GAME_PLAY) Mix_PlayMusic(gBackground, -1);

        if (game.move) {
            if (Mix_PlayChannel(-1, sound_effect[SOUND_EFFECT::MOVE], 0) == -1) std::cout << Mix_GetError();
            game.move = false;
        }
        if (game.drop) {
            if (Mix_PlayChannel(-1, sound_effect[SOUND_EFFECT::DROP], 0) == -1) std::cout << Mix_GetError();
            game.drop = false;
        }
        if (game.lock) {
            Mix_PlayChannel(-1, sound_effect[SOUND_EFFECT::LOCK], 0);
            game.lock = false;
        }
        if (game.line_clear) {
            Mix_PlayChannel(-1, sound_effect[SOUND_EFFECT::LINE_CLEAR], 0);
            game.line_clear = false;
        }

        if (game.game_over) {
            Mix_HaltMusic();
            Mix_PlayChannel(-1, sound_effect[SOUND_EFFECT::GAME_OVER], 0);
            game.game_over = false;
        }

        if (game.hard_drop) {
            Mix_PlayChannel(-1, sound_effect[SOUND_EFFECT::HARD_DROP], 0);
            game.hard_drop = false;
        }

        if (game.rotate) {
            Mix_PlayChannel(-1, sound_effect[SOUND_EFFECT::ROTATE], 0);
            game.rotate = false;
        }
    }
    else {
        game.move = false;
        game.drop = false;
        game.line_clear = false;
        game.game_over = false;
        game.hard_drop = false;
        game.rotate = false;
        game.lock = false;
    }
}


/***
    - load sound effect
    sound_effect: where put music in
    path: the path to the music file
 ***/
void Tetris::load_media(Mix_Chunk** sound_effect, const std::string &path) {
    *sound_effect = Mix_LoadWAV(path.c_str());
    if(sound_effect == nullptr) std::cout<<"Chunk could not be loaded! " <<Mix_GetError() <<std::endl;
}


/***
    - load music
    sound_effect: where put music in
    path: the path to the music file
 ***/
void Tetris::load_media(Mix_Music** music, const std::string &path) {
    *music = Mix_LoadMUS(path.c_str());
    if(music == nullptr) std::cout<<"Music could not be loaded! " <<Mix_GetError() <<std::endl;
}


/***
    Free music when the program is ended
 ***/
void Tetris::free_music() {
    Mix_FreeMusic(gBackground);
    gBackground = nullptr;

    for(auto i = 0; i < TOTAL_CHUNK; i++) {
        Mix_FreeChunk(sound_effect[i]);
        sound_effect[i] = nullptr;
    }
}


//constructor
Tetris::Tetris(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *gFont) {
    this->window = window;
    this->renderer = renderer;
    this->gFont = gFont;
}

//destructor
Tetris::~Tetris() {
    delete [] menu;
    free_music();
}

/***
    Initialize SDL, TTF, Mix
 ***/
bool Tetris::init()
{
    //Declare flag
    bool flag = true;

    //Initialize SDL
    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        std::cout<<"SDL could not be initialized!. Error: " <<SDL_GetError() <<" ";
        flag = false;
    }
        //Set texture to linear
    else if( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1")) {
        std::cout<<"Warning: Linear texture filtering is not enabled! " <<SDL_GetError();
        flag = false;
    }
    else{
        //Initialize window
        window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH * BLOCKW,SCREEN_HEIGHT * BLOCKH,SDL_WINDOW_SHOWN);
        if (window == nullptr){
            std::cout<<"Window could not be initialized!. Error: " <<SDL_GetError() <<" ";
            flag = false;
        }

        else{
            //Initialize renderer
            renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer == nullptr){
                std::cout<<"Renderer surface could not be initialized! " <<SDL_GetError() <<" ";
                flag = false;
            }
            //Initialize PNG Loading
            bool imgFlag = IMG_INIT_PNG;
            if ( !(IMG_Init(imgFlag) && imgFlag) ){
                std::cout<<"PNG Loading could not be initialized!. Error: " <<IMG_GetError();
                flag = false;
            }
            else{
                if( TTF_Init() == -1){
                    std::cout<<"TTF could not be initialized! " <<TTF_GetError() <<std::endl;
                    flag = false;
                }
                //The first argument is the frequency of the
                //Second argument is the format
                //Third argument is number of the hardware we will use
                //Fourth argument is the size of a chunk
                if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0){
                    std::cout<<"Mix could not be created! Error: " <<Mix_GetError() <<std::endl;
                    flag = false;
                }
            }
        }
    }


    return flag;

}


/***
    - Set up menu, button, music
 ***/
void Tetris::setup() {
    menu = new Tetris_menu(this->renderer);
    pause_button.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\pause_button.png",
                             "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\pause_button.png",
                             0, SCREEN_HEIGHT * BLOCKH - 60,
                             renderer);
    audio_button.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\on_audio_in_game .png",
                 "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\on_audio_in_game .png",
                 60,SCREEN_HEIGHT * BLOCKH - 60,
                 renderer);
    exit_button.set_up("C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\exit_door.png",
                       "C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Images\\exit_door.png",
                       SCREEN_WIDTH * BLOCKW - 60,SCREEN_HEIGHT * BLOCKH - 60,
                       renderer);
    input = {};
    //Set up music________________________________________________________________
    load_media(&sound_effect[DROP],"C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Music\\SDL_Game_audio_Drop.wav");
    load_media(&sound_effect[GAME_OVER],"C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Music\\SDL_Game_audio_Gameover.wav");
    load_media(&sound_effect[HARD_DROP],"C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Music\\SDL_Game_audio_hardDrop.wav");
    load_media(&sound_effect[LINE_CLEAR],"C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Music\\SDL_Game_audio_Lineclear_.wav");
    load_media(&sound_effect[LOCK],"C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Music\\SDL_Game_audio_lock.wav");
    load_media(&sound_effect[MOVE],"C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Music\\SDL_Game_audio_Move.wav");
    load_media(&sound_effect[ROTATE],"C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Music\\SDL_Game_audio_rotate (1).wav");
    load_media(&gBackground,"C:\\Users\\My PC\\CLionProjects\\tetris_project\\asset\\Music\\Background.wav");

    //Set up button________________________________________________________________
}

/***
    - Free SDL, TTF, Mix
 ***/
void Tetris::free() {
    //blocks->free();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(gFont);

    free_music();

    //Quit IMG and SDL
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    Mix_Quit();
}
















