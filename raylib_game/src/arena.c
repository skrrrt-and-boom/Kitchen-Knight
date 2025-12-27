/**
 * Kitchen Knight - Arena (Floor & Walls)
 * =======================================
 * 3D environment matching the Python version.
 */

#include "arena.h"

// Colors (matching Python version)
static const Color FLOOR_COLOR = {80, 140, 80, 255}; // Green
static const Color WALL_COLOR = {160, 100, 60, 255}; // Brown

// ==========================================
// FLOOR
// ==========================================

void DrawFloor(void) {
  // Draw a large plane at y=0
  // Size matches Python: 50x50 units
  DrawPlane((Vector3){0.0f, 0.0f, 0.0f},       // Center position
            (Vector2){ARENA_SIZE, ARENA_SIZE}, // Size
            FLOOR_COLOR);
}

// ==========================================
// WALLS
// ==========================================

void DrawWalls(void) {
  float halfArena = ARENA_SIZE / 2.0f; // 25.0
  float wallY = WALL_HEIGHT / 2.0f;    // Center of wall height

  // North wall (+Z)
  DrawCube((Vector3){0.0f, wallY, halfArena}, ARENA_SIZE, WALL_HEIGHT,
           WALL_THICKNESS, WALL_COLOR);
  DrawCubeWires((Vector3){0.0f, wallY, halfArena}, ARENA_SIZE, WALL_HEIGHT,
                WALL_THICKNESS, DARKGRAY);

  // South wall (-Z)
  DrawCube((Vector3){0.0f, wallY, -halfArena}, ARENA_SIZE, WALL_HEIGHT,
           WALL_THICKNESS, WALL_COLOR);
  DrawCubeWires((Vector3){0.0f, wallY, -halfArena}, ARENA_SIZE, WALL_HEIGHT,
                WALL_THICKNESS, DARKGRAY);

  // East wall (+X)
  DrawCube((Vector3){halfArena, wallY, 0.0f}, WALL_THICKNESS, WALL_HEIGHT,
           ARENA_SIZE, WALL_COLOR);
  DrawCubeWires((Vector3){halfArena, wallY, 0.0f}, WALL_THICKNESS, WALL_HEIGHT,
                ARENA_SIZE, DARKGRAY);

  // West wall (-X)
  DrawCube((Vector3){-halfArena, wallY, 0.0f}, WALL_THICKNESS, WALL_HEIGHT,
           ARENA_SIZE, WALL_COLOR);
  DrawCubeWires((Vector3){-halfArena, wallY, 0.0f}, WALL_THICKNESS, WALL_HEIGHT,
                ARENA_SIZE, DARKGRAY);
}

// ==========================================
// FULL ARENA
// ==========================================

void DrawArena(void) {
  DrawFloor();
  DrawWalls();
}
