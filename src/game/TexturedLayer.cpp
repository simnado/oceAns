/*
 * TexturedLayer.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: twiemann
 *      Author: ledelkott
 *      Author: tstahn
 */

#include "TexturedLayer.hpp"

#include <cstdlib>
#include <iostream>

using std::cout;
using std::endl;

namespace jumper
{


TexturedLayer::TexturedLayer(SDL_Renderer* renderer)
    : StaticRenderable(renderer)
{
    m_scrollSpeed = 1.0f;
    m_upperTexture = 0;
    m_upperScrollSpeed = 0;
    // change background color to blue
    SDL_SetRenderDrawColor(m_renderer, 10, 76, 124, 255);
}

TexturedLayer::TexturedLayer(SDL_Renderer* renderer, SDL_Texture* texture)
    : StaticRenderable(renderer, texture)
{
    m_scrollSpeed = 1.0f;
    m_upperTexture = 0;
    m_upperScrollSpeed = 0;
}

void TexturedLayer::render()
{
    renderUpperLayer();
    renderLayer();
}

void TexturedLayer::renderUpperLayer()
{
    if (!m_upperTexture)
        return;

    SDL_Rect target;

    int xOff = m_camera.x();
    int yOff = m_camera.y();
    // Handle sign (why is there no signum function in C++?
    if(m_camera.x() > 0)
        xOff = -xOff;

    if(m_camera.y() > 0)
        yOff = -yOff;

    // Start rendering
    target.x = xOff;
    target.y = yOff;
    target.w = m_upperSourceRect.w;
    target.h = m_upperSourceRect.h;

    // If camera too low to display upperLayer
    if (target.h <= 0)
        return;
    // render the same image until the whole camera view width is filled, but render only the height of the image
    while (target.x < m_camera.w())
    {
        SDL_RenderCopy(m_renderer, m_upperTexture, &m_upperSourceRect, &target);
        target.x += m_upperSourceRect.w;
    }
}

void TexturedLayer::renderLayer()
{
    if (!m_texture)
        return;

    SDL_Rect target;

    int xOff = m_camera.x();
    int yOff = m_camera.y();
    
    // Handle sign (why is there no signum function in C++?
    if(m_camera.x() > 0)
        xOff = -xOff;

    if(m_camera.y() > 0)
        yOff = -yOff;

    target.x = xOff;
    // start rendering at the height of the upperTexture if an upperTexture exists
    if (m_upperTexture)
    {
        target.y = yOff + (m_upperSourceRect.h);
    }
    // start rendering at the beginning if no upperTexture exists
    else
    {
        target.y = yOff;
    }

    target.w = m_sourceRect.w;
    target.h = m_sourceRect.h;

    // start rendering, until the whole camera view is filled (width and height)
    while (target.y < m_camera.h())
    {
        while (target.x < m_camera.w())
        {
            SDL_RenderCopy(m_renderer, m_texture, &m_sourceRect, &target);
            target.x += m_sourceRect.w;
        }
        target.y += m_sourceRect.h;
        target.x = xOff;
    }
}


void TexturedLayer::setTexture(SDL_Texture* texture)
{
    m_texture = texture;

    int w, h;
    // queries the attributes width and height  of the texture
    SDL_QueryTexture(m_texture, NULL, NULL, &w, &h);

    m_sourceRect.x = 0;
    m_sourceRect.y = 0;
    m_sourceRect.w = w;
    m_sourceRect.h = h;
};

void TexturedLayer::setUpperTexture(SDL_Texture* texture)
{
    m_upperTexture = texture;

    int w, h;
    SDL_QueryTexture(m_upperTexture, NULL, NULL, &w, &h);

    m_upperSourceRect.x = 0;
    m_upperSourceRect.y = 0;
    m_upperSourceRect.w = w;
    m_upperSourceRect.h = h;
};

TexturedLayer::~TexturedLayer()
{
}

} /* namespace jumper */
