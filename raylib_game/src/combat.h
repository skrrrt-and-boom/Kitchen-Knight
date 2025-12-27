/**
 * Kitchen Knight - Combat System
 * ===============================
 * Weapon handling, hit detection, damage, and projectiles.
 */

#ifndef COMBAT_H
#define COMBAT_H

#include "game.h"
#include "raylib.h"
#include <stdbool.h>

// --- Constants ---
#define MAX_PROJECTILES 128

// --- Weapon State Machine ---
typedef enum { WEAPON_IDLE, WEAPON_FIRING, WEAPON_COOLDOWN } WeaponState;

typedef enum {
  WEAPON_SPATULA,     // Melee, fast, low damage
  WEAPON_FRYING_PAN,  // Melee, stun effect
  WEAPON_KETCHUP,     // Ranged, slows enemies
  WEAPON_EGG_LAUNCHER // Ranged, explosive
} WeaponType;

typedef struct {
  WeaponType type;
  WeaponState state;
  int damage;
  float range;
  float cooldownTime;
  float currentCooldown;
  bool isRanged;
  // Assets
  Texture2D sprite;
  Sound sfxAttack;
} Weapon;

typedef struct {
  Vector3 position;
  Vector3 velocity;
  float radius;
  int damage;
  bool active;
  float lifetime;
} Projectile;

// --- Functions ---
void InitCombat(void);
void UpdateCombat(GameState *game, float dt);
void DrawCombat3D(const GameState *game);
void DrawCombatUI(const GameState *game);
void UnloadCombat(void);

// Hit detection
bool CheckMeleeHit(GameState *game);
void SpawnProjectile(Vector3 pos, Vector3 dir, int damage);

// Screen shake
Vector2 GetScreenShakeOffset(void);

#endif // COMBAT_H
