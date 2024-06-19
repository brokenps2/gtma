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
void playSoundFrom(Sound* sound, int seconds);
void playTrack(Track* track);
void playTrackFrom(Track* track, int seconds);
void updateAudio(vec3 camPos, vec3 cameraDir);
void stopTrack(Track* track);
void stopSound(Sound* sound);
void destroyAudio();
