#include <stdlib.h>

#include "game.h"

typedef struct snake {
    int x;
    int y;
    struct snake *prev;
    struct snake *next;
} snake_t;

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

    // Snake coordinates
    snake_t *snake = malloc(sizeof(snake_t));
    snake->x = grid.xCells / 2;
    snake->y = grid.yCells / 2;
    snake->next = NULL;
    snake_t *tail = snake;

    int snakeX = grid.xCells / 2;
    int snakeY = -1;
    int snakeSpeed = 1;
    int snake_direction = 0;
    int snake_next_direction = 0;
    int snake_length = 5;
    int grow_count = 0;

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
                    if (snake_direction & 1) {
                        snake_next_direction = 0;
                    }
                    break;
                case SDLK_DOWN:
                    if (!(snake_direction & 1)) {
                        snake_next_direction = 3;
                    }
                    break;
                case SDLK_UP:
                    if (!(snake_direction & 1)) {
                        snake_next_direction = 1;
                    }
                    break;
                case SDLK_LEFT:
                    if (snake_direction & 1) {
                        snake_next_direction = 2;
                    }
                    break;
                }
            }
        }

        // Move the falling brick
        if(Utils_time() - last >= 1000 / snakeSpeed)
        {

            ++grow_count;

            if (grow_count > 4) {
                grow_count = 0;
                ++snake_length;
            }

            snake_direction = snake_next_direction;

            // Un-color the snake last position
            /* grid.cells[snakeX][snakeY].rectColor = grid.backgroundColor; */

            switch (snake_direction) {
            case 0:
                ++snakeX;
                break;
            case 3:
                ++snakeY;
                break;
            case 2:
                --snakeX;
                break;
            case 1:
                --snakeY;
                break;
            default:
                snake_direction = 0;
                ++snakeX;
                break;
            }

            snakeY = (snakeY + grid.yCells) % grid.yCells;
            snakeX = (snakeX + grid.xCells) % grid.xCells;

            snake_t *new_head = malloc(sizeof(snake_t));
            new_head->next = snake;
            snake->prev = new_head;
            new_head->x = snakeX;
            new_head->y = snakeY;
            snake = new_head;

            if (grow_count) {
                // didn't grow
                grid.cells[tail->x][tail->y].rectColor = grid.backgroundColor;

                snake_t *new_tail = tail->prev;
                free(tail);
                tail = new_tail;
            } else {
                // grew
                ++snake_length;
            }

            printf("snake %p\t\ttail %p\n", snake, tail);


            // Color the snake new position
            grid.cells[snakeX][snakeY].rectColor = COLOR_RED;

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
