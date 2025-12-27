/**
 * Kitchen Knight - Enemy (Toaster) AI
 * ====================================
 * AI-controlled enemy that chases the player.
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"

// AI Actions (matching Python training)
typedef enum {
  ACTION_STAY = 0,
  ACTION_UP = 1,    // +Z
  ACTION_DOWN = 2,  // -Z
  ACTION_LEFT = 3,  // -X
  ACTION_RIGHT = 4, // +X
} EnemyAction;

// Initialize enemy position
void InitEnemy(GameState *game);

// Update enemy AI and movement
void UpdateEnemy(GameState *game);

// Draw the enemy cube
void DrawEnemy(const GameState *game);

// Convert world position to grid coordinates (matching Python)
void WorldToGrid(Vector3 pos, float *gridX, float *gridZ);

// Get AI action (simple chase for now)
EnemyAction GetAIAction(const GameState *game);

#endif // ENEMY_H
