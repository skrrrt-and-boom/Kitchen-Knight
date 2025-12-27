/**
 * Kitchen Knight - Audio System
 * =============================
 * Sound effect management (stubs for future implementation).
 */

#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"

// --- Sound Effect Types ---
typedef enum {
  SFX_ATTACK_MELEE,
  SFX_ATTACK_RANGED,
  SFX_HIT_ENEMY,
  SFX_PLAYER_HURT,
  SFX_ENEMY_DEATH,
  SFX_PICKUP,
  SFX_COUNT
} SFXType;

// --- Functions ---
void InitAudioSystem(void);
void PlaySFX(SFXType type);
void UnloadAudioSystem(void);

// Music
void PlayMusic(const char *filename);
void UpdateMusic(void);
void StopMusic(void);

#endif // AUDIO_H
