#include <stdlib.h>
#include <time.h>

#include "game.h"

typedef struct snake_body {
    int x;
    int y;
    struct snake_body *prev;
} snake_body_t;

typedef enum {
    DIR_RIGHT,
    DIR_UP,
    DIR_LEFT,
    DIR_DOWN
} SnakeDirection;

typedef struct snake_state {
    int x;
    int y;
    int speed;
    int direction;
} snake_t;

bool dir_is_horiz(SnakeDirection direction) {
    return direction == DIR_LEFT || direction == DIR_RIGHT;
}

void snake_set_direction(snake_t *snake, SnakeDirection direction) {
    bool allowed =
        (dir_is_horiz(direction) && !dir_is_horiz(snake->direction))
        || (!dir_is_horiz(direction) && dir_is_horiz(snake->direction));
    if (allowed) {
        snake->direction = direction;
    }
}

void snake_advance(snake_t *snake) {
    switch (snake->direction) {
    case DIR_RIGHT:
        ++snake->x;
        break;
    case DIR_DOWN:
        ++snake->y;
        break;
    case DIR_LEFT:
        --snake->x;
        break;
    case DIR_UP:
        --snake->y;
        break;
    default:
        snake->direction = DIR_RIGHT;
        ++snake->x;
        break;
    }
}

void snake_wrap_position(snake_t *snake, int x_max, int y_max) {
    while (snake->x > x_max) {
        snake->x -= x_max;
    }
    while (snake->x < 0) {
        snake->x += x_max;
    }
    while (snake->y > y_max) {
        snake->y -= y_max;
    }
    while (snake->y < 0) {
        snake->y += y_max;
    }
}

bool cell_is_food(Cell *cell) {
    return !memcmp(&cell->rectColor, &COLOR_GREEN, sizeof(SDL_Color));
}

bool cell_is_snake(Cell *cell) {
    return !memcmp(&cell->rectColor, &COLOR_RED, sizeof(SDL_Color));
}

void place_snake_food(Grid *grid) {
    int x = grid->xCells * (((float)rand()) / RAND_MAX);
    int y = grid->yCells * (((float)rand()) / RAND_MAX);

    // Naively search for a non-snake cell.
    while (cell_is_snake(&grid->cells[x][y])) {
        x = grid->xCells * (((float)rand()) / RAND_MAX);
        y = grid->yCells * (((float)rand()) / RAND_MAX);
    }

    grid->cells[x][y].rectColor = COLOR_GREEN;
}

snake_body_t *move_head(snake_body_t *head, int x, int y) {
    snake_body_t *new_head = malloc(sizeof(snake_body_t));
    head->prev = new_head;
    new_head->x = x;
    new_head->y = y;
    return new_head;
}

snake_body_t *move_tail(snake_body_t *tail) {
    snake_body_t *new_tail = tail->prev;
    free(tail);
    return new_tail;
}

bool Game_start(SDL_Renderer *renderer, int w, int h)
{
    srand(time(0));

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

    // Snake state
    snake_t snake;
    snake.x = grid.xCells / 2;
    snake.y = grid.yCells / 2;
    snake.speed = 2;
    snake.direction = DIR_RIGHT;

    snake_body_t *head = malloc(sizeof(snake_body_t));
    head->x = snake.x;
    head->y = snake.y;
    snake_body_t *tail = head;

    place_snake_food(&grid);

    // Event loop exit flag
    bool quit = false;

    SnakeDirection new_direction = snake.direction;

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
                    new_direction = DIR_RIGHT;
                    break;
                case SDLK_DOWN:
                    new_direction = DIR_DOWN;
                    break;
                case SDLK_UP:
                    new_direction = DIR_UP;
                    break;
                case SDLK_LEFT:
                    new_direction = DIR_LEFT;
                    break;
                }
            }
        }

        // Move the snake
        if (Utils_time() - last >= 1000 / snake.speed)
        {
            snake_set_direction(&snake, new_direction);

            snake_advance(&snake);

            snake_wrap_position(&snake, grid.xCells, grid.yCells);

            head = move_head(head, snake.x, snake.y);

            if (cell_is_food(&grid.cells[snake.x][snake.y])) {
                // grew; tail stays where it is; make new food appear
                place_snake_food(&grid);
            } else if (cell_is_snake(&grid.cells[snake.x][snake.y])) {
                // snake tried to eat itself
                printf("Snake is not allowed to eat itself.\n");
                return false;
            } else {
                // didn't grow; move the tail along
                grid.cells[tail->x][tail->y].rectColor = grid.backgroundColor;
                tail = move_tail(tail);
            }

            // Color the snake new position
            grid.cells[snake.x][snake.y].rectColor = COLOR_RED;

            last = Utils_time();
        }

        // Set background color
        Utils_setBackgroundColor(renderer, COLOR_DARK_GRAY);

        // Render grid
        Grid_render(&grid, renderer);

        // Show message
        stringRGBA(renderer, grid.rect.x + grid.xCells, grid.rect.y - 20,
                   "Move the snake with UP, DOWN, LEFT, and RIGHT. Eat food. Don't eat snake.",
                   COLOR_LIGHT_GRAY.r, COLOR_LIGHT_GRAY.g, COLOR_LIGHT_GRAY.b, COLOR_LIGHT_GRAY.a);

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
    }

    return true;
}
