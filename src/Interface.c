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

void updateNuklear() {
    int w = 1280;
    int h = 210;
    int x = 0;
    int y = 0;
    if (nk_begin(ctx, "menu", nk_rect(x, y, w, h), NK_WINDOW_BORDER)) {
        enum {EASY, HARD};
        static int op = EASY;
        static int property = 20;
        nk_layout_row_static(ctx, 30, 80, 1);
        if (nk_button_label(ctx, "lock mouse")) {
            SDL_SetRelativeMouseMode(true);
        }
        nk_layout_row_dynamic(ctx, 30, 2);
        if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 25, 1);            
        if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx),400))) {
            nk_layout_row_dynamic(ctx, 120, 1);
            bg = nk_color_picker(ctx, bg, NK_RGBA);
            nk_layout_row_dynamic(ctx, 25, 1);
            bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
            bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
            bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f); 
            bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
            nk_combo_end(ctx);
        }
    }
}
void renderNuklear() {
    nk_sdl_render(NK_ANTI_ALIASING_OFF, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

struct nk_context* getContext() {
    return ctx;
}

