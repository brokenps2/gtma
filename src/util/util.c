#include "graphics/renderer.h"
#include "scenes/scenes.h"
#include "util/config.h"
#include "window/events.h"
#include "window/windowManager.h"
#include "util/diagnostics.h"
#include "audio/audio.h"
#include <stdlib.h>

int randBetween(int a, int b) { 
    return rand() % (b - a + 1) + a;
}

void gtmaInit(const char* cfgPath) {
    gtmaInitConfig(cfgPath);
    gtmaInitWindow();
    gtmaInitRenderer();
    gtmaPrintSystemInfo();
}

void gtmaUpdate() {
    gtmaUpdateEvents();
    gtmaGetCurrentScene()->update();
    gtmaUpdateWindow();
}

void gtmaClose() {
    gtmaGetCurrentScene()->dispose();
    gtmaCloseRenderer();
    gtmaCloseAudio();
    gtmaDestroyConfig();
    gtmaCloseWindow();
}
