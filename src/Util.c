#include "Util.h"
#include <SDL2/SDL_stdinc.h>
#include <nuklear.h>
#include "Camera.h"
#include "Interface.h"
#include <SDL2/SDL.h>
#include "Renderer.h"
#include "WindowManager.h"
#include "Events.h"
#include <stdbool.h>
#include <stdlib.h>

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

float ftoa_reverse(const char *str) {
    if (!str) return 0.0f;

    float result = 0.0f;
    int sign = 1;

    // Handle negative numbers
    if (*str == '-') {
        sign = -1;
        str++;
    }

    // Process integer part
    while (*str && *str != '.') {
        if (*str >= '0' && *str <= '9') {
            result = result * 10 + (*str - '0');
        } else {
            return 0.0f; // Invalid character
        }
        str++;
    }

    // Process fractional part
    if (*str == '.') {
        str++;
        float fraction = 0.0f;
        float divisor = 10.0f;

        while (*str) {
            if (*str >= '0' && *str <= '9') {
                fraction += (*str - '0') / divisor;
                divisor *= 10.0f;
            } else {
                return 0.0f; // Invalid character
            }
            str++;
        }

        result += fraction;
    }

    return result * sign;
}


int currentObj = 0;
float objSpeed = 25;
static struct nk_colorf clearColor = {0.03125, 0.03515625f, 0.0859375f, 1.0f};
bool showMenu = false;
int transformMode = 0;
int selected = nk_true;

bool isEditing = false;

void debugMenu() {

    static char nameBuffer[64] = "name";
    static char modelPathBuffer[128] = "models/model.glb";
    static char posXBuffer[32] = "0.0", posYBuffer[32] = "0.0", posZBuffer[32] = "0.0";
    static char scaleXBuffer[32] = "1.0", scaleYBuffer[32] = "1.0", scaleZBuffer[32] = "1.0";
    static char rotXBuffer[32] = "0.0", rotYBuffer[32] = "0.0", rotZBuffer[32] = "0.0";

    if(isKeyPressed(SDL_SCANCODE_E) && !isEditing) {
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

            nk_layout_row_dynamic(getContext(), 30, 1);
            nk_flags nameFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, nameBuffer, sizeof(nameBuffer), nk_filter_default);

            nk_flags pathFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, modelPathBuffer, sizeof(modelPathBuffer), nk_filter_default);

            nk_layout_row_dynamic(getContext(), 30, 3);
            nk_flags xFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, posXBuffer, sizeof(posXBuffer), nk_filter_float);
            nk_flags yFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, posYBuffer, sizeof(posYBuffer), nk_filter_float);
            nk_flags zFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, posZBuffer, sizeof(posZBuffer), nk_filter_float);
    
            nk_layout_row_dynamic(getContext(), 30, 3);
            nk_flags sxFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, scaleXBuffer, sizeof(scaleXBuffer), nk_filter_float);
            nk_flags syFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, scaleYBuffer, sizeof(scaleYBuffer), nk_filter_float);
            nk_flags szFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, scaleZBuffer, sizeof(scaleZBuffer), nk_filter_float);

            nk_layout_row_dynamic(getContext(), 30, 3);
            nk_flags rxFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, rotXBuffer, sizeof(rotXBuffer), nk_filter_float);
            nk_flags ryFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, rotYBuffer, sizeof(rotYBuffer), nk_filter_float);
            nk_flags rzFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, rotZBuffer, sizeof(rotZBuffer), nk_filter_float);

            nk_layout_row_dynamic(getContext(), 30, 1);
            if (nk_button_label(getContext(), "create")) {
                Object* newObj = (Object*)malloc(sizeof(Object));
                if (newObj) {
                    gtmaCreateObject(newObj, modelPathBuffer, nameBuffer,
                    atof(posXBuffer), atof(posYBuffer), atof(posZBuffer),
                    atof(scaleXBuffer), atof(scaleYBuffer), atof(scaleZBuffer),
                    atof(rotXBuffer), atof(rotYBuffer), atof(rotZBuffer));
                    gtmaAddObject(newObj);
                    SDL_SetRelativeMouseMode(true);
                }
            }
            
            if((nameFlags & NK_EDIT_ACTIVE) || (pathFlags & NK_EDIT_ACTIVE) || (xFlags & NK_EDIT_ACTIVE) || (yFlags & NK_EDIT_ACTIVE) || (zFlags & NK_EDIT_ACTIVE) || (sxFlags & NK_EDIT_ACTIVE) || (syFlags & NK_EDIT_ACTIVE) || (szFlags & NK_EDIT_ACTIVE) || (rxFlags & NK_EDIT_ACTIVE) || (ryFlags & NK_EDIT_ACTIVE) || (rzFlags & NK_EDIT_ACTIVE)) {
                isEditing = true;
            } else {
                isEditing = false;
            }

            if(nk_button_label(getContext(), "exit")) {
                SDL_SetRelativeMouseMode(true);
                showMenu = false; 
            }
            
            nk_end(getContext());
        }
    }
}

/*
void objsdMenu() {

    if(isKeyPressed(SDL_SCANCODE_C) && !isEditing) {
        if(showObjMenu) { SDL_SetRelativeMouseMode(true); } else { SDL_SetRelativeMouseMode(false);}   
        showObjMenu = !showObjMenu;
    }


    if(showObjMenu) {
        if(nk_begin(getContext(), "object creation menu", nk_rect(getWindowWidth() - 350, 50, 300, 400), NK_WINDOW_MOVABLE|NK_WINDOW_TITLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_BORDER|NK_WINDOW_SCALABLE)) {
            nk_layout_row_dynamic(getContext(), 30, 1);
            nk_flags pathFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, path, sizeof(path), nk_filter_ascii);
            nk_flags nameFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, name, sizeof(name), nk_filter_ascii);
            nk_layout_row_static(getContext(), 30, 60, 3);
            nk_flags xFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, x, sizeof(x), nk_filter_ascii);
            nk_flags yFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, y, sizeof(y), nk_filter_ascii);
            nk_flags zFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, z, sizeof(z), nk_filter_ascii);
            nk_layout_row_static(getContext(), 30, 60, 3);
            nk_flags sxFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, sx, sizeof(sx), nk_filter_ascii);
            nk_flags syFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, sy, sizeof(sy), nk_filter_ascii);
            nk_flags szFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, sz, sizeof(sz), nk_filter_ascii);
            nk_layout_row_static(getContext(), 30, 60, 3);
            nk_flags rxFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, rx, sizeof(rx), nk_filter_ascii);
            nk_flags ryFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, ry, sizeof(ry), nk_filter_ascii);
            nk_flags rzFlags = nk_edit_string_zero_terminated(getContext(), NK_EDIT_BOX|NK_EDIT_AUTO_SELECT, rz, sizeof(rz), nk_filter_ascii);

            nk_layout_row_dynamic(getContext(), 30, 1);
            if(nk_button_label(getContext(), "create object")) {
                nk_edit_unfocus(getContext());
                gtmaCreateAndAddObject(path, name, atof(x), atof(y), atof(z), atof(sx), atof(sy), atof(sz), atof(rx), atof(ry), atof(rz));
            }

            nk_spacer(getContext());

            if(nk_button_label(getContext(), "exit")) {
                SDL_SetRelativeMouseMode(true);
                showObjMenu = false;                
            }

            
        }
        nk_end(getContext());
    }

}
*/
    
