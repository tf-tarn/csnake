#include "game.h"

bool Game_start(SDL_Renderer *renderer, int w, int h)
{
    // Init grid
    Grid grid = {0};

    // Set grid dimensions
    int margin = 50;
    grid.rect.w = MIN(w - margin * 2, h - margin * 2);
    grid.rect.h = grid.rect.w;

    // Set grid backgroud
    grid.backgroundColor = COLOR_DARK_GRAY;

    // Set grid border thickness and color
    grid.border = 3;
    grid.borderColor = COLOR_GRAY;

    // Set number of cells
    grid.xCells = 10;
    grid.yCells = 10;

    // Set cells border thickness and color
    grid.cellsBorder = grid.border;
    grid.cellsBorderColor = grid.borderColor;

    // Ajust size and center
    Grid_ajustSize(&grid);
    Grid_alignCenter(&grid, w, h);

    if(!Grid_init(&grid))
    {
        fprintf(stderr, "Grid fail to initialize !\n");
        return false;
    }

    // Initialize framerate manager : 30 FPS
    FPSmanager fpsmanager;
    SDL_initFramerate(&fpsmanager);
    SDL_setFramerate(&fpsmanager, 30);

    // Initialize start time (in ms)
    long long last = Utils_time();

    // Falling brick coordinates
    int fallingBrickX = grid.xCells / 2;
    int fallingBrickY = -1;
    int fallingBrickSpeed = 4;


    // Event loop exit flag
    bool quit = false;

    // Event loop
    while(!quit)
    {
        SDL_Event e;

        // Get available event
        while(SDL_PollEvent(&e))
        {
            // User requests quit
            if(e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            else if(e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;

                case SDLK_RIGHT:
                    if(fallingBrickY != -1 && fallingBrickX < grid.xCells - 1)
                    {
                        // Un-color last position
                        grid.cells[fallingBrickX][fallingBrickY].rectColor = grid.backgroundColor;

                        // Color new position
                        fallingBrickX++;
                        grid.cells[fallingBrickX][fallingBrickY].rectColor = COLOR_RED;
                    }
                    break;

                case SDLK_LEFT:
                    if(fallingBrickY != -1 && fallingBrickX > 0)
                    {
                        // Un-color last position
                        grid.cells[fallingBrickX][fallingBrickY].rectColor = grid.backgroundColor;

                        // Color new position
                        fallingBrickX--;
                        grid.cells[fallingBrickX][fallingBrickY].rectColor = COLOR_RED;
                    }
                    break;
                }
            }
        }

        // Move the falling brick
        if(Utils_time() - last >= 1000 / fallingBrickSpeed)
        {
            if(fallingBrickY >= 0)
            {
                // Un-color the falling brick last position
                grid.cells[fallingBrickX][fallingBrickY].rectColor = grid.backgroundColor;
            }

            if(fallingBrickY < grid.yCells - 1)
            {
                // Go to next position
                fallingBrickY++;

                // Color the falling brick new position
                grid.cells[fallingBrickX][fallingBrickY].rectColor = COLOR_RED;
            }
            else
            {
                // Reset position
                fallingBrickY = -1;
            }

            last = Utils_time();
        }

        // Set background color
        Utils_setBackgroundColor(renderer, COLOR_DARK_GRAY);

        // Render grid
        Grid_render(&grid, renderer);

        // Show message
        stringRGBA(renderer, grid.rect.x + grid.xCells, grid.rect.y - 20,
                   "This is a falling brick < Press RIGTH and LEFT to move >",
                   COLOR_LIGHT_GRAY.r, COLOR_LIGHT_GRAY.g, COLOR_LIGHT_GRAY.b, COLOR_LIGHT_GRAY.a);

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
    }

    return true;
}
