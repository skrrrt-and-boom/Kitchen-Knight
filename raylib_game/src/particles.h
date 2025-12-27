/**
 * Kitchen Knight - Particle System
 * =================================
 * Visual effects for explosions, hits, etc.
 */

#ifndef PARTICLES_H
#define PARTICLES_H

#include "raylib.h"
#include <stdbool.h>

// --- Particle Struct ---
typedef struct {
  Vector3 position;
  Vector3 velocity;
  Color color;
  float lifetime;
  float maxLifetime;
  float size;
  bool active;
} Particle;

// Pool size
#define MAX_PARTICLES 256

// --- Functions ---
void InitParticleSystem(void);
void UpdateParticles(float dt);
void DrawParticles(void);

// Effects
void SpawnExplosion(Vector3 pos, Color color, int count);
void SpawnHitSparks(Vector3 pos, int count);
void SpawnBlood(Vector3 pos, int count);

#endif // PARTICLES_H
