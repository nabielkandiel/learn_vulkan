#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_video.h"
#include "utility.h"

int main()
{
    constexpr int k_screen_width{640};
    constexpr int k_screen_height{480};

    // window to be rendered to
    SDL_Window *g_window{};
    // surface in the window
    SDL_Surface *g_screen_surface{};
    // image to load and draw to screen
    SDL_Surface *g_hello_world{};

    SDL_Log("Starting with dimension %u x %u\n", k_screen_width, k_screen_height);
    if (!init(&g_window, &g_screen_surface, k_screen_width, k_screen_height)) {
        SDL_Log("failed to init SDL\n");
        return 1;
    }

    if (!loadMedia(&g_hello_world)) {
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

        // draw to screen
        SDL_FillSurfaceRect(g_screen_surface, nullptr, SDL_MapSurfaceRGB(g_screen_surface, 0xFF, 0xFF, 0xFF));
        SDL_BlitSurface(g_hello_world, nullptr, g_screen_surface, nullptr);
        SDL_UpdateWindowSurface(g_window);
    }

    close(&g_hello_world, &g_window, &g_screen_surface);
}
