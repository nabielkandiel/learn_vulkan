#pragma once

#include <array>
#include <concepts>
#include <span>
#include <utility>
#include "gridDimension.hpp"
#include "texture.hpp"

template <typename DirType> class Sprites
{
  private:
    static constexpr size_t TOTAL_DIRS = static_cast<size_t>(DirType::COUNT);
    Texture texture;
    std::array<SDL_FRect, TOTAL_DIRS> sprite_rects;
    std::array<DirType, TOTAL_DIRS> dir_map;
    DirType active_dir;

  public:
    // passed is (index in png -> direction it represents)
    Sprites(std::array<DirType, TOTAL_DIRS> dir_map, SDL_Color key) : dir_map(dir_map), texture(key) {}
    Sprites(std::array<DirType, TOTAL_DIRS> dir_map) : dir_map(dir_map), texture(std::nullopt) {}
    ~Sprites() = default;
    // dont allow this struct to be copied or moved
    Sprites(const Sprites &) = delete;            // Copy Constructor
    Sprites &operator=(const Sprites &) = delete; // Copy Assignment
    Sprites(Sprites &&) = delete;                 // Move Constructor
    Sprites &operator=(Sprites &&) = delete;      // Move Assignment

    Texture &getTexture()
    {
        return texture;
    }

    void sliceTextureHorizontal(std::same_as<float> auto spr_w, std::same_as<float> auto spr_h)
    {
        std::span<SDL_FRect, TOTAL_DIRS> view{sprite_rects};
        size_t idx = 0;
        for (const auto &dir : dir_map) {
            view[std::to_underlying(dir)] = {static_cast<float>(idx) * spr_w, 0.0F, spr_w, spr_h};
            idx++;
        }
    }

    void sliceTextureBox(std::same_as<float> auto spr_w, std::same_as<float> auto spr_h, GridDimensions dims)
    {
        std::span<SDL_FRect, TOTAL_DIRS> view{sprite_rects};
        size_t idx = 0;
        size_t idy = 0;
        for (const auto &dir : dir_map) {
            view[std::to_underlying(dir)] = {static_cast<float>(idx) * spr_w, static_cast<float>(idy) * spr_h, spr_w,
                                             spr_h};
            idx++;
            if (idx == dims.rows) {
                idx = 0;
                idy++;
            }
        }
    }

    void renderActive(SDL_Renderer *sdl_renderer, std::same_as<float> auto center_x, std::same_as<float> auto center_y)
    {
        const auto src_rect = getActiveRect();
        float render_x = center_x - (src_rect.w / 2.F);
        float render_y = center_y - (src_rect.h / 2.F);
        texture.render(render_x, render_y, sdl_renderer, &src_rect);
    }

    [[nodiscard]] const SDL_FRect &getActiveRect() const
    {
        return sprite_rects[std::to_underlying(active_dir)];
    }

    void setActiveDir(const DirType dir)
    {
        active_dir = dir;
    }
};
