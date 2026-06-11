#pragma once

#include <SDL3/SDL.h>
#include "texture.hpp"

// start SDL and creates window
bool init(SDL_Window **window, SDL_Renderer **renderer, int width, int height);

// loads media
bool loadMedia(Texture &sdl_texture, SDL_Renderer *renderer);

// frees media and shuts down SDL
void close(SDL_Window **window_pp, Texture &sdl_texture, SDL_Renderer **renderer);
