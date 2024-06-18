#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>
#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>
#include <cglm/vec3.h>
#include "Audio.h"

ALCcontext* audioContext;
ALCdevice* audioDevice;

void initAudio() {
    const ALCchar* defaultDevice = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    audioDevice = alcOpenDevice(defaultDevice);
    if (!audioDevice) {
        printf("unable to open default audio device\n");
        exit(1);
    }

    audioContext = alcCreateContext(audioDevice, NULL);
    if (!audioContext) {
        printf("unable to create audio context\n");
        alcCloseDevice(audioDevice);
        exit(1);
    }

    if (!alcMakeContextCurrent(audioContext)) {
        printf("unable to make AL context current\n");
        alcDestroyContext(audioContext);
        alcCloseDevice(audioDevice);
        exit(1);
    }
}

void createTrack(Track* track, const char* path, bool loop, float vol) {
    drwav_int16* rawAudioBuffer;
    unsigned int channels, sampleRate;
    drwav_uint64 sampleCount;
    rawAudioBuffer = drwav_open_file_and_read_pcm_frames_s16(path, &channels, &sampleRate, &sampleCount, NULL);

    if(!rawAudioBuffer) {
        printf("unable to open audio file\n");
        free(rawAudioBuffer);
        exit(1);
    }

    int format = -1;
    if (channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else {
        printf("unsupported number of channels: %d\n", channels);
        free(rawAudioBuffer);
        exit(1);
    }

    alGenBuffers(1, &track->bufferID);
    alBufferData(track->bufferID, format, rawAudioBuffer, sampleCount * channels * sizeof(drwav_int16), sampleRate);

    alGenSources(1, &track->sourceID);
    alSourcei(track->sourceID, AL_BUFFER, track->bufferID);
    if(loop) {
        alSourcei(track->sourceID, AL_LOOPING, AL_TRUE);
    } else {
        alSourcei(track->sourceID, AL_LOOPING, AL_FALSE);
    }
    alSourcei(track->sourceID, AL_POSITION, 0);
    alSourcei(track->sourceID, AL_GAIN, vol);

    free(rawAudioBuffer);

}

void createSound(Sound* sound, const char* path, bool loop, float vol, vec3 position) {
    drwav_int16* rawAudioBuffer;
    unsigned int channels, sampleRate;
    drwav_uint64 sampleCount;
    rawAudioBuffer = drwav_open_file_and_read_pcm_frames_s16(path, &channels, &sampleRate, &sampleCount, NULL);

    if(!rawAudioBuffer) {
        printf("unable to open audio file\n");
        free(rawAudioBuffer);
        exit(1);
    }

    int format = -1;
    if (channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else {
        printf("unsupported number of channels: %d\n", channels);
        free(rawAudioBuffer);
        exit(1);
    }

    alGenBuffers(1, &sound->bufferID);
    alBufferData(sound->bufferID, format, rawAudioBuffer, sampleCount * channels * sizeof(drwav_int16), sampleRate);

    alGenSources(1, &sound->sourceID);
    alSourcei(sound->sourceID, AL_BUFFER, sound->bufferID);
    if(loop) {
        alSourcei(sound->sourceID, AL_LOOPING, AL_TRUE);
    } else {
        alSourcei(sound->sourceID, AL_LOOPING, AL_FALSE);
    }
    alSourcei(sound->sourceID, AL_GAIN, vol);

    alSource3f(sound->sourceID, AL_POSITION, position[0], position[1], position[2]);

    alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

    alSourcef(sound->sourceID, AL_REFERENCE_DISTANCE, 1.0f);
    alSourcef(sound->sourceID, AL_MAX_DISTANCE, 50.0f);
    alSourcef(sound->sourceID, AL_ROLLOFF_FACTOR, 1.0f);

    free(rawAudioBuffer);

}

void setSoundPosition(Sound* sound, vec3 position) {
    alSource3f(sound->sourceID, AL_POSITION, position[0], position[1], position[2]);
}

void updateAudio(vec3 camPos) {
    alListener3f(AL_POSITION, camPos[0], camPos[1], camPos[2]);
}

void disposeTrack(Track* track) {
    alDeleteSources(1, &track->sourceID);
    alDeleteBuffers(1, &track->bufferID);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);
}

void disposeSound(Sound* sound) {
    alDeleteSources(1, &sound->sourceID);
    alDeleteBuffers(1, &sound->bufferID);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);
}

void playSound(Sound* sound) {
    int state;
    alGetSourcei(sound->sourceID, AL_SOURCE_STATE, &state);
    if(state == AL_STOPPED) {
        sound->isPlaying = false;
        alSourcei(sound->sourceID, AL_POSITION, 0);
    }

    if(!sound->isPlaying) {
        alSourcePlay(sound->sourceID);
        sound->isPlaying = true;
    }
}

void playTrack(Track* track) {
    int state;
    alGetSourcei(track->sourceID, AL_SOURCE_STATE, &state);
    if(state == AL_STOPPED) {
        track->isPlaying = false;
        alSourcei(track->sourceID, AL_POSITION, 0);
    }

    if(!track->isPlaying) {
        alSourcePlay(track->sourceID);
        track->isPlaying = true;
    }
}

void stopSound(Sound* sound) {
    alSourceStop(sound->sourceID);
    sound->isPlaying = false;
}

void stopTrack(Track* track) {
    alSourceStop(track->sourceID);
    track->isPlaying = false;
}

void destroyAudio() {
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);
}
