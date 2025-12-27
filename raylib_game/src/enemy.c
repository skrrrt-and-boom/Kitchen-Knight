/**
 * Kitchen Knight - Enemy (Toaster) AI
 * ====================================
 * Simple chase AI that moves toward the player.
 */

#include "enemy.h"
#include <math.h>

// Clamp helper
static float Clampf(float value, float min, float max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

// ==========================================
// INITIALIZATION
// ==========================================

void InitEnemy(GameState *game) {
  // Start at (0, 2, 15) - matching Python version
  game->enemyPos = (Vector3){0.0f, ENEMY_HEIGHT / 2.0f, 15.0f};
}

// ==========================================
// GRID CONVERSION (matches Python train_ai.py)
// ==========================================

void WorldToGrid(Vector3 pos, float *gridX, float *gridZ) {
  // Convert world position to 0-9 grid
  // In Python: grid_x = np.clip((pos.x + 25) / GRID_SCALE, 0, 9)
  float halfArena = ARENA_SIZE / 2.0f; // 25.0

  *gridX =
      Clampf((pos.x + halfArena) / GRID_SCALE, 0.0f, (float)(GRID_SIZE - 1));
  *gridZ =
      Clampf((pos.z + halfArena) / GRID_SCALE, 0.0f, (float)(GRID_SIZE - 1));
}

// ==========================================
// AI DECISION
// ==========================================

EnemyAction GetAIAction(const GameState *game) {
  // Convert positions to grid coordinates
  float enemyGridX, enemyGridZ;
  float playerGridX, playerGridZ;

  WorldToGrid(game->enemyPos, &enemyGridX, &enemyGridZ);
  WorldToGrid(game->playerPos, &playerGridX, &playerGridZ);

  // Simple chase: move toward player
  float dx = playerGridX - enemyGridX;
  float dz = playerGridZ - enemyGridZ;

  // Choose the axis with the larger difference
  if (fabsf(dx) > fabsf(dz)) {
    // Move horizontally
    if (dx > 0.5f)
      return ACTION_RIGHT;
    if (dx < -0.5f)
      return ACTION_LEFT;
  } else {
    // Move vertically (Z axis)
    if (dz > 0.5f)
      return ACTION_UP;
    if (dz < -0.5f)
      return ACTION_DOWN;
  }

  return ACTION_STAY;
}

// ==========================================
// UPDATE
// ==========================================

void UpdateEnemy(GameState *game) {
  float dt = GetFrameTime();

  // Get AI action
  EnemyAction action = GetAIAction(game);

  // Convert action to movement vector
  Vector3 moveDir = {0.0f, 0.0f, 0.0f};

  switch (action) {
  case ACTION_UP:
    moveDir.z = 1.0f;
    break;
  case ACTION_DOWN:
    moveDir.z = -1.0f;
    break;
  case ACTION_LEFT:
    moveDir.x = -1.0f;
    break;
  case ACTION_RIGHT:
    moveDir.x = 1.0f;
    break;
  case ACTION_STAY:
    break;
  }

  // Apply movement
  game->enemyPos.x += moveDir.x * ENEMY_SPEED * dt;
  game->enemyPos.z += moveDir.z * ENEMY_SPEED * dt;

  // Keep within arena bounds
  float halfArena = (ARENA_SIZE / 2.0f) - 1.0f;
  game->enemyPos.x = Clampf(game->enemyPos.x, -halfArena, halfArena);
  game->enemyPos.z = Clampf(game->enemyPos.z, -halfArena, halfArena);
}

// ==========================================
// DRAW
// ==========================================

void DrawEnemy(const GameState *game) {
  // Bright orange color (matching Python: 255, 80, 0)
  Color enemyColor = (Color){255, 80, 0, 255};

  // Draw the enemy cube
  DrawCube(game->enemyPos, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_DEPTH, enemyColor);

  // Draw wireframe outline for better visibility
  DrawCubeWires(game->enemyPos, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_DEPTH, BLACK);
}
