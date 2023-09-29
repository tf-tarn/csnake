#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#include <SDL2/SDL.h>


// Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))


#define COLOR_BREAKUP(COLOR)    COLOR.r, COLOR.g, COLOR.b, COLOR.a

extern const SDL_Color COLOR_TRANSPARENT;
extern const SDL_Color COLOR_BLACK;
extern const SDL_Color COLOR_WHITE;
extern const SDL_Color COLOR_GRAY;
extern const SDL_Color COLOR_DARK_GRAY;
extern const SDL_Color COLOR_LIGHT_GRAY;
extern const SDL_Color COLOR_RED;
extern const SDL_Color COLOR_GREEN;
extern const SDL_Color COLOR_BLUE;
extern const SDL_Color COLOR_YELLOW;
extern const SDL_Color COLOR_ORANGE;
extern const SDL_Color COLOR_PINK;
extern const SDL_Color COLOR_VIOLET;


void Utils_setBackgroundColor(SDL_Renderer *renderer, SDL_Color color);

long long Utils_time();

void Utils_randInit();
int Utils_rand(int min, int max);

bool Utils_equalColors(SDL_Color color1, SDL_Color color2);

#endif // UTILS_H
