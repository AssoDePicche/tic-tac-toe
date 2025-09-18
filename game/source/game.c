#include "game.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define BOARD_PLAYER_1 'X'
#define BOARD_PLAYER_2 'O'
#define BOARD_BLANK '_'
#define BOARD_ROWS 3
#define BOARD_COLUMNS 3

typedef enum {
  GAME_STATE_PLAYER_1_WON,
  GAME_STATE_PLAYER_2_WON,
  GAME_STATE_DRAW,
  GAME_STATE_UNFINISHED,
} GameState;

struct Game {
  GameState state;
  char* buffer;
};

Game* game_construct(void) {
  Game* this = (Game*)malloc(sizeof(Game));

  this->buffer = (char*)malloc(sizeof(char) * BOARD_ROWS * BOARD_COLUMNS);

  for (size_t index = 0; index < BOARD_ROWS * BOARD_COLUMNS; ++index) {
    this->buffer[index] = BOARD_BLANK;
  }

  this->state = GAME_STATE_UNFINISHED;

  return this;
}

void game_destruct(Game* this) {
  free(this->buffer);

  free(this);
}

static void draw_game_board(void) {
  DrawLine(0, WINDOW_SECOND_THIRD, WINDOW_SIZE, WINDOW_SECOND_THIRD, BLACK);

  DrawLine(0, WINDOW_THIRD_THIRD, WINDOW_SIZE, WINDOW_THIRD_THIRD, BLACK);

  DrawLine(WINDOW_SECOND_THIRD, 0, WINDOW_SECOND_THIRD, WINDOW_SIZE, BLACK);

  DrawLine(WINDOW_THIRD_THIRD, 0, WINDOW_THIRD_THIRD, WINDOW_SIZE, BLACK);
}

static void draw_player_1(const int x, const int y) {
  DrawLine(0 + x, 0 + y, WINDOW_CELL_SIZE + x, WINDOW_CELL_SIZE + y, RED);

  DrawLine(WINDOW_CELL_SIZE + x, 0 + y, 0 + x, WINDOW_CELL_SIZE + y, RED);
}

static void draw_player_2(const int x, const int y) {
  const int RADIUS = WINDOW_CELL_SIZE / 2;

  DrawCircleLines(x + RADIUS, y + RADIUS, RADIUS, BLUE);
}

static int current_cell_third(int point) {
  if (point < WINDOW_SECOND_THIRD) {
    return WINDOW_FIRST_THIRD;
  }

  if (point < WINDOW_THIRD_THIRD) {
    return WINDOW_SECOND_THIRD;
  }

  return WINDOW_THIRD_THIRD;
}

static void draw_hover(void) {
  const Vector2 mouse = GetMousePosition();

  const int x = current_cell_third(mouse.x);

  const int y = current_cell_third(mouse.y);

  DrawRectangle(x, y, WINDOW_CELL_SIZE, WINDOW_CELL_SIZE, BLUE);
}

void game_draw(const Game* this) {
  draw_hover();

  draw_game_board();

  for (size_t index = 0; index < BOARD_ROWS * BOARD_COLUMNS; ++index) {
    const int x = (index % BOARD_ROWS) * WINDOW_CELL_SIZE;

    const int y = (index / BOARD_COLUMNS) * WINDOW_CELL_SIZE;

    switch (this->buffer[index]) {
      case BOARD_PLAYER_1:
        draw_player_1(x, y);
        break;
      case BOARD_PLAYER_2:
        draw_player_2(x, y);
        break;
      case BOARD_BLANK:
        break;
    }
  }
}

static size_t index_2d_to_1d(const size_t x, const size_t y) {
  return BOARD_ROWS * y + x;
}

static bool cells_are_equal(const Game* this, const size_t i, const size_t j,
                            const size_t k, const char buffer) {
  return this->buffer[i] == this->buffer[j] &&
         this->buffer[i] == this->buffer[k] && this->buffer[i] == buffer;
}

static bool row_cells_are_equal(const Game* this, const size_t row,
                                const char player) {
  const size_t i = index_2d_to_1d(0, row);

  const size_t j = index_2d_to_1d(1, row);

  const size_t k = index_2d_to_1d(2, row);

  return cells_are_equal(this, i, j, k, player);
}

static bool column_cells_are_equal(const Game* this, const size_t column,
                                   const char player) {
  const size_t i = index_2d_to_1d(column, 0);

  const size_t j = index_2d_to_1d(column, 1);

  const size_t k = index_2d_to_1d(column, 2);

  return cells_are_equal(this, i, j, k, player);
}

static bool primary_diagonal_cells_are_equal(const Game* this,
                                             const char player) {
  const size_t i = index_2d_to_1d(0, 0);

  const size_t j = index_2d_to_1d(1, 1);

  const size_t k = index_2d_to_1d(2, 2);

  return cells_are_equal(this, i, j, k, player);
}

static bool secondary_diagonal_cells_are_equal(const Game* this,
                                               const char player) {
  const size_t i = index_2d_to_1d(0, 2);

  const size_t j = index_2d_to_1d(1, 1);

  const size_t k = index_2d_to_1d(2, 0);

  return cells_are_equal(this, i, j, k, player);
}

static bool have_player_won(const Game* this, const char player) {
  if (primary_diagonal_cells_are_equal(this, player)) {
    return true;
  }

  if (secondary_diagonal_cells_are_equal(this, player)) {
    return true;
  }

  for (size_t index = 0; index < BOARD_ROWS; ++index) {
    if (row_cells_are_equal(this, index, player)) {
      return true;
    }

    if (column_cells_are_equal(this, index, player)) {
      return true;
    }
  }

  return false;
}

static GameState current_game_state(const Game* game) {
  if (have_player_won(game, BOARD_PLAYER_1)) {
    return GAME_STATE_PLAYER_1_WON;
  }

  if (have_player_won(game, BOARD_PLAYER_2)) {
    return GAME_STATE_PLAYER_2_WON;
  }

  // TODO: check for game_draw

  return GAME_STATE_UNFINISHED;
}

static void game_input(Game* this) {
  if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
      !IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    return;
  }

  const Vector2 position = GetMousePosition();

  const int x = (int)(BOARD_COLUMNS * position.x / WINDOW_SIZE);

  const int y = (int)(BOARD_ROWS * position.y / WINDOW_SIZE);

  const size_t index = index_2d_to_1d(x, y);

  if (this->buffer[index] != BOARD_BLANK) {
    return;
  }

  const char player =
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ? BOARD_PLAYER_1 : BOARD_PLAYER_2;

  this->buffer[index] = player;
}

void game_update(Game* this) {
  game_input(this);

  this->state = current_game_state(this);
}

bool game_is_running(const Game* this) {
  return this->state == GAME_STATE_UNFINISHED;
}
