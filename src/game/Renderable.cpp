//
// Created by isaak on 25.11.15.
//

#include <SDL2/SDL_image.h>
#include <iostream>
#include "Renderable.hpp"
namespace jumper
{

Camera Renderable::m_camera;

Renderable::Renderable(SDL_Renderer* renderer)
    : m_renderer(renderer), m_texture(0)
{
    m_sourceRect.x = 0;
    m_sourceRect.y = 0;
    m_sourceRect.w = 0;
    m_sourceRect.h = 0;
}

Renderable::Renderable(SDL_Renderer* renderer, SDL_Texture* texture)
    : m_renderer(renderer), m_texture(texture)
{
    int w, h;
    SDL_QueryTexture(m_texture, NULL, NULL, &w, &h);

    m_sourceRect.x = 0;
    m_sourceRect.y = 0;
    m_sourceRect.w = w;
    m_sourceRect.h = h;
}

SDL_Renderer *Renderable::getRenderer() const
{
    return m_renderer;
}

SDL_Texture *Renderable::getTexture() const
{
    return m_texture;
}

int Renderable::w() const
{
    return m_sourceRect.w;
}

int Renderable::h() const
{
    return m_sourceRect.h;
}

Renderable::~Renderable() {}

} /* namespace jumper */


