/**
 * Kitchen Knight - Map Loader
 * ===========================
 * ASCII map parsing and level generation.
 */

#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include "raylib.h"
#include <stdbool.h>

// --- Map Cell Types ---
#define CELL_EMPTY '.'
#define CELL_WALL '#'
#define CELL_START 'S'
#define CELL_ENEMY_T 'T' // Toaster spawn
#define CELL_ENEMY_B 'B' // Blender spawn
#define CELL_ENEMY_M 'M' // Microwave spawn
#define CELL_ITEM_H 'H'  // Health pickup

// --- Level Map Struct ---
typedef struct {
  int width;
  int height;
  char *data;          // Raw map data
  Vector3 playerStart; // Player spawn position
  int enemyCount;      // Number of enemies placed
} LevelMap;

// --- Functions ---
bool LoadLevel(const char *filename, LevelMap *map);
void DrawLevel(const LevelMap *map);
void UnloadLevel(LevelMap *map);

// Convert grid position to world position
Vector3 GridToWorld(int gridX, int gridZ, int mapWidth, int mapHeight);

// Get cell at position
char GetCell(const LevelMap *map, int x, int z);

#endif // MAP_LOADER_H
