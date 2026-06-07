#include "utility.h"
#include <SDL3/SDL.h>
#include <string>
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_video.h"
#include "config.h"

bool init(SDL_Window **window_pp, SDL_Surface **surface_pp, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize - SDL error :%s\n", SDL_GetError());
        return false;
    }

    *window_pp = SDL_CreateWindow("Nasty Tetris", width, height, 0);
    if (*window_pp == nullptr) {
        SDL_Log("Window could not be created - SDL error: %s\n", SDL_GetError());
        return false;
    }

    *surface_pp = SDL_GetWindowSurface(*window_pp);
    return true;
}

// loads media
bool loadMedia(SDL_Surface **surface_pp)
{
    std::string image_path{ASSET_PATH};
    image_path += "luxray.png";
    *surface_pp = SDL_LoadPNG(image_path.c_str());
    if (*surface_pp == nullptr) {
        SDL_Log("Unable to load image %s - SDL error: %s\n", image_path.c_str(), SDL_GetError());
        return false;
    }
    return true;
}

// frees media and shuts down SDL
void close(SDL_Surface **surface_pp, SDL_Window **window_pp, SDL_Surface **screen_surface_pp)
{
    SDL_DestroySurface(*surface_pp);
    *surface_pp = nullptr;
    SDL_DestroyWindow(*window_pp);
    *window_pp = nullptr;
    *screen_surface_pp = nullptr;
}
