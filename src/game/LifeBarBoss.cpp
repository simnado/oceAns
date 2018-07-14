/*
 * LifeBoard.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: doschulze, vtecker
 */

#include "LifeBarBoss.hpp"
#include <iostream>
#include <list>
using std::cout;
using std::endl;
using std::list;

namespace jumper
{

LifeBarBoss::LifeBarBoss(SDL_Renderer* renderer, SDL_Texture* texture, Bot* boss)
	: StaticRenderable(renderer, texture)
{
    m_boss = boss;
}

LifeBarBoss::LifeBarBoss(SDL_Renderer* renderer, Bot* boss)
	: StaticRenderable(renderer)
{
    m_boss = boss;
}

void LifeBarBoss::render()
{
    float prozentHealth;
    prozentHealth = ((float)m_boss->getHealth()/(float)m_boss->getMaxHealth());

    SDL_Rect srs;
    srs.w = 10;
    srs.h = 10;
    srs.x = 0;
    srs.y = 0;

    SDL_Rect dst;
    dst.w = prozentHealth*700;
    dst.h = 16;
    dst.x = 50;
    dst.y = 50;

    SDL_RenderCopy(m_renderer, TextureFactory::instance(m_renderer).getTexture("..\\res\\mario1.png"), &srs, &dst);

}



LifeBarBoss::~LifeBarBoss()
{
	// TODO Auto-generated destructor stub
}

} /* namespace jumper */

