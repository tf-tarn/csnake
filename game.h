#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "grid.h"


bool Game_start(SDL_Renderer *renderer, int w, int h);

#endif // GAME_H
