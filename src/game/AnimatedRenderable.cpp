/*
 * AnimatedRenderable.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: twiemann
 */

#include "AnimatedRenderable.hpp"
#include "TextureFactory.hpp"

#include <fstream>
#include <iostream>

using std::cout;
using std::endl;

namespace jumper
{

AnimatedRenderable::AnimatedRenderable(SDL_Renderer* renderer, std::string filename)
    : Renderable(renderer),
      m_numFrames(0),
      m_currentFrame(0),
      m_frameWidth(0),
      m_frameHeight(0),
      m_lastRenderTicks(0),
      m_frameTimeout(0)
{
    std::ifstream ifs(filename.c_str());
    std::string textureFileName;
    if(ifs.good())
    {
        ifs >> textureFileName >> m_frameWidth >> m_frameHeight >> m_numFrames;
        m_texture = TextureFactory::instance(m_renderer).getTexture(textureFileName);
    }
    else
    {
        std::cout << "Unable to open file " << filename << std::endl;
    }

    // Initialize source rect
    m_sourceRect.x = 0;
    m_sourceRect.y = 0;
    m_sourceRect.w = m_frameWidth;
    m_sourceRect.h = m_frameHeight;

}

AnimatedRenderable::AnimatedRenderable(SDL_Renderer* renderer, SDL_Texture* texture, int frameWidth, int frameHeight, int numFrames)
    : Renderable(renderer, texture),
      m_numFrames(numFrames),
      m_currentFrame(0),
      m_frameWidth(frameWidth),
      m_frameHeight(frameHeight),
      m_lastRenderTicks(0),
      m_frameTimeout(0)
{
    // Initialize source recnt
    m_sourceRect.x = 0;
    m_sourceRect.y = 0;
    m_sourceRect.w = m_frameWidth;
    m_sourceRect.h = m_frameHeight;
}

AnimatedRenderable::~AnimatedRenderable() {}

void AnimatedRenderable::setupExplosion()
{
    if (std::max(m_frameHeight,m_frameWidth)>40)
    {
        m_explodeFile = m_explodeFileBig;
    } else if ((std::max(m_frameHeight,m_frameWidth)>30))
    {
        m_explodeFile = m_explodeFileMedium;
    }  else
    {
        m_explodeFile = m_explodeFileSmall;
    }
}



void AnimatedRenderable::nextFrame()
{

    Uint32 ticks = SDL_GetTicks();
    float time =  (ticks - m_lastRenderTicks);

    if(time > m_frameTimeout)
    {
        // Check and increase frame counter
        if(m_currentFrame + 1 < m_numFrames)
        {
            m_currentFrame++;
        }
        else
        {
            m_currentFrame = 0;
        }

        // Setup source rect
        m_sourceRect.x = m_currentFrame * m_frameWidth;

        // Save current tick count
        m_lastRenderTicks = ticks;
    }
}

void AnimatedRenderable::setFPS(int frames)
{
    m_frameTimeout = (Uint32)(1000.0 / frames);
}

} /* namespace jumper */


