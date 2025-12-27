/**
 * Kitchen Knight - Player Controller
 * ===================================
 * First-person camera and WASD movement.
 */

#include "player.h"
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

void InitPlayer(GameState *game) {
  // Start position (center of arena, slightly elevated)
  game->playerPos = (Vector3){0.0f, PLAYER_HEIGHT, 0.0f};

  // Initial look direction (facing +Z)
  game->playerYaw = 0.0f;
  game->playerPitch = 0.0f;
  game->playerVelY = 0.0f;
  game->isGrounded = true;

  // Setup camera
  game->camera.position = game->playerPos;
  game->camera.target = (Vector3){0.0f, PLAYER_HEIGHT, 1.0f};
  game->camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  game->camera.fovy = 75.0f;
  game->camera.projection = CAMERA_PERSPECTIVE;
}

// ==========================================
// UPDATE
// ==========================================

void UpdatePlayer(GameState *game) {
  float dt = GetFrameTime();

  // --- MOUSE LOOK ---
  Vector2 mouseDelta = GetMouseDelta();

  game->playerYaw -= mouseDelta.x * MOUSE_SENSITIVITY;
  game->playerPitch -= mouseDelta.y * MOUSE_SENSITIVITY;

  // Clamp pitch to prevent camera flip
  game->playerPitch = Clampf(game->playerPitch, -1.4f, 1.4f);

  // --- KEYBOARD MOVEMENT ---
  Vector3 forward = {sinf(game->playerYaw), 0.0f, cosf(game->playerYaw)};

  Vector3 right = {cosf(game->playerYaw), 0.0f, -sinf(game->playerYaw)};

  Vector3 moveDir = {0.0f, 0.0f, 0.0f};

  if (IsKeyDown(KEY_W)) {
    moveDir.x += forward.x;
    moveDir.z += forward.z;
  }
  if (IsKeyDown(KEY_S)) {
    moveDir.x -= forward.x;
    moveDir.z -= forward.z;
  }
  if (IsKeyDown(KEY_A)) {
    moveDir.x += right.x;
    moveDir.z += right.z;
  }
  if (IsKeyDown(KEY_D)) {
    moveDir.x -= right.x;
    moveDir.z -= right.z;
  }

  // Normalize if moving diagonally
  float len = sqrtf(moveDir.x * moveDir.x + moveDir.z * moveDir.z);
  if (len > 0.0f) {
    moveDir.x /= len;
    moveDir.z /= len;
  }

  // Apply movement
  game->playerPos.x += moveDir.x * PLAYER_SPEED * dt;
  game->playerPos.z += moveDir.z * PLAYER_SPEED * dt;

  // --- GRAVITY ---
  if (!game->isGrounded) {
    game->playerVelY -= GRAVITY * dt * 60.0f;
    game->playerPos.y += game->playerVelY * dt;
  }

  // Ground check
  if (game->playerPos.y <= PLAYER_HEIGHT) {
    game->playerPos.y = PLAYER_HEIGHT;
    game->playerVelY = 0.0f;
    game->isGrounded = true;
  }

  // --- WALL COLLISIONS ---
  CheckPlayerCollisions(game);

  // --- UPDATE CAMERA ---
  game->camera.position = game->playerPos;

  // Calculate look target from yaw/pitch
  game->camera.target = (Vector3){
      game->playerPos.x + sinf(game->playerYaw) * cosf(game->playerPitch),
      game->playerPos.y + sinf(game->playerPitch),
      game->playerPos.z + cosf(game->playerYaw) * cosf(game->playerPitch)};
}

// ==========================================
// COLLISIONS
// ==========================================

void CheckPlayerCollisions(GameState *game) {
  float halfArena =
      (ARENA_SIZE / 2.0f) - 1.0f; // Leave margin for wall thickness

  // Clamp player position within arena
  game->playerPos.x = Clampf(game->playerPos.x, -halfArena, halfArena);
  game->playerPos.z = Clampf(game->playerPos.z, -halfArena, halfArena);
}
