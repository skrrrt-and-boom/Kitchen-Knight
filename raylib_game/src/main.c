/**
 * Kitchen Knight - Main Entry Point
 * ==================================
 * A Doom-style 3D game built with Raylib.
 * Cross-platform: Mac and Windows.
 */

#include "arena.h"
#include "enemy.h"
#include "game.h"
#include "player.h"
#include "raylib.h"

int main(void) {
  // ==========================================
  // INITIALIZATION
  // ==========================================

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Kitchen Knight 3D");
  SetTargetFPS(TARGET_FPS);

  // Lock and hide cursor for FPS-style controls
  DisableCursor();

  // Initialize game state
  GameState game = {0};
  InitGame(&game);

  // ==========================================
  // MAIN GAME LOOP
  // ==========================================

  while (!WindowShouldClose() && game.isRunning) {
    // --- UPDATE ---
    UpdateGame(&game);

    // --- DRAW ---
    BeginDrawing();

    ClearBackground((Color){40, 40, 40, 255}); // Dark gray

    BeginMode3D(game.camera);
    DrawGame(&game);
    EndMode3D();

    // HUD
    DrawFPS(10, 10);
    DrawText("WASD - Move | Mouse - Look | ESC - Quit", 10, 40, 20, LIGHTGRAY);
    DrawText(TextFormat("Pos: (%.1f, %.1f, %.1f)", game.playerPos.x,
                        game.playerPos.y, game.playerPos.z),
             10, SCREEN_HEIGHT - 30, 20, GREEN);

    EndDrawing();
  }

  // ==========================================
  // CLEANUP
  // ==========================================

  CleanupGame(&game);
  CloseWindow();

  return 0;
}
