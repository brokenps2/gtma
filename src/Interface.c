#include <SDL2/SDL_mouse.h>
#include <stdbool.h>
#include "WindowManager.h"
#include "Files.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include <GL/glew.h>
#include <nuklear.h>
#include <nuklear_sdl_gl3.h>
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
struct nk_context* ctx;
struct nk_colorf bg;
struct nk_font_atlas* atlas;
void initNuklear() {

        
    ctx = nk_sdl_init(getWindow());

    {
        nk_sdl_font_stash_begin(&atlas);
        struct nk_font *termin = nk_font_atlas_add_from_file(atlas, res("fonts/Times.ttf"), 24, 0);
        termin->config->pixel_snap = true;
        termin->config->oversample_v = 4;
        termin->config->oversample_h = 4;
        nk_sdl_font_stash_end();
        nk_style_set_font(ctx, &termin->handle);
    }
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
}


void renderNuklear() {
    nk_sdl_render(NK_ANTI_ALIASING_OFF, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

struct nk_context* getContext() {
    return ctx;
}

