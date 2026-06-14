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

void Texture::render(float x_cord, float y_cord, SDL_Renderer *sdl_renderer, const SDL_FRect *src_rect)
{
    auto src_w = static_cast<float>(m_width);
    auto src_h = static_cast<float>(m_height);
    if (src_rect != nullptr) {
        src_w = src_rect->w;
        src_h = src_rect->h;
    }
    SDL_FRect dst_rect{.x = x_cord, .y = y_cord, .w = src_w, .h = src_h};
    SDL_RenderTexture(sdl_renderer, sdl_texture, src_rect, &dst_rect);
}
