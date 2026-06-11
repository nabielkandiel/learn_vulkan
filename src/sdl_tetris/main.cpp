#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_video.h"
#include "texture.hpp"
#include "utility.h"

int main()
{
    constexpr int k_screen_width{640};
    constexpr int k_screen_height{480};

    // window to be rendered to
    SDL_Window *sdl_window{nullptr};
    // surface in the window
    SDL_Renderer *sdl_renderer{nullptr};
    Texture png_texture;

    SDL_Log("Starting with dimension %u x %u\n", k_screen_width, k_screen_height);
    if (!init(&sdl_window, &sdl_renderer, k_screen_width, k_screen_height)) {
        SDL_Log("failed to init SDL\n");
        return 1;
    }

    if (!loadMedia(png_texture, sdl_renderer)) {
        SDL_Log("failed to load media\n");
        return 2;
    }

    bool quit{false};
    SDL_Event event;
    SDL_zero(event);

    while (!quit) {
        // handle input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // set background to white
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sdl_renderer);
        // render image to screen
        png_texture.render(0.F, 0.F, sdl_renderer);
        // update screen
        SDL_RenderPresent(sdl_renderer);
    }

    close(&sdl_window, png_texture, &sdl_renderer);
    return 0;
}
