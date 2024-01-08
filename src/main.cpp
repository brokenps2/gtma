//author: eli
//created: 1-2-2024
//main.cpp containing main method

#include <string>
#include <SDL2/SDL.h>
#include "windowmgr.h"
#include <SDL2/SDL_video.h>
#include "renderer.h"
#include "config.h"

int main(int argc, char* argv[]) {

  SDL_Init(SDL_INIT_EVERYTHING);

  if(argc < 2 || argc > 2) {
    printf("Usage: chiselengine [config file path]\n");
    printf("this will be handled by the launcher eventually\n");
    exit(1);
  }

  cfgSetPath(argv[1]);

  initWindow();
  initRenderer();

  
  //initial setup finished, run start tasks here

  cfgPrintAllValues();


  while(1) {
    
    SDL_Event quitEvent;
    if(SDL_PollEvent(&quitEvent) && quitEvent.type == SDL_QUIT) break;

    updateWindow();
    
    updRenderer();

  }
 
  return 0;

}
