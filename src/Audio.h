#pragma once
#include <AL/al.h>
#include <cglm/vec3.h>

typedef struct Track {
    ALuint bufferID;
    ALuint sourceID;
    bool isPlaying;
} Track;

typedef struct Sound {
    ALuint bufferID;
    ALuint sourceID;
    bool isPlaying;
    vec3 position;
} Sound;


void initAudio();
void createTrack(Track* track, const char* path, bool loop, float vol);
void createSound(Sound* sound, const char* path, bool loop, float vol, vec3 position);
void disposeSound(Sound* sound);
void disposeTrack(Track* track);
void playSound(Sound* sound);
void playTrack(Track* track);
void updateAudio(vec3 camPos);
void stopTrack(Track* track);
void stopSound(Sound* sound);
void destroyAudio();
