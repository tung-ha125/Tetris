cmake_minimum_required(VERSION 3.21)
project(tetris_project)

set(CMAKE_CXX_STANDARD 14)

set(SDL2_INCLUDE_DIR D:/code/src/include)
set(SDL2_LIB_DIR D:/code/src/lib)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(tetris_project main.cpp Texture.cpp Header/Texture.h common.cpp Header/common.h Tetris.cpp Header/Tetris.h Game.cpp Header/Game.h Tetrimino.cpp Header/Tetrimino.h Tetrimino.cpp Header/Tetrimino.h Piece.cpp Header/Piece.h LButton.cpp Header/LButton.h Tetris_menu.cpp Header/Tetris_menu.h)
target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_image SDL2_ttf SDL2_mixer)
