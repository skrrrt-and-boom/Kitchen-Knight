/**
 * Kitchen Knight - Arena (Floor & Walls)
 * =======================================
 * 3D environment rendering.
 */

#ifndef ARENA_H
#define ARENA_H

#include "game.h"

// Initialize arena assets (textures, etc.)
void InitArena(void);

// Unload arena assets
void UnloadArena(void);

// Draw the complete arena (floor + walls)
void DrawArena(void);

// Draw just the floor
void DrawFloor(void);

// Draw all four walls
void DrawWalls(void);

#endif // ARENA_H
