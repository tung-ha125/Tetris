#include <windows.h>
#include "Header/Tetris.h"


SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* gFont = nullptr;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow) {
    //Declare the game
    Tetris *tetris = new Tetris(window, renderer, gFont);

    //Generate seed for random function
    std::srand(time(0));
    //Initialize the game
    if(tetris->init()) {
        //Set up needed things for the game
        tetris->setup();

        //quit flag, if the flag == true, the window will be eliminated
        bool quit = false;

        //Event the program will handle (about keyboard)
        SDL_Event e;

        //game loop
        while(!quit) {
            tetris->game.time = SDL_GetTicks64() / 1000.0;
            while(SDL_PollEvent(&e) > 0) {
                if(e.type == SDL_QUIT) quit = true;
                tetris->handle_button(&e);
            }
            tetris->handle_keyboard();
            tetris->update_game();
            tetris->render_game();
            tetris->play_music();
        }
    }

    //Free tetris after out game
    tetris->free();

    return 0;
}








