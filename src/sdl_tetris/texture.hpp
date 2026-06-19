#pragma once
#include <optional>
#include <string>
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"

class Texture
{
  public:
    Texture();
    Texture(SDL_Color key);
    Texture(std::optional<SDL_Color> key);
    ~Texture();

    // copy
    Texture(const Texture &texture) = delete;
    Texture &operator=(const Texture &texture) = delete;

    // move
    Texture(Texture &&texture) = delete;
    Texture &operator=(Texture &&texture) = delete;

    bool loadFromFile(const std::string &path, SDL_Renderer *sdl_renderer);
    void destory();
    void render(SDL_FPoint dst_cords, SDL_Renderer *sdl_renderer, SDL_FPoint *dst_dims, SDL_FRect *src_rect);
    void renderWithTransform(SDL_FPoint dst_cords, SDL_Renderer *sdl_renderer, SDL_FPoint *dst_dims,
                             const SDL_FRect *src_rect, double degrees, SDL_FPoint *center,
                             SDL_FlipMode = SDL_FLIP_NONE);

    [[nodiscard]] int getWidth() const
    {
        return m_width;
    }

    [[nodiscard]] int getHeight() const
    {
        return m_height;
    }

    [[nodiscard]] bool isLoaded() const
    {
        return sdl_texture != nullptr;
    }

  private:
    SDL_Texture *sdl_texture;
    std::optional<SDL_Color> color_key;
    int m_width;
    int m_height;
};
