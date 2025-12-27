/**
 * Kitchen Knight - Game State and Core Types
 * ==========================================
 * Central game state structure and constants.
 */

#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <stdbool.h>

// ===========================================
// CONSTANTS (matching Python version)
// ===========================================

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TARGET_FPS 60

// Arena dimensions
#define ARENA_SIZE 50.0f
#define WALL_HEIGHT 5.0f
#define WALL_THICKNESS 1.0f

// Player settings
#define PLAYER_SPEED 10.0f
#define MOUSE_SENSITIVITY 0.003f
#define PLAYER_HEIGHT 1.8f
#define GRAVITY 0.5f
#define PLAYER_MAX_HP 100

// Enemy settings
#define ENEMY_WIDTH 3.0f
#define ENEMY_HEIGHT 4.0f
#define ENEMY_DEPTH 3.0f
#define ENEMY_SPEED 3.0f

// AI Grid (matching Python training)
#define GRID_SIZE 10
#define GRID_SCALE 5.0f

// ===========================================
// GAME STATE
// ===========================================

typedef struct {
  // Camera / Player
  Camera3D camera;
  Vector3 playerPos;
  float playerYaw;   // Horizontal rotation (radians)
  float playerPitch; // Vertical rotation (radians)
  float playerVelY;  // Vertical velocity (for gravity)
  bool isGrounded;

  // Enemy
  Vector3 enemyPos;
  int enemyHP;
  bool enemyActive;

  // Player combat
  int playerHP;
  int playerMaxHP;
  float damageFlashTimer;

  // Game state
  bool isRunning;
  bool isPaused;
  float gameTime;

} GameState;

// ===========================================
// FUNCTION DECLARATIONS
// ===========================================

// Game lifecycle
void InitGame(GameState *game);
void UpdateGame(GameState *game);
void DrawGame(const GameState *game);
void DrawGameUI(const GameState *game);
void CleanupGame(GameState *game);

#endif // GAME_H
