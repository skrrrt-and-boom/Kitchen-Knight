/**
 * Kitchen Knight - Particle System Implementation
 * ================================================
 * Particle pool, updates, and rendering.
 */

#include "particles.h"
#include "raymath.h"
#include <stdlib.h>

// --- Global Pool ---
static Particle particlePool[MAX_PARTICLES];

// ==========================================
// INITIALIZATION
// ==========================================

void InitParticleSystem(void) {
  for (int i = 0; i < MAX_PARTICLES; i++) {
    particlePool[i].active = false;
  }
}

// ==========================================
// HELPERS
// ==========================================

static int FindFreeSlot(void) {
  for (int i = 0; i < MAX_PARTICLES; i++) {
    if (!particlePool[i].active)
      return i;
  }
  return -1; // No free slot
}

static Vector3 RandomDirection(void) {
  float theta = ((float)GetRandomValue(0, 360)) * DEG2RAD;
  float phi = ((float)GetRandomValue(0, 180)) * DEG2RAD;

  return (Vector3){sinf(phi) * cosf(theta), cosf(phi), sinf(phi) * sinf(theta)};
}

// ==========================================
// SPAWNING
// ==========================================

void SpawnExplosion(Vector3 pos, Color color, int count) {
  for (int i = 0; i < count; i++) {
    int slot = FindFreeSlot();
    if (slot < 0)
      break;

    Vector3 dir = RandomDirection();
    float speed = 5.0f + (float)GetRandomValue(0, 100) / 10.0f;

    particlePool[slot] =
        (Particle){.position = pos,
                   .velocity = Vector3Scale(dir, speed),
                   .color = color,
                   .lifetime = 0.5f + (float)GetRandomValue(0, 50) / 100.0f,
                   .maxLifetime = 1.0f,
                   .size = 0.2f + (float)GetRandomValue(0, 30) / 100.0f,
                   .active = true};
    particlePool[slot].maxLifetime = particlePool[slot].lifetime;
  }
}

void SpawnHitSparks(Vector3 pos, int count) {
  for (int i = 0; i < count; i++) {
    int slot = FindFreeSlot();
    if (slot < 0)
      break;

    Vector3 dir = RandomDirection();
    float speed = 3.0f + (float)GetRandomValue(0, 50) / 10.0f;

    particlePool[slot] =
        (Particle){.position = pos,
                   .velocity = Vector3Scale(dir, speed),
                   .color = YELLOW,
                   .lifetime = 0.2f + (float)GetRandomValue(0, 20) / 100.0f,
                   .maxLifetime = 0.4f,
                   .size = 0.1f,
                   .active = true};
    particlePool[slot].maxLifetime = particlePool[slot].lifetime;
  }
}

void SpawnBlood(Vector3 pos, int count) {
  for (int i = 0; i < count; i++) {
    int slot = FindFreeSlot();
    if (slot < 0)
      break;

    Vector3 dir = RandomDirection();
    dir.y = fabsf(dir.y); // Bias upward
    float speed = 2.0f + (float)GetRandomValue(0, 40) / 10.0f;

    particlePool[slot] =
        (Particle){.position = pos,
                   .velocity = Vector3Scale(dir, speed),
                   .color = RED,
                   .lifetime = 0.3f + (float)GetRandomValue(0, 30) / 100.0f,
                   .maxLifetime = 0.6f,
                   .size = 0.15f,
                   .active = true};
    particlePool[slot].maxLifetime = particlePool[slot].lifetime;
  }
}

// ==========================================
// UPDATE
// ==========================================

void UpdateParticles(float dt) {
  for (int i = 0; i < MAX_PARTICLES; i++) {
    if (!particlePool[i].active)
      continue;

    Particle *p = &particlePool[i];

    // Physics
    p->position = Vector3Add(p->position, Vector3Scale(p->velocity, dt));
    p->velocity.y -= 15.0f * dt; // Gravity

    // Lifetime
    p->lifetime -= dt;
    if (p->lifetime <= 0) {
      p->active = false;
    }

    // Floor collision
    if (p->position.y < 0.1f) {
      p->position.y = 0.1f;
      p->velocity.y *= -0.3f; // Bounce
      p->velocity.x *= 0.8f;
      p->velocity.z *= 0.8f;
    }
  }
}

// ==========================================
// RENDERING
// ==========================================

void DrawParticles(void) {
  for (int i = 0; i < MAX_PARTICLES; i++) {
    if (!particlePool[i].active)
      continue;

    Particle *p = &particlePool[i];

    // Fade out
    float alpha = p->lifetime / p->maxLifetime;
    Color drawColor = p->color;
    drawColor.a = (unsigned char)(255 * alpha);

    // Shrink over time
    float size = p->size * alpha;

    DrawSphere(p->position, size, drawColor);
  }
}
