#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../scenes/scenes.h"
#include "../window/events.h"
#include "util/util.h"
#include <time.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    extern Scene titleScreen;
    
    srand(time(NULL));

    if(argc < 2 || argc > 2) {
        printf("Usage: gtma [config file path]\n");
        exit(1);
    }

    gtmaInit(argv[1]);

    gtmaLoadInitialScene(&titleScreen);

    while(gtmaIsRunning()) {
        gtmaUpdate();
    }

    gtmaClose();

    printf("\n");

    return 0;
}
