#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>
#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>
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

void createSound(Sound* sound, const char* path, bool loop, float vol) {
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
    alSourcei(sound->sourceID, AL_POSITION, 0);
    alSourcei(sound->sourceID, AL_GAIN, vol);

    free(rawAudioBuffer);

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

void stopSound(Sound* sound) {
    alSourceStop(sound->sourceID);
    sound->isPlaying = false;
}

void destroyAudio() {
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);
}
