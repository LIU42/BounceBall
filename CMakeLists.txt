cmake_minimum_required(VERSION 3.0.0)
project(BounceBall VERSION 0.1.0)

set(SDL_PATH "D:/SDL2")

include_directories(${SDL_PATH}/include)
link_directories(${SDL_PATH}/lib)

add_executable(
    BounceBall
    WIN32
    Sources/main.cpp
    Sources/bounceball.cpp
    Sources/ball.cpp
    Sources/block.cpp
    Sources/plank.cpp
    Sources/resources.rc
)

target_link_libraries(
    BounceBall
    SDL2.lib
    SDL2main.lib
    SDL2_image.lib
    SDL2_ttf.lib
)