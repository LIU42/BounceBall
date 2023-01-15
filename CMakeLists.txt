cmake_minimum_required(VERSION 3.0.0)
project(BounceBall VERSION 0.1.0)

set(SDL_PATH "D:/SDL2")

include_directories(${SDL_PATH}/include)
link_directories(${SDL_PATH}/lib)

file(GLOB_RECURSE SOURCES_FILES 
    ${PROJECT_SOURCE_DIR}/Sources/*.cpp
    ${PROJECT_SOURCE_DIR}/Sources/*.rc
)

file(GLOB_RECURSE LIBRARY_FILES
    ${SDL_PATH}/lib/*.lib
)

add_executable(BounceBall WIN32 ${SOURCES_FILES})
target_link_libraries(BounceBall ${LIBRARY_FILES} Dwmapi.lib)