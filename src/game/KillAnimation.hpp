/*
 * KillAnimation.hpp
 *
 *  Created on: Feb 21, 2016
 *      Author: twiemann
 */

#ifndef SRC_GAME_KILLANIMATION_HPP_
#define SRC_GAME_KILLANIMATION_HPP_

#include "Item.hpp"
#include <fstream>

namespace jumper
{

class KillAnimation : public Item
{
public:
    KillAnimation(Actor* actor, Mixer* mixer);
    KillAnimation(Actor* actor, std::string filename, Mixer* mixer);
    bool isFinished(){return m_finished;}
	virtual ~KillAnimation();

	virtual void render();

private:
    int         m_framePosX = 0;
    int         m_deathAnimFrames;
    int         m_currentFrame = 0;
    bool        m_finished = false;
    SDL_Rect    m_dst;
    SDL_Rect    m_srs;
    SDL_Texture* m_deathAnim;

};

} /* namespace jumper */

#endif /* SRC_GAME_KILLANIMATION_HPP_ */
