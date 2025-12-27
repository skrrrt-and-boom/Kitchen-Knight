/**
 * Kitchen Knight - Arena (Floor & Walls)
 * =======================================
 * 3D environment matching the Python version.
 */

#include "arena.h"
#include <stdio.h>

// Colors (matching Python version)
static const Color FLOOR_COLOR = {80, 140, 80, 255}; // Green
static const Color WALL_COLOR = {160, 100, 60, 255}; // Brown

static Texture2D floorTexture;
static Model floorModel;
static bool arenaAssetsLoaded = false;

// ==========================================
// INITIALIZATION
// ==========================================

void InitArena(void) {
  floorTexture = LoadTexture("assets/tile_floor.png");
  if (floorTexture.id > 0) {
    // Create a model for the floor
    Mesh mesh = GenMeshCube(ARENA_SIZE, 0.1f, ARENA_SIZE);
    floorModel = LoadModelFromMesh(mesh);
    // Assign texture to model
    floorModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = floorTexture;

    arenaAssetsLoaded = true;
    printf("[Arena] Loaded floor texture and created model\n");
  }
}

void UnloadArena(void) {
  if (arenaAssetsLoaded) {
    UnloadModel(floorModel);
    UnloadTexture(floorTexture);
  }
}

// ==========================================
// FLOOR
// ==========================================

void DrawFloor(void) {
  if (arenaAssetsLoaded) {
    // Draw the textured floor model
    DrawModel(floorModel, (Vector3){0.0f, -0.05f, 0.0f}, 1.0f, WHITE);
  } else {
    // Fallback to plane if texture not loaded
    DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){ARENA_SIZE, ARENA_SIZE},
              FLOOR_COLOR);
  }
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
