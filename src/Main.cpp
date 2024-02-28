#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "WindowManager.h"
#include <SDL2/SDL_video.h>
#include "Renderer.h"
#include "Config.h"
#include "Events.h"
#include <SDL2/SDL_stdinc.h>

int main(int argc, char* argv[]) {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Init(SDL_INIT_AUDIO);
  SDL_Init(SDL_INIT_EVENTS);

  if(argc < 2 || argc > 2) {
    std::cout << "Usage: gtma [config file path]" << std::endl;
    std::cout << "this will be handled by the launcher eventually" << std::endl;
    exit(1);
  }

  cfgSetPath(argv[1]);

  initWindow();
  initRenderer();
 
  //initial setup finished, run start tasks here

  while(1) {

    if(checkForQuit()) {
      break;
    }

    updateWindow();

    render();

  }
 
  return 0;

}
