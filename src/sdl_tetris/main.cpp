#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_video.h"
#include "sprites.hpp"
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
    // character sprite
    Sprites character_texture(
        {Sprites::Direction::LEFT, Sprites::Direction::UP, Sprites::Direction::DOWN, Sprites::Direction::RIGHT});
    Texture ball_texture{SDL_Color{.r = 0, .g = 179, .b = 180, .a = 0}};

    character_texture.sliceTextureHorizontal(105.0F, 420.0F);

    SDL_Log("Starting with dimension %u x %u\n", k_screen_width, k_screen_height);
    if (!init(&sdl_window, &sdl_renderer, k_screen_width, k_screen_height)) {
        SDL_Log("failed to init SDL\n");
        return 1;
    }

    if (!loadMedia(character_texture.texture, sdl_renderer, "arrows.png")) {
        SDL_Log("failed to load media\n");
        return 2;
    }

    if (!loadMedia(ball_texture, sdl_renderer, "ball.png")) {
        SDL_Log("falied to load media\n");
        return 3;
    }

    bool quit{false};
    SDL_Event event;
    SDL_zero(event);

    character_texture.active_dir = Sprites::Direction::UP;
    // default background to white
    SDL_Color bg_color{.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF};

    while (!quit) {
        // handle input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                case SDLK_UP:
                    character_texture.active_dir = Sprites::Direction::UP;
                    break;
                case SDLK_DOWN:
                    character_texture.active_dir = Sprites::Direction::DOWN;
                    break;
                case SDLK_LEFT:
                    character_texture.active_dir = Sprites::Direction::LEFT;
                    break;
                case SDLK_RIGHT:
                    character_texture.active_dir = Sprites::Direction::RIGHT;
                    break;
                default:
                    break;
                }
            }
        }

        bg_color.r = 0xFF;
        bg_color.g = 0xFF;
        bg_color.b = 0xFF;

        // set background color based on keys state
        const bool *key_states = SDL_GetKeyboardState(nullptr);
        if (key_states[SDL_SCANCODE_UP]) {
            bg_color.r = 0xFF;
            bg_color.g = 0x00;
            bg_color.b = 0x00;
        }
        if (key_states[SDL_SCANCODE_DOWN]) {
            bg_color.r = 0x00;
            bg_color.g = 0xFF;
            bg_color.b = 0x00;
        }
        if (key_states[SDL_SCANCODE_LEFT]) {
            bg_color.r = 0xFF;
            bg_color.g = 0xFF;
            bg_color.b = 0x00;
        }
        if (key_states[SDL_SCANCODE_RIGHT]) {
            bg_color.r = 0x00;
            bg_color.g = 0x00;
            bg_color.b = 0xFF;
        }
        // set background to white
        SDL_SetRenderDrawColor(sdl_renderer, bg_color.r, bg_color.g, bg_color.b, 0xFF);
        SDL_RenderClear(sdl_renderer);
        // render image to screen
        const SDL_FRect src_rect = character_texture.getActiveRect();
        float render_x = (k_screen_width - src_rect.w) / 2.F;
        float render_y = (k_screen_height - src_rect.h) / 2.F;
        character_texture.texture.render(render_x, render_y, sdl_renderer, &src_rect);

        // render ball
        ball_texture.render(0.F, 0.F, sdl_renderer);

        // update screen
        SDL_RenderPresent(sdl_renderer);
    }

    close(&sdl_window, character_texture.texture, &sdl_renderer);
    return 0;
}
