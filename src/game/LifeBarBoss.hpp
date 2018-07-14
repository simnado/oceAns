/*
 * LifeBoard.hpp
 *
 *  Created on: Feb 23, 2016
 *      Author: doschulze, vtecker
 */

#ifndef SRC_LifeBarBoss_HPP_
#define SRC_LifeBarBoss_HPP_

#include "StaticRenderable.hpp"
#include "TextureFactory.hpp"
#include "Bot.hpp"

namespace jumper
{

/**
 * @brief A class to display life using a digit map for rendering
 */
class LifeBarBoss : public StaticRenderable
{
public:


	/**
	 * Constructs a LifeBoard for the given renderer
	 *
	 * @param renderer			A valid SDL_Renderer structure
	 * @param texture			A texture that contains the 10 digits (from 0 to 9 in
	 * 							that order)
	 * @param digitWidth		The width of a digit within the texture
	 * @param digitHeight		The height of a digit within the texture
	 */
	LifeBarBoss(SDL_Renderer* renderer, SDL_Texture* texture, Bot* boss);

	/**
	 * Constructs an empty LifeBoard for the renderer
	 *
	 * @param renderer			A valid SDL_Renderer structure
	 */
	LifeBarBoss(SDL_Renderer* renderer, Bot* boss);

	/// Renders the LifeBoard
	virtual void render();

	/// Destructor
	virtual ~LifeBarBoss();

private:

    Bot* m_boss;


};

} /* namespace jumper */

#endif /* SRC_LIFEBOARD_HPP_ */
