#pragma once

#include <array>
#include <span>
#include <utility>
#include "texture.hpp"

struct Sprites
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
    Sprites(std::array<Direction, 4> dir_map) : dir_map(dir_map) {}
    ~Sprites() = default;
    // dont allow this struct to be copied or moved
    Sprites(const Sprites &) = delete;            // Copy Constructor
    Sprites &operator=(const Sprites &) = delete; // Copy Assignment
    Sprites(Sprites &&) = delete;                 // Move Constructor
    Sprites &operator=(Sprites &&) = delete;      // Move Assignment

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
