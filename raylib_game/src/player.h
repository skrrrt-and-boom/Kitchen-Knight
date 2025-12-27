/**
 * Kitchen Knight - Player Controller
 * ===================================
 * First-person player movement and camera control.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

// Initialize player position and camera
void InitPlayer(GameState *game);

// Update player movement and camera from input
void UpdatePlayer(GameState *game);

// Check and resolve wall collisions
void CheckPlayerCollisions(GameState *game);

#endif // PLAYER_H
