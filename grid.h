#ifndef GRID_H
#define GRID_H

#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "utils.h"

#define GRID_MAX_X_CELLS    (20)
#define GRID_MAX_Y_CELLS    (20)

struct Cell
{
    // Rect dimensions and color
    SDL_Rect rect;
    SDL_Color rectColor;

    // Border dimensions and color
    SDL_Rect border;
    SDL_Color borderColor;
};
typedef struct Cell Cell;


struct Grid
{
    // x, y, width, height
    SDL_Rect rect;

    // Grid background color
    SDL_Color backgroundColor;

    // Grid border thickness and color
    unsigned int border;
    SDL_Color borderColor;

    // Number of cells over the x axis
    int xCells;
    // Number of cells over the y axis
    int yCells;

    // Cells boder thickness and color
    unsigned int cellsBorder;
    SDL_Color cellsBorderColor;

    // Matrix of Cells
    Cell cells[GRID_MAX_X_CELLS][GRID_MAX_Y_CELLS];
};
typedef struct Grid Grid;

int Grid_ajustSize(Grid *grid);
void Grid_alignCenter(Grid *grid, int screenWidth, int screenHeight);

bool Grid_init(Grid *grid);
void Grid_initCell(Grid *grid, Cell *cell, int i, int j, SDL_Color color, SDL_Color borderColor);

void Grid_render(Grid *grid, SDL_Renderer *renderer);
void Grid_renderCell(Cell *cell, SDL_Renderer *renderer);

#endif // GRID_H
