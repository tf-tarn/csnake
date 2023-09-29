# cmake_minimum_required (VERSION 2.6)
# project (Snake)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -Wall -Werror=return-type")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -Wall -Werror=return-type -Werror=switch")

# add_executable(snake
#   main.cpp
#   )
# target_link_libraries(snake SDL2)



project(basic-c-sdl-game)
set(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required(VERSION 3.5.1)
add_executable(snake
  main.c
  game.c
  grid.c
  utils.c
  )
target_include_directories(snake PRIVATE include)
target_link_libraries(snake SDL2)
target_link_libraries(snake SDL2_gfx)

