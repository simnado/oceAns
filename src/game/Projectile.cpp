/*
 * Projectile.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: doschulze, vtecker
 */

#include "Projectile.hpp"

#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

namespace jumper
{
Projectile::Projectile(SDL_Renderer* renderer, std::string filename, bool friendly, Vector2f direction, int velocity, int kollisionsschaden, Mixer* mixer)
    : Actor(renderer, filename, mixer)
{
    m_friendly = friendly;
    float abs = sqrt(pow(direction.x(),2)+pow(direction.y(),2));
    float x = direction.x()/abs*velocity;
    float y = direction.y()/abs*velocity;
    m_direction = Vector2f(x,y) ;
    m_velocity = velocity;
    m_kollisionsschaden = kollisionsschaden;
    m_explodeFile = m_explodeFileMedium;

}

Projectile::~Projectile()
{
    // TODO Auto-generated destructor stub
}

std::vector<Actor*> Projectile::onDeath()
{
    std::vector<Actor*> animation;
    KillAnimation* k = new KillAnimation(this,m_explodeFile, m_mixer);
    animation.push_back(k);
    return animation;
}

void Projectile::move(Level& level, float dt)
{
    nextFrame();
    if(dt > 0)
    {
        m_physicalProps.setPosition(physics().position()+ m_direction*dt);
    }
}




} /* namespace jumper */
