#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>
#include <unordered_map>
#include <utility>
#include "SDL3/SDL_events.h"
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
    bool sliceTextureHorizontal(T spr_w, T spr_h)
    {
        std::span<SDL_FRect, 4> view{sprite_rects};
        size_t idx = 0;
        for (const auto &dir : dir_map) {
            view[std::to_underlying(dir)] = {static_cast<float>(idx) * spr_w, 0.0F, spr_w, spr_h};
            idx++;
        }
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
