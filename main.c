#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MYRED                                                                  \
  CLITERAL(Color) { 255, 0, 0, 255 }
#define WIDTH 500
#define HEIGHT 500
#define CELLSIZE 25

#define screenWidth 450
#define screenHeight 450
short automaton_index = 0;

typedef enum { DEAD, ALIVE } State;
typedef struct {
  State state : 1;
  Color color;

} Cell;

Cell gameGrid[WIDTH][HEIGHT] = {0};
Cell newGrid[WIDTH][HEIGHT] = {0};

typedef State cur[9];

State Gol[2][9] = {{DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
                   {DEAD, DEAD, ALIVE, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD}};

State Seeds[2][9] = {
    {DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
    {DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
};

State Som[2][9] = {
    {DEAD, DEAD, ALIVE, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
};

State Rule30[2][9] = {
    {DEAD, ALIVE, ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD, DEAD},
    {DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
};
State Maze[2][9] = {
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
    {DEAD, ALIVE, ALIVE, ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD}};
State MiceMaze[2][9] = {
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, ALIVE, DEAD},
    {DEAD, ALIVE, ALIVE, ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD}};
State Mazectric[2][9] = {
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
    {DEAD, ALIVE, ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD, DEAD}};
State MiceMazectric[2][9] = {
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, ALIVE, DEAD},
    {DEAD, ALIVE, ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD, DEAD}};
State CoolEvil[2][9] = {
    {DEAD, ALIVE, ALIVE, ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD},
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD}};
State DayNight[2][9] = {
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, ALIVE, ALIVE, ALIVE},
    {DEAD, DEAD, DEAD, ALIVE, ALIVE, DEAD, ALIVE, ALIVE, ALIVE},
};

typedef struct {
  char *arg;
  void *ptr;
} Type;

#define TYPE_SIZE 10
Type type[TYPE_SIZE] = {
    {"Game of life", &Gol},
    {"Seeds", &Seeds},
    {"test", &Som},
    {"Maze", &Maze},
    {"Mice Maze", &MiceMaze},
    {"Mazectric", &Mazectric},
    {"Mice Mazectric", &MiceMazectric},
    {"Rule 30", &Rule30},
    {"Day and Night", &DayNight},
    {"cool evil", &CoolEvil},
};

int countNeighbors(int i, int j) {
  int alive_count = 0;

  for (int k = -1; k <= 1; k++) {
    for (int l = -1; l <= 1; l++) {
      if (k == 0 && l == 0)
        continue;
      int row = (i + k + HEIGHT) % HEIGHT;
      int col = (j + l + WIDTH) % WIDTH;
      if (gameGrid[row][col].state == ALIVE) {
        alive_count++;
      }
    }
  }

  return alive_count;
}
void gen() {

  cur *automaton = type[automaton_index].ptr;
  memcpy(newGrid, gameGrid, sizeof(Cell) * WIDTH * HEIGHT);

  for (size_t i = 0; i < WIDTH; i++) {
    for (size_t j = 0; j < HEIGHT; j++) {

      int alive_count = countNeighbors(i, j);

      if (newGrid[i][j].state == ALIVE &&
          newGrid[i][j].state == automaton[gameGrid[i][j].state][alive_count]) {

        if (newGrid[i][j].color.b != 255) {

          newGrid[i][j].color.b += 3;
        }
        if (newGrid[i][j].color.r != 0) {

          newGrid[i][j].color.r -= 3;
        }
      } else {
        newGrid[i][j].color = MYRED;
      }
      newGrid[i][j].state = automaton[gameGrid[i][j].state][alive_count];
    }
  }
  memcpy(gameGrid, newGrid, sizeof(Cell) * WIDTH * HEIGHT);
}
void init_grid(bool rand) {
  SetRandomSeed(time(NULL));
  for (size_t i = 0; i < WIDTH; i++) {
    for (size_t j = 0; j < HEIGHT; j++) {

      gameGrid[i][j].state = rand ? (State)(GetRandomValue(0, 1)) : 0;
      gameGrid[i][j].color = MYRED;
    }
  }
}
int Floor(double x) {
  if (x >= 0.0) {
    return (int)x;
  } else {
    int intPart = (int)(x);
    return (x == intPart) ? (int)x : intPart - 1;
  }
}
int main() {

  State Pen = ALIVE;

  InitWindow(screenWidth, screenHeight, "Game of Life");
  init_grid(false);

  // gameGrid[250][250].state = ALIVE;
  double penTime = 0;
  Camera2D camera = {0};
  camera.zoom = 1.0f;

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // Update

    // Translate based on mouse right click
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      Vector2 delta = GetMouseDelta();
      delta = Vector2Scale(delta, -1.0f / camera.zoom);

      if (camera.target.y + delta.y + 450 < WIDTH * CELLSIZE * 0.5 &&
          camera.target.y + delta.y > -WIDTH * CELLSIZE * 0.5) {
        camera.target.y += delta.y;
      }
      if (camera.target.x + delta.x + 450 < HEIGHT * CELLSIZE * 0.5 &&
          camera.target.x + delta.x > -HEIGHT * CELLSIZE * 0.5) {
        camera.target.x += delta.x;
      }

      // printf("x = %f\n", camera.target.x);
      // printf("y = %f\n", camera.target.y);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

      int x = Floor(mouseWorldPos.x / CELLSIZE);
      int y = Floor(mouseWorldPos.y / CELLSIZE);

      if (x >= -WIDTH / 2 && x < WIDTH / 2 && y >= -HEIGHT / 2 && y < HEIGHT / 2) {

        gameGrid[x + WIDTH / 2][y + HEIGHT / 2].state = Pen;
        gameGrid[x + WIDTH / 2][y + HEIGHT / 2].color = MYRED;
      }
    }
    // Zoom based on mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
      // Get the world point that is under the mouse
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

      // Set the offset to where the mouse is
      camera.offset = GetMousePosition();

      // Set the target to match, so that the camera maps the world space
      // point under the cursor to the screen space point under the cursor at
      // any zoom
      camera.target = mouseWorldPos;

      // Zoom increment
      const float zoomIncrement = 0.125f;

      camera.zoom += (wheel * zoomIncrement);
      if (camera.zoom < zoomIncrement)
        camera.zoom = zoomIncrement;
    }
    if (IsKeyPressed(KEY_ENTER)) {
      penTime = GetTime();
      Pen = !Pen;
    }
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressedRepeat(KEY_SPACE)) {
      gen();
    }
    if (IsKeyPressed(KEY_R)) {

      init_grid(true);
    }

    if (IsKeyPressed(KEY_C)) {

      init_grid(false);
    }
    if (IsKeyPressed(KEY_J)) {
      automaton_index = (automaton_index + TYPE_SIZE - 1) % TYPE_SIZE;
    }
    if (IsKeyPressed(KEY_K)) {

      automaton_index += 1;
      automaton_index %= TYPE_SIZE;
    }
    // Draw
    BeginDrawing();
    ClearBackground(DARKGRAY);

    BeginMode2D(camera);
    for (int i = -WIDTH / 2; i < WIDTH / 2; i++) {

      for (int j = -HEIGHT / 2; j < HEIGHT / 2; j++) {

        switch (gameGrid[i + WIDTH / 2][j + HEIGHT / 2].state) {
        case DEAD:
          DrawRectangle(i * CELLSIZE, j * CELLSIZE, CELLSIZE * 1, CELLSIZE * 1,
                        WHITE);
          break;
        case ALIVE:
          DrawRectangle(i * CELLSIZE, j * CELLSIZE, CELLSIZE * 1, CELLSIZE * 1,
                        gameGrid[i + WIDTH / 2][j + HEIGHT / 2].color);

          break;
        }
      }
    }

    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

    float x = Floor(mouseWorldPos.x / CELLSIZE);
    float y = Floor(mouseWorldPos.y / CELLSIZE);

    if (x >= -WIDTH / 2.f && x < WIDTH / 2.f && y >= -HEIGHT / 2.f && y < HEIGHT / 2.f) {
      DrawRectangle(x * CELLSIZE, y * CELLSIZE, CELLSIZE * 1, CELLSIZE * 1,
                    GRAY);
    }

    EndMode2D();
    // DrawText("Mouse right button drag to move\n\nMouse left button toggle
    // cell state\n\nMouse wheel to zoom", 10, 10, 20,PURPLE);
    //
    if (GetTime() - penTime <= 0.3) {
      if ((bool)Pen) {

        DrawText("Switched pen to Alive", 10, 10, 20, PURPLE);
      } else
        DrawText("Switched pen to Dead", 10, 10, 20, PURPLE);
    }
    DrawText(TextFormat("automaton: %s", type[automaton_index].arg), 10,
             GetScreenHeight() * .9f, 20, PURPLE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
