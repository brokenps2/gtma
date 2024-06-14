#pragma once
#include <AL/al.h>

typedef struct Sound {
    ALuint bufferID;
    ALuint sourceID;
    bool isPlaying;
} Sound;

void initAudio();
void createSound(Sound* sound, const char* path, bool loop, float vol);
void disposeSound(Sound* sound);
void playSound(Sound* sound);
void stopSound(Sound* sound);
void destroyAudio();
