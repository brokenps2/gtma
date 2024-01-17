//author: eli
//created: 1-2-2024
//main.cpp containing main method

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "WindowManager.h"
#include <SDL2/SDL_video.h>
#include "Renderer.h"
#include "Config.h"
#include <stdio.h>
#include <SDL2/SDL_stdinc.h>

SDL_Event mainEvent;
int cool = 0;

int main(int argc, char* argv[]) {

  SDL_Init(SDL_INIT_EVERYTHING);

  if(argc < 2 || argc > 2) {
    std::cout << "Usage: chiselengine [config file path]";
    std::cout << "this will be handled by the launcher eventually";
    exit(1);
  }

  cfgSetPath(argv[1]);

  initWindow();
  initRenderer();

  
  //initial setup finished, run start tasks here

  while(1) {
    
    if(SDL_PollEvent(&mainEvent) && mainEvent.type == SDL_QUIT) break;

    updateWindow();

    //std::cout << cool << std::endl;
    cool++;

    render();

  }
 
  return 0;

}

SDL_Event* getMainEvent() {
  return &mainEvent;
}
