#include "texture.hpp"
#include <optional>
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "SDL3_image/SDL_image.h"

Texture::Texture() : Texture(std::nullopt) {}

Texture::Texture(SDL_Color key) : Texture{std::optional<SDL_Color>{key}} {}

Texture::Texture(std::optional<SDL_Color> key) : color_key{key}, sdl_texture{nullptr}, m_width{0}, m_height{0} {}

Texture::~Texture()
{
    destory();
    sdl_texture = nullptr;
}

bool Texture::loadFromFile(const std::string &path, SDL_Renderer *sdl_renderer)
{
    destory();
    SDL_Surface *loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == nullptr) {
        SDL_Log("Unable to load image %s - SDL image error%s\n", path.c_str(), SDL_GetError());
        return false;
    }

    if (color_key.has_value()) {
        auto surface_rgb = SDL_MapSurfaceRGB(loaded_surface, color_key->r, color_key->g, color_key->b);
        if (!SDL_SetSurfaceColorKey(loaded_surface, true, surface_rgb)) {
            SDL_Log("Unable to color key - SDL error: %s\n", SDL_GetError());
            return false;
        }
    }

    sdl_texture = SDL_CreateTextureFromSurface(sdl_renderer, loaded_surface);
    if (sdl_texture == nullptr) {
        SDL_Log("Unable to create texture from surface - SDL error%s\n", SDL_GetError());
        SDL_DestroySurface(loaded_surface);
        return false;
    }
    m_width = loaded_surface->w;
    m_height = loaded_surface->h;
    SDL_DestroySurface(loaded_surface);

    return sdl_texture != nullptr;
}

void Texture::destory()
{
    SDL_DestroyTexture(sdl_texture);
    sdl_texture = nullptr;
    m_height = 0;
    m_width = 0;
}

void Texture::render(SDL_FPoint dst_cords, SDL_Renderer *sdl_renderer, SDL_FPoint *dst_dims, SDL_FRect *src_rect)
{
    auto dst_w = static_cast<float>(m_width);
    auto dst_h = static_cast<float>(m_height);
    if (dst_dims != nullptr) {
        dst_w = dst_dims->x;
        dst_h = dst_dims->y;
    } else if (src_rect != nullptr) {
        dst_w = src_rect->w;
        dst_h = src_rect->h;
    }
    SDL_FRect dst_rect{.x = dst_cords.x, .y = dst_cords.y, .w = dst_w, .h = dst_h};
    SDL_RenderTexture(sdl_renderer, sdl_texture, src_rect, &dst_rect);
}

void Texture::renderWithTransform(SDL_FPoint dst_cords, SDL_Renderer *sdl_renderer, SDL_FPoint *dst_dims,
                                  const SDL_FRect *src_rect, double degrees, SDL_FPoint *center, SDL_FlipMode flip_mode)
{
    auto dst_w = static_cast<float>(m_width);
    auto dst_h = static_cast<float>(m_height);
    if (dst_dims != nullptr) {
        dst_w = dst_dims->x;
        dst_h = dst_dims->y;
    } else if (src_rect != nullptr) {
        dst_w = src_rect->w;
        dst_h = src_rect->h;
    }
    SDL_FRect dst_rect{.x = dst_cords.x, .y = dst_cords.y, .w = dst_w, .h = dst_h};
    SDL_RenderTextureRotated(sdl_renderer, sdl_texture, src_rect, &dst_rect, degrees, center, flip_mode);
}
