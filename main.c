#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MYRED                                                                  \
  CLITERAL(Color) { 255, 0, 0, 255 }
#define HEIGHT 1000
#define WIDTH 1000
#define CELLSIZE 25

short automaton_index = 0;
float grid_lines = 1.0;

#define ALIVE true
#define DEAD false
typedef struct {
  unsigned char state; // (wasted)000 (neighbor count )0000 (state)0
  Color color;

} Cell;

Cell gameGrid[HEIGHT * WIDTH] = {0};
Cell newGrid[HEIGHT * WIDTH] = {0};

typedef bool cur[9];

bool Gol[2][9] = {{DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
                   {DEAD, DEAD, ALIVE, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD}};

bool Seeds[2][9] = {
    {DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
    {DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
};

bool Som[2][9] = {
    {DEAD, DEAD, ALIVE, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
};

bool Cool[2][9] = {
    {DEAD, ALIVE, ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD, DEAD},
    {DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
};
bool Maze[2][9] = {
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
    {DEAD, ALIVE, ALIVE, ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD}};
bool MiceMaze[2][9] = {
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, ALIVE, DEAD},
    {DEAD, ALIVE, ALIVE, ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD}};
bool Mazectric[2][9] = {
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
    {DEAD, ALIVE, ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD, DEAD}};
bool MiceMazectric[2][9] = {
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, ALIVE, DEAD},
    {DEAD, ALIVE, ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD, DEAD}};

bool CoolEvil[2][9] = {
    {DEAD, ALIVE, ALIVE, ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD},
    {DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD}};

bool DayNight[2][9] = {
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
    {"Cool rule", &Cool},
    {"Day and Night", &DayNight},
    {"cool evil", &CoolEvil},
};

int countNeighbors(int i, int j) {
  int alive_count = 0;

  for (int k = -1; k <= 1; k++) {
    for (int l = -1; l <= 1; l++) {
      if (k == 0 && l == 0)
        continue;

      int row = (i + l + HEIGHT) % HEIGHT;
      int col = (j + k + WIDTH) % WIDTH;
      if (gameGrid[row * WIDTH + col].state == ALIVE) {
        alive_count++;
      }
    }
  }

  return alive_count;
}

void gen() {
  cur *automaton = type[automaton_index].ptr;
  // Cell *newGrid = calloc(WIDTH * HEIGHT, sizeof(Cell));

  memcpy(newGrid, gameGrid, sizeof(Cell) * HEIGHT * WIDTH);

  for (size_t i = 0; i < HEIGHT; i++) {
    for (size_t j = 0; j < WIDTH; j++) {

      int alive_count = countNeighbors(i, j);

      if (newGrid[i * WIDTH + j].state == ALIVE &&
          newGrid[i * WIDTH + j].state ==
              automaton[gameGrid[i * WIDTH + j].state][alive_count]) {

        if (newGrid[i * WIDTH + j].color.b != 255) {

          newGrid[i * WIDTH + j].color.b += 3;
        }
        if (newGrid[i * WIDTH + j].color.r != 0) {

          newGrid[i * WIDTH + j].color.r -= 3;
        }
      } else {
        newGrid[i * WIDTH + j].color = MYRED;
      }
      newGrid[i * WIDTH + j].state =
          automaton[gameGrid[i * WIDTH + j].state][alive_count];
    }
  }
  memcpy(gameGrid, newGrid, sizeof(Cell) * HEIGHT * WIDTH);
}

void init_grid(bool rand) {
  SetRandomSeed(time(NULL));
  for (size_t i = 0; i < HEIGHT; i++) {
    for (size_t j = 0; j < WIDTH; j++) {

      gameGrid[i * WIDTH + j].state = rand ? (GetRandomValue(0, 1)) : 0;
      gameGrid[i * WIDTH + j].color = MYRED;
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
int Round(double x) {
  if (x >= 0.0) {
    return (int)(x + 0.5);
  } else {
    return (int)(x - 0.5);
  }
}

void draw_glider(size_t height, size_t width) {
  gameGrid[(height + 0) * WIDTH + width + 1].state = ALIVE;
  gameGrid[(height + 0) * WIDTH + width + 1].color = MYRED;
  gameGrid[(height + 1) * WIDTH + width + 2].state = ALIVE;
  gameGrid[(height + 1) * WIDTH + width + 2].color = MYRED;
  gameGrid[(height + 2) * WIDTH + width + 0].state = ALIVE;
  gameGrid[(height + 2) * WIDTH + width + 0].color = MYRED;
  gameGrid[(height + 2) * WIDTH + width + 1].state = ALIVE;
  gameGrid[(height + 2) * WIDTH + width + 1].color = MYRED;
  gameGrid[(height + 2) * WIDTH + width + 2].state = ALIVE;
  gameGrid[(height + 2) * WIDTH + width + 2].color = MYRED;
}
int main() {

  bool pause = true;
  bool Pen = ALIVE;
  bool helpMenu = false;

  double prevUpdateTime = 0;
  double nowTime = 0;
  double timeBetweenUpdatesSec = 0.25;
  const double slowestUpdateSec = 2.0;
  const double fastestUpdateSec = 0.020;
  const double updateStepSec = 0.040;
  int genNum = 0;

  int screenWidth = 600;
  int screenHeight = 450;

  // Set the window to be resizable
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);

  SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  InitWindow(screenWidth, screenHeight, "Game of Life");
  init_grid(false);

  gameGrid[HEIGHT * WIDTH / 2 + WIDTH / 2].state = ALIVE;
  gameGrid[HEIGHT * WIDTH - 1].state = ALIVE;
  gameGrid[0].state = ALIVE;
  double penTime = 0;
  Camera2D camera = {0};
  camera.zoom = 1.0f;

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    if (IsWindowResized()) {
      screenWidth = GetScreenWidth();
      screenHeight = GetScreenHeight();
    }
    // Update
    // Translate based on mouse right click
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      Vector2 delta = GetMouseDelta();
      delta = Vector2Scale(delta, -1.0f / camera.zoom);
      //
      // if (camera.target.y + delta.y + 450 < HEIGHT * CELLSIZE * 0.5 &&
      //     camera.target.y + delta.y > -HEIGHT * CELLSIZE * 0.5) {
      camera.target.y += delta.y;
      // }
      // if (camera.target.x + delta.x + 450 < WIDTH * CELLSIZE * 0.5 &&
      //     camera.target.x + delta.x > -WIDTH * CELLSIZE * 0.5) {
      camera.target.x += delta.x;
      // }

      // printf("x = %f\n", camera.target.x);
      // printf("y = %f\n", camera.target.y);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsCursorOnScreen()) {
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

      int x = Floor(mouseWorldPos.x / CELLSIZE);
      int y = Floor(mouseWorldPos.y / CELLSIZE);
      x += HEIGHT / 2;
      y += WIDTH / 2;

      // printf("x %d\n", x);
      // printf("y %d\n", y);
      if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH) {

        if (Pen) {
          gameGrid[x * WIDTH + y].state = ALIVE;
          gameGrid[x * WIDTH + y].color = MYRED;
        } else
          draw_glider(x, y);
      }
    }
    //
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
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressedRepeat(KEY_SPACE)) {
      gen();
      genNum++;
    }

    switch (GetKeyPressed()) {
    case KEY_ENTER:
      penTime = GetTime();
      Pen = !Pen;
      break;
    case KEY_R:
      genNum=0;
      init_grid(true);
      break;
    case KEY_C:
      genNum=0;
      init_grid(false);
      break;
    case KEY_J:
      automaton_index = (automaton_index + TYPE_SIZE - 1) % TYPE_SIZE;
      break;
    case KEY_K:
      automaton_index += 1;
      automaton_index %= TYPE_SIZE;
      break;
    case KEY_G:
      if (grid_lines == 1)
        grid_lines -= 0.05;
      else
        grid_lines = 1.f;
      break;
    case KEY_P:
      pause = !pause;
      break;
    case KEY_MINUS:
      timeBetweenUpdatesSec += updateStepSec;
      if (timeBetweenUpdatesSec > slowestUpdateSec) {
        timeBetweenUpdatesSec = slowestUpdateSec;
      }
      break;

    case KEY_EQUAL:
      timeBetweenUpdatesSec -= updateStepSec;
      if (timeBetweenUpdatesSec < fastestUpdateSec) {
        timeBetweenUpdatesSec = fastestUpdateSec;
      }
      break;
    case KEY_H:
    case KEY_SLASH:
    case KEY_F1:
      helpMenu = true;
      break;
    }
    nowTime = GetTime();
    if (!pause && nowTime - prevUpdateTime >= timeBetweenUpdatesSec &&
        !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      gen();
      prevUpdateTime = nowTime;
      genNum++;
    }
    // Draw
    BeginDrawing();
    ClearBackground(DARKGRAY);

    BeginMode2D(camera);

    Vector2 windStart = GetWorldToScreen2D(Vector2Zero(), camera);
    int windStartX = Round(-(windStart.x / CELLSIZE) / camera.zoom);
    int windStartY = Round(-(windStart.y / CELLSIZE) / camera.zoom);
    int windEndX =
        windStartX + (int)((screenWidth / CELLSIZE) + 1) / camera.zoom;
    int windEndY =
        windStartY + (int)((screenHeight / CELLSIZE) + 1) / camera.zoom;

    for (int i = windStartX - 1; i <= windEndX; i++) {

      for (int j = windStartY - 1; j <= windEndY; j++) {

        if ((i < -HEIGHT / 2 || j < -WIDTH / 2) ||
            (i >= HEIGHT / 2 || j >= WIDTH / 2)) {
          continue;
        }
        switch (gameGrid[(i + HEIGHT / 2) * WIDTH + j + WIDTH / 2].state) {

        case DEAD:
          DrawRectangle(i * CELLSIZE, j * CELLSIZE, CELLSIZE * grid_lines,
                        CELLSIZE * grid_lines, WHITE);
          break;
        case ALIVE:
          DrawRectangle(
              i * CELLSIZE, j * CELLSIZE, CELLSIZE * grid_lines,
              CELLSIZE * grid_lines,
              gameGrid[(i + HEIGHT / 2) * WIDTH + j + WIDTH / 2].color);

          break;
        }
      }
    }

    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

    float x = Floor(mouseWorldPos.x / CELLSIZE);
    float y = Floor(mouseWorldPos.y / CELLSIZE);

    if (x >= -HEIGHT / 2.f && x < HEIGHT / 2.f && y >= -WIDTH / 2.f &&
        y < WIDTH / 2.f) {
      DrawRectangle(x * CELLSIZE, y * CELLSIZE, CELLSIZE * 1, CELLSIZE * 1,
                    GRAY);
    }

    EndMode2D();

    if (GetTime() - penTime <= 0.3) {
      if (Pen) {

        DrawText("Switched pen to Alive", 10, 10, 20, PURPLE);
      } else
        DrawText("Switched pen to Glider", 10, 10, 20, PURPLE);
    }

    DrawRectangle(
        10 - 5, GetScreenHeight() * .9f - 5,
        MeasureText(TextFormat("Automaton: %s ", type[automaton_index].arg),
                    20) +
            10,
        30, Fade(WHITE, 0.8f));

    DrawText(TextFormat("Automaton: %s", type[automaton_index].arg), 15,
             GetScreenHeight() * .9f, 20, PURPLE);

    DrawRectangle(10 - 5, GetScreenHeight() * .95f - 5, 110, 30,
                  Fade(WHITE, 0.8f));

    DrawText(TextFormat("Gen: %d", genNum), 15, GetScreenHeight() * .95f, 20,
             PURPLE);
    EndDrawing();
    while (helpMenu) {
      BeginDrawing();

      ClearBackground(RAYWHITE);
      DrawText("Help Menu",
               GetScreenWidth() / 2 - MeasureText("Help Menu", 30) / 2, 10, 30,
               DARKGRAY);

      // Menu items
      int y = 70;
      int spacing = 25;

      DrawText("- Press H to show this help menu", 30, y += spacing, 20,
               DARKGRAY);
      DrawText("- Right-click and drag to move the camera", 30, y += spacing,
               20, DARKGRAY);
      DrawText("- Scroll wheel to zoom in/out", 30, y += spacing, 20, DARKGRAY);
      DrawText("- Press and hold LEFT MOUSE BUTTON to draw in the grid", 30,
               y += spacing, 20, DARKGRAY);
      DrawText("- Use the mouse wheel to zoom in/out at the cursor position",
               30, y += spacing, 20, DARKGRAY);

      DrawText("- Press SPACE to generate the next iteration", 30, y += spacing,
               20, DARKGRAY);

      DrawText("- Press ENTER to toggle the pen for drawing on the grid", 30,
               y += spacing, 20, DARKGRAY);

      DrawText("- Press R to initialize the grid with random cells", 30,
               y += spacing, 20, DARKGRAY);
      DrawText("- Press C to clear the grid", 30, y += spacing, 20, DARKGRAY);
      DrawText("- Press J/K to cycle through different automaton types", 30,
               y += spacing, 20, DARKGRAY);
      DrawText("- Press G to toggle grid boredom effect", 30, y += spacing, 20,
               DARKGRAY);
      DrawText("- Press P to pause/unpause simulation", 30, y += spacing, 20,
               DARKGRAY);
      DrawText("- Press +/- to adjust simulation speed", 30, y += spacing, 20,
               DARKGRAY);

      DrawText("- Press any key to go back", 30, y += spacing * 2, 30,
               DARKGRAY);
      EndDrawing();
      if (GetKeyPressed()) {
        helpMenu = false;
      }
    }
  }

  CloseWindow();
  return 0;
}
