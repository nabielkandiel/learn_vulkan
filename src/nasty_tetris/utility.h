#pragma once

#include <SDL3/SDL.h>

// start SDL and creates window
bool init(SDL_Window **window, SDL_Surface **surface, int width, int height);

// loads media
bool loadMedia(SDL_Surface **surface);

// frees media and shuts down SDL
void close(SDL_Surface **surface_pp, SDL_Window **window_pp, SDL_Surface **screen_surface_pp);
