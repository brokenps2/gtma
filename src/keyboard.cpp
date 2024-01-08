#include <SDL2/SDL.h>
#include "main.h"

int getKeyDown(char key) {
  
	if(getMainEvent()->type == SDL_KEYDOWN) {
		if (key == 'a' && getMainEvent()->key.keysym.sym == SDLK_a) {
			return 1;
	  }
    if (key == 'q' && getMainEvent()->key.keysym.sym == SDLK_q) {
			return 1;
	  }

	}
    
  return 0;

}
