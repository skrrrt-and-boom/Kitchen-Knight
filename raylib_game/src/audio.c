/**
 * Kitchen Knight - Audio System Implementation
 * =============================================
 * Stub implementation ready for sound files.
 */

#include "audio.h"
#include <stdio.h>

// --- Audio State ---
static bool audioInitialized = false;
static Music currentMusic = {0};
static bool musicPlaying = false;

// TODO: Add sound effects when assets are available
// static Sound sounds[SFX_COUNT];

// ==========================================
// INITIALIZATION
// ==========================================

void InitAudioSystem(void) {
  InitAudioDevice();
  audioInitialized = true;
  printf("[Audio] Audio device initialized\n");

  // TODO: Load sound effects
  // sounds[SFX_ATTACK_MELEE] = LoadSound("assets/sfx/whoosh.wav");
  // sounds[SFX_HIT_ENEMY] = LoadSound("assets/sfx/hit.wav");
  // sounds[SFX_ENEMY_DEATH] = LoadSound("assets/sfx/explosion.wav");
  // sounds[SFX_PLAYER_HURT] = LoadSound("assets/sfx/oof.wav");
}

// ==========================================
// SOUND EFFECTS
// ==========================================

void PlaySFX(SFXType type) {
  if (!audioInitialized)
    return;

  // TODO: Play actual sounds when available
  // if (type < SFX_COUNT && sounds[type].frameCount > 0) {
  //     PlaySound(sounds[type]);
  // }

  // Debug output (remove when sounds are implemented)
  const char *names[] = {"ATTACK_MELEE", "ATTACK_RANGED", "HIT_ENEMY",
                         "PLAYER_HURT",  "ENEMY_DEATH",   "PICKUP"};
  if (type < SFX_COUNT) {
    printf("[Audio] Play SFX: %s (stub)\n", names[type]);
  }
}

// ==========================================
// MUSIC
// ==========================================

void PlayMusic(const char *filename) {
  if (!audioInitialized)
    return;

  if (musicPlaying) {
    StopMusic();
  }

  currentMusic = LoadMusicStream(filename);
  if (currentMusic.frameCount > 0) {
    PlayMusicStream(currentMusic);
    musicPlaying = true;
    printf("[Audio] Playing music: %s\n", filename);
  } else {
    printf("[Audio] Failed to load music: %s\n", filename);
  }
}

void UpdateMusic(void) {
  if (musicPlaying) {
    UpdateMusicStream(currentMusic);
  }
}

void StopMusic(void) {
  if (musicPlaying) {
    StopMusicStream(currentMusic);
    UnloadMusicStream(currentMusic);
    musicPlaying = false;
  }
}

// ==========================================
// CLEANUP
// ==========================================

void UnloadAudioSystem(void) {
  if (!audioInitialized)
    return;

  StopMusic();

  // TODO: Unload sound effects
  // for (int i = 0; i < SFX_COUNT; i++) {
  //     if (sounds[i].frameCount > 0) {
  //         UnloadSound(sounds[i]);
  //     }
  // }

  CloseAudioDevice();
  audioInitialized = false;
  printf("[Audio] Audio system unloaded\n");
}
