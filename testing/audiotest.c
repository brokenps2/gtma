/*
#include <stdlib.h>
#include <stdbool.h>
#include <stb_vorbis.c>
#include <AL/al.h>
#include <AL/alc.h>

ALCcontext* audioContext;
ALCdevice* audioDevice;

typedef struct Sound {
    ALuint bufferID;
    ALuint sourceID;
    bool isPlaying;
} Sound;

void createSound(Sound* sound, const char* path, bool loop, float vol) {
    short* rawAudioBuffer;
    int channels, sampleRate;
    stb_vorbis_decode_filename(path, &channels, &sampleRate, &rawAudioBuffer);

    if(!rawAudioBuffer) {
        printf("unable to open audio file\n");
        free(rawAudioBuffer);
        exit(1);
    }

    int format = -1;
    if(channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if(channels == 2) {
        format = AL_FORMAT_STEREO16;
    }

    alGenBuffers(1, &sound->bufferID);
    alBufferData(sound->bufferID, format, rawAudioBuffer, sizeof(rawAudioBuffer), sampleRate);

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


int main() {
    const ALCchar* defaultDevice = alcGetString(0, ALC_DEFAULT_DEVICE_SPECIFIER);
    audioDevice = alcOpenDevice(defaultDevice);

    int attribs[] = {0};
    audioContext = alcCreateContext(audioDevice, attribs);
    alcMakeContextCurrent(audioContext);

    Sound sound;
    createSound(&sound, "test.ogg", true, 1);
    playSound(&sound);

    return 0;

}
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stb_vorbis.c>
#include <AL/al.h>
#include <AL/alc.h>
#include <stdio.h>

ALCcontext* audioContext;
ALCdevice* audioDevice;

typedef struct Sound {
    ALuint bufferID;
    ALuint sourceID;
    bool isPlaying;
} Sound;

void checkALError() {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        printf("OpenAL Error: %s\n", alGetString(error));
    }
}

void createSound(Sound* sound, const char* path, bool loop, float vol) {
    short* rawAudioBuffer;
    int channels, sampleRate;
    int sampleCount = stb_vorbis_decode_filename(path, &channels, &sampleRate, &rawAudioBuffer);

    if (sampleCount < 0) {
        printf("Unable to open audio file\n");
        free(rawAudioBuffer);
        exit(1);
    }

    int format = -1;
    if (channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else {
        printf("Unsupported number of channels: %d\n", channels);
        free(rawAudioBuffer);
        exit(1);
    }

    alGenBuffers(1, &sound->bufferID);

    alBufferData(sound->bufferID, format, rawAudioBuffer, sampleCount * channels * sizeof(short), sampleRate);

    alGenSources(1, &sound->sourceID);

    alSourcei(sound->sourceID, AL_BUFFER, sound->bufferID);

    alSourcei(sound->sourceID, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);

    alSourcef(sound->sourceID, AL_GAIN, vol);


    sound->isPlaying = false;

    free(rawAudioBuffer);
}

void playSound(Sound* sound) {
    int state;
    alGetSourcei(sound->sourceID, AL_SOURCE_STATE, &state);

    if (state == AL_STOPPED) {
        sound->isPlaying = false;
        alSourcei(sound->sourceID, AL_POSITION, 0);
    }

    if (!sound->isPlaying) {
        alSourcePlay(sound->sourceID);
        sound->isPlaying = true;
    }
}

int main() {
    const ALCchar* defaultDevice = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    audioDevice = alcOpenDevice(defaultDevice);
    if (!audioDevice) {
        printf("unable to open default audio device\n");
        return -1;
    }

    audioContext = alcCreateContext(audioDevice, NULL);
    if (!audioContext) {
        printf("unable to create audio context\n");
        alcCloseDevice(audioDevice);
        return -1;
    }

    if (!alcMakeContextCurrent(audioContext)) {
        printf("unable to make context current\n");
        alcDestroyContext(audioContext);
        alcCloseDevice(audioDevice);
        return -1;
    }

    Sound sound;
    createSound(&sound, "test.ogg", true, 1);
    playSound(&sound);

    // Cleanup
    alDeleteSources(1, &sound.sourceID);
    alDeleteBuffers(1, &sound.bufferID);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);

    return 0;
}
