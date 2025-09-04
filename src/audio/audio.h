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


void gtmaInitAudio();
void gtmaCreateTrack(Track* track, const char* path, bool loop, float vol);
void gtmaCreateSound(Sound* sound, const char* path, bool loop, float vol, vec3 position);
void gtmaDeleteSound(Sound* sound);
void gtmaDeleteTrack(Track* track);
void gtmaPlaySound(Sound* sound);
void gtmaPlaySoundFrom(Sound* sound, int seconds);
void gtmaPlayTrack(Track* track);
void gtmaPlayTrackFrom(Track* track, int seconds);
void gtmaUpdateAudio(vec3 camPos, vec3 cameraDir);
void gtmaStopTrack(Track* track);
void gtmaStopSound(Sound* sound);
void gtmaSetSoundPosition(Sound* sound, vec3 position);
void gtmaChangePitch(Sound* sound, float value);
void gtmaCloseAudio();
