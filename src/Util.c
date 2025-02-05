#include "Util.h"
#include <SDL2/SDL_stdinc.h>
#include <nuklear.h>
#include "Interface.h"
#include <SDL2/SDL.h>
#include "Renderer.h"
#include "WindowManager.h"
#include "Events.h"
#include <stdbool.h>

float glc(float color) {
    float glcolor;
    glcolor = color/255;
    return glcolor;
}

char *ftoa(float num, int precision) {
    char *str = (char *)malloc(32);
    if (!str) return NULL;

    int intPart = (int)num;
    float fracPart = fabs(num - intPart);

    sprintf(str, "%d", intPart);

    if (precision > 0) {
        char *ptr = str;
        while (*ptr) ptr++;
        *ptr++ = '.';

        for (int i = 0; i < precision; i++) {
            fracPart *= 10;
            int digit = (int)fracPart;
            *ptr++ = '0' + digit;
            fracPart -= digit;
        }
        *ptr = '\0';
    }

    return str;
}

int currentObj = 0;
float objSpeed = 25;
static struct nk_colorf clearColor = {0.03125, 0.03515625f, 0.0859375f, 1.0f};
bool showMenu = false;
int transformMode = 0;
int selected = nk_true;

void debugMenu() {

    if(isKeyPressed(SDL_SCANCODE_E)) {
        if(showMenu) { SDL_SetRelativeMouseMode(true); } else { SDL_SetRelativeMouseMode(false);}   
        showMenu = !showMenu;
    }

    if(showMenu) {
        if(isKeyDown(SDL_SCANCODE_UP)) {
            if(transformMode == 0) { 
            getObjPack()->objects[currentObj]->position[2] -= objSpeed * getDeltaTime(); 
            } else if(transformMode == 1) {
            getObjPack()->objects[currentObj]->rotation[2] -= objSpeed * getDeltaTime(); };
        }
        if(isKeyDown(SDL_SCANCODE_DOWN)) {
            if(transformMode == 0) { 
            getObjPack()->objects[currentObj]->position[2] += objSpeed * getDeltaTime(); 
            } else if(transformMode == 1) {
            getObjPack()->objects[currentObj]->rotation[2] += objSpeed * getDeltaTime(); };
        }
        if(isKeyDown(SDL_SCANCODE_LEFT)) {
            if(transformMode == 0) { 
            getObjPack()->objects[currentObj]->position[0] -= objSpeed * getDeltaTime(); 
            } else if(transformMode == 1) {
            getObjPack()->objects[currentObj]->rotation[1] -= objSpeed * getDeltaTime(); }
        }
        if(isKeyDown(SDL_SCANCODE_RIGHT)) {
            if(transformMode == 0) { 
            getObjPack()->objects[currentObj]->position[0] += objSpeed * getDeltaTime(); 
            } else if(transformMode == 1) {
            getObjPack()->objects[currentObj]->rotation[1] += objSpeed * getDeltaTime(); }
        }
        if(isKeyDown(SDL_SCANCODE_PAGEUP)) {
            getObjPack()->objects[currentObj]->position[1] += objSpeed * getDeltaTime();
        }
        if(isKeyDown(SDL_SCANCODE_PAGEDOWN)) {
            getObjPack()->objects[currentObj]->position[1] -= objSpeed * getDeltaTime();
        }

        if(nk_begin(getContext(), "menu", nk_rect(50, 50, 300, getWindowHeight() - 100), NK_WINDOW_MOVABLE|NK_WINDOW_TITLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_BORDER|NK_WINDOW_SCALABLE)) {
            nk_layout_row_static(getContext(), 30, 150, 1);
            
            for(int i=0; i < getObjPack()->objectCount; i++) {
                if(nk_option_label(getContext(), getObjPack()->objects[i]->name, currentObj == i)) {
                    currentObj = i;
                }
            }

            nk_layout_row_dynamic(getContext(), 30, 3);
            if(nk_option_label(getContext(), "position", transformMode == 0)) {
                transformMode = 0;
            }
            if(nk_option_label(getContext(), "rotation", transformMode == 1)) {
                transformMode = 1;
            }

            nk_layout_row_dynamic(getContext(), 20, 3);
            nk_selectable_label(getContext(), ftoa(getObjPack()->objects[currentObj]->position[0], 2), NK_LEFT, &selected);
            nk_selectable_label(getContext(), ftoa(getObjPack()->objects[currentObj]->position[1], 2), NK_LEFT, &selected);
            nk_selectable_label(getContext(), ftoa(getObjPack()->objects[currentObj]->position[2], 2), NK_LEFT, &selected);
            nk_layout_row_dynamic(getContext(), 30, 1);
            nk_slider_float(getContext(), 0, &objSpeed, 60, 0.25);
            if(nk_button_label(getContext(), "delete")) {
                gtmaRemoveObject(getObjPack()->objects[currentObj]);
            }
            nk_spacer(getContext());

            nk_layout_row_dynamic(getContext(), 200, 1);
            clearColor = nk_color_picker(getContext(), clearColor, NK_RGBA);
            gtmaSetClearColor(clearColor.r * 256, clearColor.g * 256, clearColor.b * 256);

            nk_layout_row_dynamic(getContext(), 30, 1);
            nk_spacer(getContext());
    
            if(nk_button_label(getContext(), "exit")) {
                SDL_SetRelativeMouseMode(true);
                showMenu = false;                }
            }

            nk_end(getContext());
        }
    
}
