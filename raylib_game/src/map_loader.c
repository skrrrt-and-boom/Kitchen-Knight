/**
 * Kitchen Knight - Map Loader Implementation
 * ==========================================
 * Loads ASCII map files and generates game level.
 */

#include "map_loader.h"
#include "enemies/enemy_types.h"
#include "game.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// COORDINATE CONVERSION
// ==========================================

Vector3 GridToWorld(int gridX, int gridZ, int mapWidth, int mapHeight) {
  // Center the map around origin
  float cellSize = 4.0f;
  float halfWidth = (mapWidth * cellSize) / 2.0f;
  float halfHeight = (mapHeight * cellSize) / 2.0f;

  return (Vector3){(gridX * cellSize) - halfWidth + (cellSize / 2.0f), 0.0f,
                   (gridZ * cellSize) - halfHeight + (cellSize / 2.0f)};
}

char GetCell(const LevelMap *map, int x, int z) {
  if (x < 0 || x >= map->width || z < 0 || z >= map->height) {
    return CELL_WALL; // Out of bounds = wall
  }
  return map->data[z * map->width + x];
}

// ==========================================
// LOADING
// ==========================================

bool LoadLevel(const char *filename, LevelMap *map) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("[MapLoader] Failed to open: %s\n", filename);
    return false;
  }

  // First pass: determine dimensions
  int width = 0, height = 0;
  int currentWidth = 0;
  char c;

  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      if (currentWidth > width)
        width = currentWidth;
      currentWidth = 0;
      height++;
    } else if (c != '\r') {
      currentWidth++;
    }
  }
  if (currentWidth > 0)
    height++; // Last line without newline

  // Allocate data
  map->width = width;
  map->height = height;
  map->data = (char *)malloc(width * height);
  memset(map->data, CELL_EMPTY, width * height);
  map->enemyCount = 0;
  map->playerStart = (Vector3){0.0f, PLAYER_HEIGHT, 0.0f};

  // Second pass: read data
  rewind(file);
  int x = 0, z = 0;

  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      z++;
      x = 0;
    } else if (c != '\r') {
      if (x < width && z < height) {
        map->data[z * width + x] = c;

        // Handle special cells
        Vector3 worldPos = GridToWorld(x, z, width, height);

        switch (c) {
        case CELL_START:
          map->playerStart = worldPos;
          map->playerStart.y = PLAYER_HEIGHT;
          break;
        case CELL_ENEMY_T:
          worldPos.y = ENEMY_HEIGHT / 2.0f;
          SpawnEnemy(ENEMY_TOASTER, worldPos);
          map->enemyCount++;
          break;
        case CELL_ENEMY_B:
          worldPos.y = ENEMY_HEIGHT / 2.0f;
          SpawnEnemy(ENEMY_BLENDER, worldPos);
          map->enemyCount++;
          break;
        case CELL_ENEMY_M:
          worldPos.y = ENEMY_HEIGHT / 2.0f;
          SpawnEnemy(ENEMY_MICROWAVE, worldPos);
          map->enemyCount++;
          break;
        }
      }
      x++;
    }
  }

  fclose(file);
  printf(
      "[MapLoader] Loaded %s: %dx%d, %d enemies, start at (%.1f, %.1f, %.1f)\n",
      filename, width, height, map->enemyCount, map->playerStart.x,
      map->playerStart.y, map->playerStart.z);

  return true;
}

// ==========================================
// RENDERING
// ==========================================

void DrawLevel(const LevelMap *map) {
  float cellSize = 4.0f;
  float wallHeight = WALL_HEIGHT;

  // Floor
  float floorSize = fmaxf((float)map->width, (float)map->height) * cellSize;
  DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){floorSize, floorSize},
            (Color){60, 80, 60, 255});

  // Walls
  for (int z = 0; z < map->height; z++) {
    for (int x = 0; x < map->width; x++) {
      if (GetCell(map, x, z) == CELL_WALL) {
        Vector3 pos = GridToWorld(x, z, map->width, map->height);
        pos.y = wallHeight / 2.0f;

        DrawCube(pos, cellSize, wallHeight, cellSize, BROWN);
        DrawCubeWires(pos, cellSize, wallHeight, cellSize, DARKBROWN);
      }
    }
  }
}

// ==========================================
// CLEANUP
// ==========================================

void UnloadLevel(LevelMap *map) {
  if (map->data) {
    free(map->data);
    map->data = NULL;
  }
  map->width = 0;
  map->height = 0;
}
