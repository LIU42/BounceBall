cmake_minimum_required(VERSION 3.0.0)
project(BounceBall VERSION 0.1.0)

set(SDL_PATH "E:/Kits/SDL2")

include_directories(${SDL_PATH}/include)
link_directories(${SDL_PATH}/lib)

add_executable(BounceBall WIN32 main.cpp bounceball.cpp bounceball.h config.h resource.h BounceBall.rc)
