#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <time.h>

#define COL 50
#define ROW 50
#define CELLSIZE 50

typedef enum {
  DEAD,
  ALIVE,
} State;

State gameGrid[COL][ROW];
void rand_grid() {
  SetRandomSeed(time(NULL));
  for (size_t i = 0; i < ROW; i++) {
    for (size_t j = 0; j < COL; j++) {

      gameGrid[i][j] = (State)(GetRandomValue(0, 1));
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
  const int screenWidth = 800;
  const int screenHeight = 450;

  // rand_grid();

  InitWindow(screenWidth, screenHeight, "Game of Life");

  Camera2D camera = {0};
  camera.zoom = 1.0f;

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // Update

    // Translate based on mouse right click
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      Vector2 delta = GetMouseDelta();
      delta = Vector2Scale(delta, -1.0f / camera.zoom);

      camera.target = Vector2Add(camera.target, delta);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

      int x = Floor(mouseWorldPos.x / CELLSIZE);
      int y = Floor(mouseWorldPos.y / CELLSIZE);
      if (x >= -COL / 2 && x < COL / 2 && y >= -ROW / 2 && y < ROW / 2) {

        gameGrid[x + COL / 2][y + ROW / 2] =IsMouseButtonDown(MOUSE_LEFT_BUTTON) ?ALIVE:DEAD;
      } else {
        printf("OUT OF BOUNDS\n");
      }
    }

    // Zoom based on mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
      // Get the world point that is under the mouse
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

      // Set the offset to where the mouse is
      camera.offset = GetMousePosition();

      // Set the target to match, so that the camera maps the world space point
      // under the cursor to the screen space point under the cursor at any zoom
      camera.target = mouseWorldPos;

      // Zoom increment
      const float zoomIncrement = 0.125f;

      camera.zoom += (wheel * zoomIncrement);
      if (camera.zoom < zoomIncrement)
        camera.zoom = zoomIncrement;
    }

    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);

    for (int i = -COL / 2; i < COL / 2; i++) {

      for (int j = -ROW / 2; j < ROW / 2; j++) {

        switch (gameGrid[i + COL / 2][j + ROW / 2]) {
        case DEAD:
          DrawRectangle(i * CELLSIZE, j * CELLSIZE, CELLSIZE * 0.95,
                        CELLSIZE * 0.95, WHITE);
          break;
        case ALIVE:
          DrawRectangle(i * CELLSIZE, j * CELLSIZE, CELLSIZE * 0.95,
                        CELLSIZE * 0.95, DARKGRAY);
          break;
        }
      }
    }

    EndMode2D();

    // DrawText("Mouse right button drag to move\n\nMouse left button toggle
    // cell state\n\nMouse wheel to zoom", 10, 10, 20,PURPLE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
