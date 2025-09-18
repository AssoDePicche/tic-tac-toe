#ifndef __GAME__
#define __GAME__

#include <stdbool.h>

#define WINDOW_SIZE 640
#define WINDOW_CELL_SIZE WINDOW_SIZE / 3
#define WINDOW_FIRST_THIRD 0
#define WINDOW_SECOND_THIRD WINDOW_SIZE / 3
#define WINDOW_THIRD_THIRD 2 * WINDOW_SECOND_THIRD

typedef struct Game Game;

Game* game_construct(void);

void game_destruct(Game*);

void game_draw(const Game*);

void game_update(Game*);

bool game_is_running(const Game*);

#endif
