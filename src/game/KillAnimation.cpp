/*
 * KillAnimation.cpp
 *
 *  Created on: Feb 21, 2016
 *      Author: twiemann
 */

#include "KillAnimation.hpp"

#include <iostream>

using std::cout;
using std::endl;

namespace jumper
{

KillAnimation::KillAnimation(Actor* actor, Mixer* mixer)
    : Item(actor->getRenderer(), actor->getTexture(), actor->frameWidth(), actor->frameHeight(), actor->numFrames(), mixer)
{
	setPosition(actor->position());
	m_lastRenderTicks = 0;

    m_deathAnimFrames = 17;

    m_dst.w = 70;
	m_dst.h = 76;



    m_srs.w = 70;
	m_srs.h = 76;
	m_srs.x = m_framePosX;
	m_srs.y = 0;

    m_deathAnim = TextureFactory::instance(m_renderer).getTexture("..\\res\\Data\\Explosionen\\Explosion.png");
}


KillAnimation::KillAnimation(Actor* actor, std::string filename, Mixer* mixer)
    : Item(actor->getRenderer(), actor->getTexture(), actor->frameWidth(), actor->frameHeight(), actor->numFrames(), mixer)
{
	setPosition(actor->position());
	m_lastRenderTicks = 0;


    std::ifstream ifs(filename.c_str());
	std::string textureFileName;

    int frameWidth = 0;
    int frameHeight = 0;

	if(ifs.good())
	{
		ifs >> textureFileName >> frameWidth >> frameHeight >> m_deathAnimFrames;
        m_deathAnim = TextureFactory::instance(m_renderer).getTexture(textureFileName);
	}
	else
	{
		std::cout << "Unable to open file " << filename << std::endl;
	}


    m_dst.w = frameWidth;
	m_dst.h = frameHeight;



    m_srs.w = frameWidth;
	m_srs.h = frameHeight;
	m_srs.x = m_framePosX;
	m_srs.y = 0;
}

void KillAnimation::render()
{
	Uint32 ticks = SDL_GetTicks();
	float time =  (ticks - m_lastRenderTicks);

    m_dst.x = floor(m_physicalProps.position().x()) - m_camera.x() - (m_dst.w -frameWidth() )/2;
    m_dst.y = floor(m_physicalProps.position().y()) - m_camera.y() - (m_dst.h- frameHeight())/2;

    m_srs.x = m_framePosX;
    int faktor = 10-(floor)(m_deathAnimFrames/2.5);

    if (faktor < 3)
        faktor = 3;

    if(time > faktor* m_deathAnimFrames)
	{

        m_framePosX += m_srs.w;
        m_currentFrame++;
		// Save current tick count
		m_lastRenderTicks = ticks;
	}

    if(m_deathAnimFrames == m_currentFrame)
    {
        m_finished = true;
    }

    SDL_RenderCopy(m_renderer, m_deathAnim, &m_srs, &m_dst);


}

KillAnimation::~KillAnimation()
{
	// TODO Auto-generated destructor stub
}

} /* namespace jumper */
