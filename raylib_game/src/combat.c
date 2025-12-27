/**
 * Kitchen Knight - Combat System Implementation
 * ==============================================
 * Handles weapons, hit detection, projectiles, and screen effects.
 */

#include "combat.h"
#include "audio.h"
#include "particles.h"
#include "raymath.h"
#include <stdio.h>

// --- Global State ---
static Weapon currentWeapon;
static Projectile projectilePool[MAX_PROJECTILES];

// Screen shake state
static float shakeTimer = 0.0f;
static float shakeIntensity = 0.0f;

// Textures for rendering
static Texture2D toasterTexture;
static Texture2D spatulaTexture;
static bool texturesLoaded = false;

// ==========================================
// INITIALIZATION
// ==========================================

void InitCombat(void) {
  // Default weapon: Spatula
  currentWeapon = (Weapon){.type = WEAPON_SPATULA,
                           .state = WEAPON_IDLE,
                           .damage = 10,
                           .range = 4.0f,
                           .cooldownTime = 0.4f,
                           .currentCooldown = 0.0f,
                           .isRanged = false};

  // Clear projectile pool
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    projectilePool[i].active = false;
  }

  // Load textures
  toasterTexture = LoadTexture("assets/toster.png");
  spatulaTexture = LoadTexture("assets/spatula_hand.png");

  if (toasterTexture.id > 0 && spatulaTexture.id > 0) {
    texturesLoaded = true;
    printf("[Combat] Loaded textures: toaster and spatula\n");
  } else if (toasterTexture.id > 0) {
    texturesLoaded =
        true; // Still allow toaster if only that loaded for some reason
    printf("[Combat] Loaded toaster texture but spatula failed\n");
  }
}

// ==========================================
// UPDATE
// ==========================================

void UpdateCombat(GameState *game, float dt) {
  // 0. Weapon switching (1-4 keys)
  if (IsKeyPressed(KEY_ONE)) {
    currentWeapon.type = WEAPON_SPATULA;
    currentWeapon.damage = 10;
    currentWeapon.range = 4.0f;
    currentWeapon.cooldownTime = 0.4f;
    currentWeapon.isRanged = false;
  } else if (IsKeyPressed(KEY_TWO)) {
    currentWeapon.type = WEAPON_FRYING_PAN;
    currentWeapon.damage = 25;
    currentWeapon.range = 3.0f;
    currentWeapon.cooldownTime = 0.8f;
    currentWeapon.isRanged = false;
  } else if (IsKeyPressed(KEY_THREE)) {
    currentWeapon.type = WEAPON_KETCHUP;
    currentWeapon.damage = 8;
    currentWeapon.range = 15.0f;
    currentWeapon.cooldownTime = 0.2f;
    currentWeapon.isRanged = true;
  } else if (IsKeyPressed(KEY_FOUR)) {
    currentWeapon.type = WEAPON_EGG_LAUNCHER;
    currentWeapon.damage = 40;
    currentWeapon.range = 20.0f;
    currentWeapon.cooldownTime = 1.2f;
    currentWeapon.isRanged = true;
  }

  // 1. Weapon cooldown
  if (currentWeapon.currentCooldown > 0) {
    currentWeapon.currentCooldown -= dt;
    if (currentWeapon.currentCooldown <= 0) {
      currentWeapon.state = WEAPON_IDLE;
    }
  }

  // 2. Attack input
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
      currentWeapon.state == WEAPON_IDLE) {

    // Play attack sound
    PlaySFX(currentWeapon.isRanged ? SFX_ATTACK_RANGED : SFX_ATTACK_MELEE);

    currentWeapon.state = WEAPON_FIRING;
    currentWeapon.currentCooldown = currentWeapon.cooldownTime;

    // Trigger screen shake
    shakeTimer = 0.1f;
    shakeIntensity = 3.0f;

    if (!currentWeapon.isRanged) {
      // Melee: Raycast hit detection
      if (CheckMeleeHit(game)) {
        printf("[Combat] HIT! Enemy HP: %d -> %d\n", game->enemyHP,
               game->enemyHP - currentWeapon.damage);

        game->enemyHP -= currentWeapon.damage;

        // Death check
        if (game->enemyHP <= 0) {
          game->enemyActive = false;
          printf("[Combat] ENEMY DESTROYED!\n");
        }
      }
    } else {
      // Ranged: Spawn projectile
      Vector3 dir = Vector3Normalize(
          Vector3Subtract(game->camera.target, game->camera.position));
      SpawnProjectile(game->camera.position, dir, currentWeapon.damage);
    }
  }

  // 3. Update projectiles
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (!projectilePool[i].active)
      continue;

    projectilePool[i].position =
        Vector3Add(projectilePool[i].position,
                   Vector3Scale(projectilePool[i].velocity, dt));

    projectilePool[i].lifetime -= dt;
    if (projectilePool[i].lifetime <= 0) {
      projectilePool[i].active = false;
    }

    // Check collision with enemy
    if (game->enemyActive) {
      float dist = Vector3Distance(projectilePool[i].position, game->enemyPos);
      if (dist < ENEMY_WIDTH) {
        game->enemyHP -= projectilePool[i].damage;
        projectilePool[i].active = false;

        if (game->enemyHP <= 0) {
          game->enemyActive = false;
          printf("[Combat] ENEMY DESTROYED by projectile!\n");
        }
      }
    }
  }

  // 4. Screen shake decay
  if (shakeTimer > 0)
    shakeTimer -= dt;
}

// ==========================================
// HIT DETECTION
// ==========================================

bool CheckMeleeHit(GameState *game) {
  if (!game->enemyActive)
    return false;

  Ray attackRay = {.position = game->camera.position,
                   .direction = Vector3Normalize(Vector3Subtract(
                       game->camera.target, game->camera.position))};

  // Check against enemy with sphere collision
  RayCollision col =
      GetRayCollisionSphere(attackRay, game->enemyPos, ENEMY_WIDTH);

  if (col.hit && col.distance <= currentWeapon.range) {
    return true;
  }
  return false;
}

void SpawnProjectile(Vector3 pos, Vector3 dir, int damage) {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (!projectilePool[i].active) {
      projectilePool[i] = (Projectile){.position = pos,
                                       .velocity = Vector3Scale(dir, 30.0f),
                                       .radius = 0.3f,
                                       .damage = damage,
                                       .active = true,
                                       .lifetime = 3.0f};
      break;
    }
  }
}

// ==========================================
// SCREEN SHAKE
// ==========================================

Vector2 GetScreenShakeOffset(void) {
  if (shakeTimer <= 0)
    return (Vector2){0, 0};
  return (Vector2){(float)(GetRandomValue(-100, 100)) / 100.0f * shakeIntensity,
                   (float)(GetRandomValue(-100, 100)) / 100.0f *
                       shakeIntensity};
}

// ==========================================
// RENDERING
// ==========================================

void DrawCombat3D(const GameState *game) {
  // Draw enemy
  if (game->enemyActive) {
    if (texturesLoaded) {
      // Enable alpha blending for transparency
      BeginBlendMode(BLEND_ALPHA);
      DrawBillboard(game->camera, toasterTexture, game->enemyPos, 4.0f, WHITE);
      EndBlendMode();
    } else {
      // Fallback: Draw cube if texture not loaded
      DrawCube(game->enemyPos, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_DEPTH, ORANGE);
      DrawCubeWires(game->enemyPos, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_DEPTH,
                    BLACK);
    }
  }

  // Draw projectiles
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (projectilePool[i].active) {
      DrawSphere(projectilePool[i].position, 0.3f, RED);
    }
  }
}

void DrawCombatUI(const GameState *game) {
  int cx = GetScreenWidth() / 2;
  int cy = GetScreenHeight() / 2;

  // Crosshair (color based on weapon)
  Color crosshairColor = GREEN;
  if (currentWeapon.isRanged)
    crosshairColor = SKYBLUE;
  DrawCircle(cx, cy, 4, crosshairColor);
  DrawCircleLines(cx, cy, 8, crosshairColor);

  // Cooldown bar below crosshair
  if (currentWeapon.currentCooldown > 0) {
    float ratio = currentWeapon.currentCooldown / currentWeapon.cooldownTime;
    DrawRectangle(cx - 25, cy + 40, 50, 6, DARKGRAY);
    DrawRectangle(cx - 25, cy + 40, (int)(50 * (1.0f - ratio)), 6, YELLOW);
  }

  // Weapon indicator (bottom center)
  const char *weaponNames[] = {"SPATULA", "FRYING PAN", "KETCHUP",
                               "EGG LAUNCHER"};
  const char *name = weaponNames[currentWeapon.type];
  int textWidth = MeasureText(name, 20);
  DrawText(name, cx - textWidth / 2, GetScreenHeight() - 100, 20, WHITE);
  DrawText("[1] [2] [3] [4]", cx - 60, GetScreenHeight() - 75, 16, GRAY);

  // Draw weapon sprite if it's the spatula
  if (currentWeapon.type == WEAPON_SPATULA && texturesLoaded &&
      spatulaTexture.id > 0) {
    float scale = 0.5f;
    int posX = GetScreenWidth() - (int)(spatulaTexture.width * scale) - 20;
    int posY = GetScreenHeight() - (int)(spatulaTexture.height * scale);

    // Subtle bobbing animation
    float bobOffset = sinf(GetTime() * 5.0f) * 10.0f;

    DrawTextureEx(spatulaTexture,
                  (Vector2){(float)posX, (float)posY + bobOffset}, 0.0f, scale,
                  WHITE);
  }

  // Player health bar (top left)
  DrawRectangle(20, 20, 200, 20, DARKGRAY);
  float hpRatio = (float)game->playerHP / (float)game->playerMaxHP;
  DrawRectangle(20, 20, (int)(200 * hpRatio), 20, RED);
  DrawRectangleLines(20, 20, 200, 20, WHITE);
  DrawText(TextFormat("HP: %d/%d", game->playerHP, game->playerMaxHP), 25, 23,
           16, WHITE);

  // Enemy info (if alive)
  if (game->enemyActive) {
    DrawText(TextFormat("Enemy HP: %d", game->enemyHP), 20, 50, 16, ORANGE);
  } else {
    DrawText("ENEMY DEFEATED!", 20, 50, 20, GREEN);
  }
}

// ==========================================
// CLEANUP
// ==========================================

void UnloadCombat(void) {
  if (texturesLoaded) {
    UnloadTexture(toasterTexture);
    UnloadTexture(spatulaTexture);
  }
}
