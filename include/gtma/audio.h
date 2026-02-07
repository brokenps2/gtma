#include <AL/al.h>
#include <cglm/vec3.h>
#pragma once

typedef struct Sound {
    ALuint bufferID;
    ALuint sourceID;
    bool isPlaying;
    vec3 position;
} Sound;


void gtmaInitAudio();
void gtmaCreateSound(Sound* sound, const char* path, bool loop, float vol, vec3 position);
void gtmaDeleteSound(Sound* sound);
void gtmaPlaySound(Sound* sound);
void gtmaPlaySoundFrom(Sound* sound, int seconds);
void gtmaUpdateAudio(vec3 camPos, vec3 cameraDir);
void gtmaStopSound(Sound* sound);
void gtmaSetSoundPosition(Sound* sound, vec3 position);
void gtmaChangePitch(Sound* sound, float value);
void gtmaCloseAudio();
