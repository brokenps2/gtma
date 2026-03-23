#include "diagnostics.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <GL/glew.h>
#include <stdarg.h>
#include <sys/utsname.h>
#include <stdio.h>

void gtmaPrintSystemInfo() {
    struct utsname sys;
    uname(&sys);
    printf("\ngtma - game toolkit with minimal abstraction\n");
    printf(">> built %s at %s\n", __DATE__, __TIME__);
    printf(">> Platform: %s %s (%s)\n", sys.sysname, sys.release, sys.machine);
    printf(">> OpenGL: %s\n", glGetString(GL_VERSION));
    printf(">> GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf(">> GPU: %s\n", glGetString(GL_RENDERER));
    printf(">> Vendor: %s\n", glGetString(GL_VENDOR));

    const ALchar* version = alGetString(AL_VERSION);

    printf(">> OpenAL: %s\n", version);
}
