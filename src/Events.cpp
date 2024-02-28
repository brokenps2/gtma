#include <SDL2/SDL.h>

int checkForQuit() {
  SDL_Event qev;
  if(SDL_PollEvent(&qev) && qev.type == SDL_QUIT) {
    return 1;
  }

  return 0;
}
