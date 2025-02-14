#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MYRED                                                                  \
  (Color) { 255, 0, 0, 255 }
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

Cell game_grid[HEIGHT * WIDTH] = {0};
Cell new_grid[HEIGHT * WIDTH] = {0};

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
bool Maze[2][9] = {{DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
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

void set_cell(int height, int width) {
  int grid_address = height * WIDTH + width;
  if ((game_grid[grid_address].state & 0x01)) {

    if (game_grid[grid_address].color.b != 255) {

      game_grid[grid_address].color.b += 3;
    }
    if (game_grid[grid_address].color.r != 0) {

      game_grid[grid_address].color.r -= 3;
    }

    return;
  }

  // Calculate the offsets to the eight neighboring cells,
  // accounting for wrapping around at the edges of the cell map
  int xoleft = (width == 0) ? WIDTH - 1 : -1;
  int xoright = (width == (WIDTH - 1)) ? -(WIDTH - 1) : 1;
  int yoabove = (height == 0) ? WIDTH * HEIGHT - WIDTH : -WIDTH;
  int yobelow = (height == (HEIGHT - 1)) ? -(WIDTH * HEIGHT - WIDTH) : WIDTH;

  game_grid[grid_address].state |= 0x01; // Set first bit to 1

  // Change successive bits for neighbour counts
  game_grid[grid_address + yoabove + xoleft].state += 0x02;
  game_grid[grid_address + yoabove].state += 0x02;
  game_grid[grid_address + yoabove + xoright].state += 0x02;
  game_grid[grid_address + xoleft].state += 0x02;
  game_grid[grid_address + xoright].state += 0x02;
  game_grid[grid_address + yobelow + xoleft].state += 0x02;
  game_grid[grid_address + yobelow].state += 0x02;
  game_grid[grid_address + yobelow + xoright].state += 0x02;
}

void clearCell(int height, int width) {
  int grid_address = height * WIDTH + width;
  game_grid[grid_address].color = MYRED;
  if (!(game_grid[grid_address].state & 0x01)) {

    return;
  }
  // Calculate the offsets to the eight neighboring cells,
  // accounting for wrapping around at the edges of the cell map
  int xoleft = (width == 0) ? WIDTH - 1 : -1;
  int xoright = (width == (WIDTH - 1)) ? -(WIDTH - 1) : 1;
  int yoabove = (height == 0) ? WIDTH * HEIGHT - WIDTH : -WIDTH;
  int yobelow = (height == (HEIGHT - 1)) ? -(WIDTH * HEIGHT - WIDTH) : WIDTH;

  game_grid[grid_address].state &= ~0x01; // Set first bit to 1
  // Change successive bits for neighbour counts
  game_grid[grid_address + yoabove + xoleft].state -= 0x02;
  game_grid[grid_address + yoabove].state -= 0x02;
  game_grid[grid_address + yoabove + xoright].state -= 0x02;
  game_grid[grid_address + xoleft].state -= 0x02;
  game_grid[grid_address + xoright].state -= 0x02;
  game_grid[grid_address + yobelow + xoleft].state -= 0x02;
  game_grid[grid_address + yobelow].state -= 0x02;
  game_grid[grid_address + yobelow + xoright].state -= 0x02;
}

void gen() {
  cur *automaton = type[automaton_index].ptr;

  memcpy(new_grid, game_grid, sizeof(Cell) * HEIGHT * WIDTH);

  for (size_t i = 0; i < HEIGHT; i++) {
    for (size_t j = 0; j < WIDTH; j++) {

      if (!new_grid[i * WIDTH + j].state) {
        continue;
      }
      int alive_count = new_grid[i * WIDTH + j].state >> 1;
      if (automaton[game_grid[i * WIDTH + j].state & 0x01][alive_count]) {

        set_cell(i, j);
      } else {

        clearCell(i, j);
      }
    }
  }
}

void init_grid(bool rand) {
  SetRandomSeed(time(NULL));
  for (size_t i = 0; i < HEIGHT; i++) {
    for (size_t j = 0; j < WIDTH; j++) {
      game_grid[i * WIDTH + j].color = MYRED;
      game_grid[i * WIDTH + j].state = 0;
    }
  }
  if (rand) {
    for (size_t i = 0; i < HEIGHT; i++) {
      for (size_t j = 0; j < WIDTH; j++) {
        if (GetRandomValue(0, 1)) {
          set_cell(i, j);
        }
      }
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
  set_cell(height + 0, width + 1);
  game_grid[(height + 0) * WIDTH + width + 1].color = MYRED;
  set_cell(height + 1, width + 2);
  game_grid[(height + 1) * WIDTH + width + 2].color = MYRED;
  set_cell(height + 2, width + 0);
  game_grid[(height + 2) * WIDTH + width + 0].color = MYRED;
  set_cell(height + 2, width + 1);
  game_grid[(height + 2) * WIDTH + width + 1].color = MYRED;
  set_cell(height + 2, width + 2);
  game_grid[(height + 2) * WIDTH + width + 2].color = MYRED;
}
int main() {

  bool pause = true;
  bool pen = ALIVE;
  bool help_menu = false;

  double prev_update_time = 0;
  double now_time = 0;
  double timeBetweenUpdatesSec = 0.25;
  const double slowestUpdateSec = 2.0;
  const double fastestUpdateSec = 0.001;
  const double updateStepSec = 0.040;
  int genNum = 0;

  int screen_width = 600;
  int screen_height = 450;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_WINDOW_UNDECORATED);

  InitWindow(screen_width, screen_height, "Game of Life");
  init_grid(false);
  set_cell(HEIGHT / 2, WIDTH / 2);
  set_cell(0, 0);
  double pen_time = 0;
  Camera2D camera = {0};
  camera.zoom = 1.0f;

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    if (IsWindowResized()) {
      screen_width = GetScreenWidth();
      screen_height = GetScreenHeight();
    }

    // Update
    // Translate based on mouse right click
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      Vector2 delta = GetMouseDelta();
      delta = Vector2Scale(delta, -1.0f / camera.zoom);

      camera.target.y += delta.y;
      camera.target.x += delta.x;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsCursorOnScreen()) {
      Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);

      int x = Floor(mouse_world_pos.x / CELLSIZE);
      int y = Floor(mouse_world_pos.y / CELLSIZE);
      x += WIDTH / 2;
      y += HEIGHT / 2;

      if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {

        if (pen) {
          set_cell(y, x);
          game_grid[y * WIDTH + x].color = MYRED;
        } else {

          // clearCell(y, x);
          draw_glider(y, x);
        }
      }
    }
    //
    // Zoom based on mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
      // Get the world point that is under the mouse
      Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);

      // Set the offset to where the mouse is
      camera.offset = GetMousePosition();

      // Set the target to match, so that the camera maps the world space
      // point under the cursor to the screen space point under the cursor at
      // any zoom
      camera.target = mouse_world_pos;

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
      pen_time = GetTime();
      pen = !pen;
      break;
    case KEY_R:
      genNum = 0;
      init_grid(true);
      break;
    case KEY_C:
      genNum = 0;
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
      help_menu = true;
      break;
    }
    now_time = GetTime();
    if (!pause && now_time - prev_update_time >= timeBetweenUpdatesSec &&
        !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      gen();
      prev_update_time = now_time;
      genNum++;
    }
    // Draw
    BeginDrawing();
    ClearBackground(DARKGRAY);

    BeginMode2D(camera);

    Vector2 wind_start = GetWorldToScreen2D(Vector2Zero(), camera);
    int wind_start_x = Round(-(wind_start.x / CELLSIZE) / camera.zoom);
    int wind_start_y = Round(-(wind_start.y / CELLSIZE) / camera.zoom);
    int wind_end_x =
        wind_start_x + (int)((screen_width / CELLSIZE) + 1) / camera.zoom;
    int wind_end_y =
        wind_start_y + (int)((screen_height / CELLSIZE) + 1) / camera.zoom;

    for (int i = wind_start_x - 1; i <= wind_end_x; i++) {

      for (int j = wind_start_y - 1; j <= wind_end_y; j++) {

        if ((i < -WIDTH / 2 || j < -HEIGHT / 2) ||
            (i >= WIDTH / 2 || j >= HEIGHT / 2)) {
          continue;
        }
        switch (game_grid[(j + HEIGHT / 2) * WIDTH + i + WIDTH / 2].state & 0x01) {

        case DEAD:
          DrawRectangle(i * CELLSIZE, j * CELLSIZE, CELLSIZE * grid_lines,
                        CELLSIZE * grid_lines, WHITE);
          break;
        case ALIVE:
          DrawRectangle(
              i * CELLSIZE, j * CELLSIZE, CELLSIZE * grid_lines,
              CELLSIZE * grid_lines,
              game_grid[(j + HEIGHT / 2) * WIDTH + i + WIDTH / 2].color);

          break;
        }
      }
    }

    Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);

    float x = Floor(mouse_world_pos.x / CELLSIZE);
    float y = Floor(mouse_world_pos.y / CELLSIZE);

    if (x >= -WIDTH / 2.f && x < WIDTH / 2.f && y >= -HEIGHT / 2.f &&
        y < HEIGHT / 2.f) {
      DrawRectangle(x * CELLSIZE, y * CELLSIZE, CELLSIZE * 1, CELLSIZE * 1,
                    GRAY);
    }

    EndMode2D();

    if (GetTime() - pen_time <= 0.3) {
      if (pen) {
        DrawText("Switched pen to Alive", 10, 10, 20, PURPLE);
      } else
        DrawText("Switched pen to Glider", 10, 10, 20, PURPLE);
    }

    DrawRectangle(10 - 5, GetScreenHeight() * .9f - 5,
        MeasureText(TextFormat("Automaton: %s ", type[automaton_index].arg),20) +
        10,30, Fade(WHITE, 0.8f));

    DrawText(TextFormat("Automaton: %s", type[automaton_index].arg), 15,
             GetScreenHeight() * .9f, 20, PURPLE);

    DrawRectangle(10 - 5, GetScreenHeight() * .95f - 5, 110, 30,
                  Fade(WHITE, 0.8f));

    DrawText(TextFormat("Gen: %d", genNum), 15, GetScreenHeight() * .95f, 20,PURPLE);
    EndDrawing();
    while (help_menu) {
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
        help_menu = false;
      }
    }
  }

  CloseWindow();
  return 0;
}
