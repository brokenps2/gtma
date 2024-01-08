//author: eli
//created: 1-2-2024
//main.cpp containing main method

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "windowmgr.h"
#include <SDL2/SDL_video.h>
#include "renderer.h"
#include "config.h"
#include "keyboard.h"

SDL_Event mainEvent;

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
    
    if(SDL_PollEvent(&mainEvent) && mainEvent.type == SDL_QUIT) break;

    if(getKeyDown('q')) {
      break;
    }
    if(getKeyDown('a')) {
      SDL_Log("cool");
    }

    updateWindow();
    
    updRenderer();

  }
 
  return 0;

}

SDL_Event* getMainEvent() {
  return &mainEvent;
}
