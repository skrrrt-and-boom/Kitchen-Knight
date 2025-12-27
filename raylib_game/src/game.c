/**
 * Kitchen Knight - Core Game Logic
 * =================================
 * Game initialization, update, and draw orchestration.
 */

#include "game.h"
#include "arena.h"
#include "enemy.h"
#include "player.h"

// ==========================================
// INITIALIZATION
// ==========================================

void InitGame(GameState *game) {
  // Initialize player and camera
  InitPlayer(game);

  // Initialize enemy
  InitEnemy(game);

  // Game state
  game->isRunning = true;
  game->isPaused = false;
  game->gameTime = 0.0f;
}

// ==========================================
// UPDATE
// ==========================================

void UpdateGame(GameState *game) {
  if (game->isPaused)
    return;

  float dt = GetFrameTime();
  game->gameTime += dt;

  // Update player movement and camera
  UpdatePlayer(game);

  // Update enemy AI
  UpdateEnemy(game);

  // Check for pause toggle
  if (IsKeyPressed(KEY_P)) {
    game->isPaused = !game->isPaused;
  }

  // Quit on ESC
  if (IsKeyPressed(KEY_ESCAPE)) {
    game->isRunning = false;
  }
}

// ==========================================
// DRAW
// ==========================================

void DrawGame(const GameState *game) {
  // Draw arena (floor and walls)
  DrawArena();

  // Draw enemy
  DrawEnemy(game);

  // Draw a reference grid on the floor (helpful for debugging)
  DrawGrid(10, GRID_SCALE);
}

// ==========================================
// CLEANUP
// ==========================================

void CleanupGame(GameState *game) {
  // Nothing to clean up for now
  (void)game;
}
