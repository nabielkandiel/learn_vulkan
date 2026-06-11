#pragma once
#include <string>
#include "SDL3/SDL_render.h"

class Texture
{
  public:
    Texture();
    ~Texture();

    // copy
    Texture(const Texture &texture) = delete;
    Texture &operator=(const Texture &texture) = delete;

    // move
    Texture(Texture &&texture) = delete;
    Texture &operator=(Texture &&texture) = delete;

    bool loadFromFile(const std::string &path, SDL_Renderer *sdl_renderer);
    void destory();
    void render(float x_cord, float y_cord, SDL_Renderer *sdl_renderer);

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
        return sdl_texture == nullptr;
    }

  private:
    SDL_Texture *sdl_texture;
    int m_width;
    int m_height;
};
