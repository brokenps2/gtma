#include "diagnostics.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <GL/glew.h>
#include <stdarg.h>
#include <sys/utsname.h>
#include <stdio.h>

void gtmaLog(LogLevel level, const char* fmt, ...) {
    const char* prefix = "[INFO]";
    if(level == LOG_WARN) prefix = "[WARN]";
    if(level == LOG_ERROR) prefix = "[ERROR]";

    va_list args;
    va_start(args, fmt);

    printf("%s ", prefix);
    vprintf(fmt, args);
    printf("\n");

    va_end(args);
}

void gtmaPrintSystemInfo() {
    struct utsname sys;
    uname(&sys);
    gtmaLog(LOG_INFO, "gtma - game toolkit with minimal abstraction");
    gtmaLog(LOG_INFO, "built %s at %s", __DATE__, __TIME__);
    gtmaLog(LOG_INFO, "Platform: %s %s (%s)", sys.sysname, sys.release, sys.machine);
    gtmaLog(LOG_INFO, "OpenGL: %s", glGetString(GL_VERSION));
    gtmaLog(LOG_INFO, "GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    gtmaLog(LOG_INFO, "GPU: %s", glGetString(GL_RENDERER));
    gtmaLog(LOG_INFO, "Vendor:%s", glGetString(GL_VENDOR));

    ALCdevice* device = alcGetContextsDevice(alcGetCurrentContext());
    const ALCchar* name = alcGetString(device, ALC_DEVICE_SPECIFIER);
    const ALchar* version = alGetString(AL_VERSION);

    gtmaLog(LOG_INFO, "OpenAL: %s", version);
    gtmaLog(LOG_INFO, "Audio Device: %s", name);
}
