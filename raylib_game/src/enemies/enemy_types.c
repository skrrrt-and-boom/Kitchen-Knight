/**
 * Kitchen Knight - Enemy Types Implementation
 * ============================================
 * Enemy pool system with multiple enemy support.
 */

#include "enemy_types.h"
#include "../game.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>

// --- Global Pool ---
Enemy enemyPool[MAX_ENEMIES];
int activeEnemyCount = 0;

// --- Textures ---
static Texture2D toasterTexture;
static bool texturesLoaded = false;

// --- Helpers ---
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

void InitEnemySystem(void) {
  // Clear all enemies
  for (int i = 0; i < MAX_ENEMIES; i++) {
    enemyPool[i].flags = 0; // Inactive
    enemyPool[i].state = AI_DEAD;
  }
  activeEnemyCount = 0;

  // Load textures
  toasterTexture = LoadTexture("assets/toster.png");
  if (toasterTexture.id > 0) {
    texturesLoaded = true;
    printf("[EnemySystem] Loaded toaster texture: %dx%d\n",
           toasterTexture.width, toasterTexture.height);
  }
}

// ==========================================
// ENEMY DEFAULTS BY TYPE
// ==========================================

void GetEnemyDefaults(EnemyType type, int *hp, float *speed, float *attackRange,
                      Color *color) {
  switch (type) {
  case ENEMY_TOASTER:
    *hp = 30;
    *speed = 3.0f;
    *attackRange = 8.0f;
    *color = (Color){255, 165, 0, 255}; // Orange
    break;
  case ENEMY_BLENDER:
    *hp = 50;
    *speed = 5.0f;
    *attackRange = 3.0f;
    *color = (Color){128, 128, 128, 255}; // Gray
    break;
  case ENEMY_MICROWAVE:
    *hp = 80;
    *speed = 1.5f;
    *attackRange = 15.0f;
    *color = (Color){200, 200, 200, 255}; // Light gray
    break;
  default:
    *hp = 30;
    *speed = 3.0f;
    *attackRange = 5.0f;
    *color = ORANGE;
    break;
  }
}

// ==========================================
// SPAWN
// ==========================================

int SpawnEnemy(EnemyType type, Vector3 pos) {
  // Find inactive slot
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!(enemyPool[i].flags & ENEMY_FLAG_ACTIVE)) {
      // Get defaults for this type
      int hp;
      float speed, attackRange;
      Color color;
      GetEnemyDefaults(type, &hp, &speed, &attackRange, &color);

      enemyPool[i] = (Enemy){.type = type,
                             .state = AI_IDLE,
                             .position = pos,
                             .radius = ENEMY_WIDTH / 2.0f,
                             .hp = hp,
                             .maxHP = hp,
                             .flags = ENEMY_FLAG_ACTIVE,
                             .stateTimer = 0.0f,
                             .color = color,
                             .attackRange = attackRange,
                             .attackCooldown = 1.0f,
                             .currentCooldown = 0.0f,
                             .speed = speed};

      activeEnemyCount++;
      printf("[EnemySystem] Spawned enemy type %d at (%.1f, %.1f, %.1f) - slot "
             "%d\n",
             type, pos.x, pos.y, pos.z, i);
      return i;
    }
  }

  printf("[EnemySystem] WARNING: No free slots for enemy spawn!\n");
  return -1;
}

// ==========================================
// UPDATE
// ==========================================

static void UpdateSingleEnemy(Enemy *enemy, const GameState *game, float dt) {
  if (!(enemy->flags & ENEMY_FLAG_ACTIVE))
    return;
  if (enemy->state == AI_DEAD)
    return;

  // Handle hurt state (brief stun)
  if (enemy->flags & ENEMY_FLAG_HURT) {
    enemy->stateTimer -= dt;
    if (enemy->stateTimer <= 0) {
      enemy->flags &= ~ENEMY_FLAG_HURT;
      enemy->state = AI_CHASE;
    }
    return; // Skip movement while hurt
  }

  // Calculate distance to player
  float dist = Vector3Distance(enemy->position, game->playerPos);

  // State transitions
  if (enemy->state == AI_IDLE) {
    if (dist < 20.0f) { // Detection range
      enemy->state = AI_CHASE;
    }
  }

  if (enemy->state == AI_CHASE) {
    if (dist <= enemy->attackRange) {
      enemy->state = AI_ATTACK;
      enemy->stateTimer = 0.5f; // Attack windup
    }
  }

  // Movement (chase state)
  if (enemy->state == AI_CHASE) {
    Vector3 dir = Vector3Subtract(game->playerPos, enemy->position);
    dir.y = 0; // Keep on ground
    dir = Vector3Normalize(dir);

    enemy->position =
        Vector3Add(enemy->position, Vector3Scale(dir, enemy->speed * dt));

    // Keep within arena
    float halfArena = (ARENA_SIZE / 2.0f) - 1.0f;
    enemy->position.x = Clampf(enemy->position.x, -halfArena, halfArena);
    enemy->position.z = Clampf(enemy->position.z, -halfArena, halfArena);
  }

  // Attack state
  if (enemy->state == AI_ATTACK) {
    enemy->stateTimer -= dt;
    if (enemy->stateTimer <= 0) {
      // TODO: Execute attack (projectile for toaster, charge for blender)
      enemy->state = AI_CHASE;
      enemy->currentCooldown = enemy->attackCooldown;
    }
  }

  // Attack cooldown
  if (enemy->currentCooldown > 0) {
    enemy->currentCooldown -= dt;
  }
}

void UpdateEnemies(GameState *game, float dt) {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    UpdateSingleEnemy(&enemyPool[i], game, dt);
  }
}

// ==========================================
// DAMAGE
// ==========================================

void DamageEnemy(int index, int damage) {
  if (index < 0 || index >= MAX_ENEMIES)
    return;
  if (!(enemyPool[index].flags & ENEMY_FLAG_ACTIVE))
    return;

  Enemy *enemy = &enemyPool[index];
  enemy->hp -= damage;
  enemy->flags |= ENEMY_FLAG_HURT;
  enemy->state = AI_HURT;
  enemy->stateTimer = 0.2f; // Hurt stun duration

  printf("[EnemySystem] Enemy %d took %d damage, HP: %d/%d\n", index, damage,
         enemy->hp, enemy->maxHP);

  if (enemy->hp <= 0) {
    KillEnemy(index);
  }
}

void KillEnemy(int index) {
  if (index < 0 || index >= MAX_ENEMIES)
    return;

  enemyPool[index].flags &= ~ENEMY_FLAG_ACTIVE;
  enemyPool[index].state = AI_DEAD;
  activeEnemyCount--;

  printf("[EnemySystem] Enemy %d destroyed! Active: %d\n", index,
         activeEnemyCount);

  // TODO: Spawn particles at enemy position
}

// ==========================================
// RENDERING
// ==========================================

void DrawEnemies(const GameState *game) {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!(enemyPool[i].flags & ENEMY_FLAG_ACTIVE))
      continue;
    if (enemyPool[i].state == AI_DEAD)
      continue;

    Enemy *enemy = &enemyPool[i];

    // Flash red when hurt
    Color drawColor = enemy->color;
    if (enemy->flags & ENEMY_FLAG_HURT) {
      drawColor = RED;
    }

    if (texturesLoaded && enemy->type == ENEMY_TOASTER) {
      BeginBlendMode(BLEND_ALPHA);
      DrawBillboard(game->camera, toasterTexture, enemy->position, 4.0f,
                    drawColor);
      EndBlendMode();
    } else {
      // Fallback cube rendering
      DrawCube(enemy->position, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_DEPTH,
               drawColor);
      DrawCubeWires(enemy->position, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_DEPTH,
                    BLACK);
    }
  }
}
