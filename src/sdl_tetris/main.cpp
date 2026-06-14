#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <print>
#include <span>
#include <utility>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_video.h"
#include "texture.hpp"
#include "utility.h"

struct SpriteTexture
{
    enum class Direction : uint8_t
    {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3,
    };

    Texture texture;
    std::array<SDL_FRect, 4> sprite_rects;
    std::array<Direction, 4> dir_map;
    Direction active_dir = Direction::UP;

    // passed is (index in png -> direction it represents)
    SpriteTexture(std::array<Direction, 4> dir_map) : dir_map(dir_map) {}
    ~SpriteTexture() = default;
    // dont allow this struct to be copied or moved
    SpriteTexture(const SpriteTexture &) = delete;            // Copy Constructor
    SpriteTexture &operator=(const SpriteTexture &) = delete; // Copy Assignment
    SpriteTexture(SpriteTexture &&) = delete;                 // Move Constructor
    SpriteTexture &operator=(SpriteTexture &&) = delete;      // Move Assignment

    template <typename T>
        requires std::same_as<T, float>
    void sliceTextureHorizontal(T spr_w, T spr_h)
    {
        std::span<SDL_FRect, 4> view{sprite_rects};
        size_t idx = 0;
        for (const auto &dir : dir_map) {
            view[std::to_underlying(dir)] = {static_cast<float>(idx) * spr_w, 0.0F, spr_w, spr_h};
            idx++;
        }
    }

    [[nodiscard]] const SDL_FRect &getActiveRect() const
    {
        switch (active_dir) {
        case Direction::UP:
            return sprite_rects[std::to_underlying(Direction::UP)];
        case Direction::DOWN:
            return sprite_rects[std::to_underlying(Direction::DOWN)];
        case Direction::LEFT:
            return sprite_rects[std::to_underlying(Direction::LEFT)];
        case Direction::RIGHT:
            return sprite_rects[std::to_underlying(Direction::RIGHT)];
        }
        std::unreachable();
    }
};

int main()
{
    constexpr int k_screen_width{640};
    constexpr int k_screen_height{480};

    // window to be rendered to
    SDL_Window *sdl_window{nullptr};
    // surface in the window
    SDL_Renderer *sdl_renderer{nullptr};
    // character sprite
    SpriteTexture character_texture({SpriteTexture::Direction::LEFT, SpriteTexture::Direction::UP,
                                     SpriteTexture::Direction::DOWN, SpriteTexture::Direction::RIGHT});

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

    bool quit{false};
    SDL_Event event;
    SDL_zero(event);

    character_texture.active_dir = SpriteTexture::Direction::UP;
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
                    character_texture.active_dir = SpriteTexture::Direction::UP;
                    break;
                case SDLK_DOWN:
                    character_texture.active_dir = SpriteTexture::Direction::DOWN;
                    break;
                case SDLK_LEFT:
                    character_texture.active_dir = SpriteTexture::Direction::LEFT;
                    break;
                case SDLK_RIGHT:
                    character_texture.active_dir = SpriteTexture::Direction::RIGHT;
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
        float render_w = (k_screen_width - src_rect.w) / 2.F;
        float render_h = (k_screen_height - src_rect.h) / 2.F;
        character_texture.texture.render(render_w, render_h, sdl_renderer, &src_rect);

        // update screen
        SDL_RenderPresent(sdl_renderer);
    }

    close(&sdl_window, character_texture.texture, &sdl_renderer);
    return 0;
}
