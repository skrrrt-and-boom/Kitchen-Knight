/**
 * Kitchen Knight - Enemy Types
 * ============================
 * Shared enemy type definitions and pool system.
 */

#ifndef ENEMY_TYPES_H
#define ENEMY_TYPES_H

#include "../game.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdint.h>

// --- Enemy Type Enum ---
typedef enum {
  ENEMY_TOASTER,  // Toast projectile attack
  ENEMY_BLENDER,  // Spin charge attack
  ENEMY_MICROWAVE // Stationary beam attack
} EnemyType;

// --- AI State Machine ---
typedef enum {
  AI_IDLE,   // Not engaged
  AI_CHASE,  // Moving toward player
  AI_ATTACK, // Attacking
  AI_HURT,   // Taking damage (brief stun)
  AI_DEAD    // Dead, awaiting respawn/cleanup
} AIState;

// --- Enemy State Flags (Bitmask) ---
#define ENEMY_FLAG_ACTIVE 0x01
#define ENEMY_FLAG_HURT 0x02
#define ENEMY_FLAG_BURNING 0x04
#define ENEMY_FLAG_STUNNED 0x08

// --- Enemy Struct ---
typedef struct {
  EnemyType type;
  AIState state;
  Vector3 position;
  float radius;
  int hp;
  int maxHP;
  uint8_t flags; // ENEMY_FLAG_ACTIVE | ENEMY_FLAG_HURT etc.
  float stateTimer;
  Color color;
  // Type-specific
  float attackRange;
  float attackCooldown;
  float currentCooldown;
  // Movement speed (type-dependent)
  float speed;
} Enemy;

// --- Pool ---
#define MAX_ENEMIES 64
extern Enemy enemyPool[MAX_ENEMIES];
extern int activeEnemyCount;

// --- Functions ---
void InitEnemySystem(void);
int SpawnEnemy(EnemyType type, Vector3 pos);
void UpdateEnemies(GameState *game, float dt);
void DrawEnemies(const GameState *game);
void DamageEnemy(int index, int damage);
void KillEnemy(int index);

// Get enemy default stats by type
void GetEnemyDefaults(EnemyType type, int *hp, float *speed, float *attackRange,
                      Color *color);

#endif // ENEMY_TYPES_H
