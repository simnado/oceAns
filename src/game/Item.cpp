/*
 * Item.cpp
 *
 *  Created on: Jan 1, 2016
 *      Author: twiemann
 */

#include "Item.hpp"
#include "KillAnimation.hpp"

namespace jumper
{
class KillAnimation;

Item::Item(SDL_Renderer* renderer, std::string filename, Mixer* mixer)
    : Actor(renderer, filename, mixer)
{
}

Item::Item(SDL_Renderer* renderer, SDL_Texture* texture, int frameWidth,
           int frameHeight, int numFrames, Mixer* mixer)
    : Actor(renderer, texture, frameWidth, frameHeight, numFrames, mixer)
{
}

void Item::move(Level& level, float dt)
{
    nextFrame();
}

void Item::setItemType(ItemType t)
{
    m_type = t;
    switch (t)
    {
    case STONE:
        m_explodeFile =m_explodeFileBlue;
        break;
    case HEART:
        m_explodeFile =m_explodeFileRred;
        break;
    case LIGHTNING:
        m_explodeFile =m_explodeFileGreen;
        break;
    case COIN:
        m_explodeFile =m_explodeFilPink;
        break;
     default:
        m_explodeFile=m_explodeFilOrange;
        break;

    }

}

std::vector<Actor*> Item::onDeath()
{
    m_mixer->play(this,"collect");
    std::vector<Actor*> animation;
    KillAnimation* k = new KillAnimation(this,m_explodeFile, m_mixer);
    animation.push_back(k);
    return animation;
}

Item::~Item()
{
    // TODO Auto-generated destructor stub
}

} // namespace jumper
