/**
 * Kitchen Knight - Core Game Logic
 * =================================
 * Game initialization, update, and draw orchestration.
 */

#include "game.h"
#include "arena.h"
#include "audio.h"
#include "combat.h"
#include "enemies/enemy_types.h"
#include "enemy.h"
#include "particles.h"
#include "player.h"

// ==========================================
// INITIALIZATION
// ==========================================

void InitGame(GameState *game) {
  // Initialize audio system
  InitAudioSystem();

  // Initialize enemy pool system
  InitEnemySystem();

  // Initialize particle system
  InitParticleSystem();

  // Initialize player and camera
  InitPlayer(game);

  // Initialize legacy enemy (for backward compatibility)
  InitEnemy(game);

  // Spawn some enemies at set positions using the new pool system
  SpawnEnemy(ENEMY_TOASTER, (Vector3){10.0f, ENEMY_HEIGHT / 2.0f, 10.0f});
  SpawnEnemy(ENEMY_TOASTER, (Vector3){-15.0f, ENEMY_HEIGHT / 2.0f, 5.0f});

  // Player combat stats
  game->playerHP = PLAYER_MAX_HP;
  game->playerMaxHP = PLAYER_MAX_HP;
  game->damageFlashTimer = 0.0f;

  // Legacy enemy combat stats (kept for backward compat)
  game->enemyHP = 30;
  game->enemyActive = true;

  // Game state
  game->isRunning = true;
  game->isPaused = false;
  game->gameTime = 0.0f;

  // Initialize combat system
  InitCombat();
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

  // Update legacy enemy AI (only if alive)
  if (game->enemyActive) {
    UpdateEnemy(game);
  }

  // Update all enemies from pool
  UpdateEnemies(game, dt);

  // Update combat system
  UpdateCombat(game, dt);

  // Update particles
  UpdateParticles(dt);

  // Update music if playing
  UpdateMusic();

  // Damage flash decay
  if (game->damageFlashTimer > 0) {
    game->damageFlashTimer -= dt;
  }

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

  // Draw legacy enemy (billboard from combat system)
  if (game->enemyActive) {
    DrawCombat3D(game);
  }

  // Draw all enemies from pool
  DrawEnemies(game);

  // Draw particles
  DrawParticles();

  // Draw a reference grid on the floor (helpful for debugging)
  DrawGrid(10, GRID_SCALE);
}

// Draw UI elements (called from main.c after EndMode3D)
void DrawGameUI(const GameState *game) {
  // Combat HUD (crosshair, health bars)
  DrawCombatUI(game);

  // Damage flash overlay
  if (game->damageFlashTimer > 0) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                  ColorAlpha(RED, game->damageFlashTimer * 0.5f));
  }

  // Pause overlay
  if (game->isPaused) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                  ColorAlpha(BLACK, 0.5f));
    DrawText("PAUSED", GetScreenWidth() / 2 - 60, GetScreenHeight() / 2 - 20,
             40, WHITE);
  }
}

// ==========================================
// CLEANUP
// ==========================================

void CleanupGame(GameState *game) {
  UnloadCombat();
  UnloadAudioSystem();
  (void)game;
}
