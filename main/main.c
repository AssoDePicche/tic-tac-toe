#include <raylib.h>

#include "game.h"

int main(void) {
  Game* game = game_construct();

  InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Tic-tac-toe");

  SetTargetFPS(60);

  SetExitKey(KEY_Q);

  while (!WindowShouldClose() && is_running(game)) {
    game_input(game);

    game_update(game);

    BeginDrawing();

    ClearBackground(WHITE);

    game_draw(game);

    EndDrawing();
  }

  game_destruct(game);

  return 0;
}
