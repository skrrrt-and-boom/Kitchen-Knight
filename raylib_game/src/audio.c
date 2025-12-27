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

// SFX Pool
static Sound sounds[SFX_COUNT];
static bool soundsLoaded[SFX_COUNT];

// ==========================================
// INITIALIZATION
// ==========================================

void InitAudioSystem(void) {
  InitAudioDevice();
  audioInitialized = true;
  printf("[Audio] Audio device initialized\n");

  // Load sound effects (will fail gracefully if files missing)
  const char *sfxFiles[] = {
      "assets/sfx_whoosh.wav",    // SFX_ATTACK_MELEE
      "assets/sfx_splat.wav",     // SFX_ATTACK_RANGED
      "assets/sfx_hit.wav",       // SFX_HIT_ENEMY
      "assets/sfx_oof.wav",       // SFX_PLAYER_HURT
      "assets/sfx_explosion.wav", // SFX_ENEMY_DEATH
      "assets/sfx_pickup.wav"     // SFX_PICKUP
  };

  for (int i = 0; i < SFX_COUNT; i++) {
    sounds[i] = LoadSound(sfxFiles[i]);
    if (sounds[i].frameCount > 0) {
      soundsLoaded[i] = true;
      printf("[Audio] Loaded SFX: %s\n", sfxFiles[i]);
    } else {
      soundsLoaded[i] = false;
    }
  }
}

// ==========================================
// SOUND EFFECTS
// ==========================================

void PlaySFX(SFXType type) {
  if (!audioInitialized)
    return;

  if (type < SFX_COUNT && soundsLoaded[type]) {
    PlaySound(sounds[type]);
  } else {
    // Debug output if sound not found
    const char *names[] = {"ATTACK_MELEE", "ATTACK_RANGED", "HIT_ENEMY",
                           "PLAYER_HURT",  "ENEMY_DEATH",   "PICKUP"};
    if (type < SFX_COUNT) {
      printf("[Audio] Play SFX: %s (file missing)\n", names[type]);
    }
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

  // Unload sound effects
  for (int i = 0; i < SFX_COUNT; i++) {
    if (soundsLoaded[i]) {
      UnloadSound(sounds[i]);
    }
  }

  CloseAudioDevice();
  audioInitialized = false;
  printf("[Audio] Audio system unloaded\n");
}
