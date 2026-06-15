#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_video.h"
#include "sprites.hpp"
#include "utility.h"

int main()
{
    constexpr int k_screen_width{640};
    constexpr int k_screen_height{480};

    enum class Direction : uint8_t
    {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3,
        COUNT = 4
    };

    // window to be rendered to
    SDL_Window *sdl_window{nullptr};
    // surface in the window
    SDL_Renderer *sdl_renderer{nullptr};
    // character sprite
    Sprites<Direction> ball_sprite({Direction::LEFT, Direction::UP, Direction::DOWN, Direction::RIGHT},
                                   SDL_Color{.r = 0, .g = 180, .b = 180, .a = 0});

    ball_sprite.sliceTextureBox(32.0F, 32.0F, {.rows = 2, .cols = 2});

    SDL_Log("Starting with dimension %u x %u\n", k_screen_width, k_screen_height);
    if (!init(&sdl_window, &sdl_renderer, k_screen_width, k_screen_height)) {
        SDL_Log("failed to init SDL\n");
        return 1;
    }

    if (!loadMedia(ball_sprite.getTexture(), sdl_renderer, "balls.png")) {
        SDL_Log("failed to load media\n");
        return 2;
    }

    bool quit{false};
    SDL_Event event;
    SDL_zero(event);

    ball_sprite.setActiveDir(Direction::UP);
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
                    ball_sprite.setActiveDir(Direction::UP);
                    break;
                case SDLK_DOWN:
                    ball_sprite.setActiveDir(Direction::DOWN);
                    break;
                case SDLK_LEFT:
                    ball_sprite.setActiveDir(Direction::LEFT);
                    break;
                case SDLK_RIGHT:
                    ball_sprite.setActiveDir(Direction::RIGHT);
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
        float render_x = k_screen_width / 2.F;
        float render_y = k_screen_height / 2.F;
        ball_sprite.renderActive(sdl_renderer, render_x, render_y);

        // render ball

        // update screen
        SDL_RenderPresent(sdl_renderer);
    }

    close(&sdl_window, ball_sprite.getTexture(), &sdl_renderer);
    return 0;
}
