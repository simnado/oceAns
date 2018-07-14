/*
 * Projectile.hpp
 *
 *  Created on: Feb 23, 2016
 *      Author: doschulze, vtecker
 */

#ifndef SRC_PROJECTILE_HPP_
#define SRC_PROJECTILE_HPP_

#include "Actor.hpp"
#include "Level.hpp"
#include "KillAnimation.hpp"


namespace jumper
{

/**
 * @brief An actor that moves in a level according to a predefined movement pattern.
 * 		  The default implementation switches x-direction when it hits an object.
 */
class Projectile : public Actor
{
public:

	/**
	 * @brief	Constructs a Projectile from given renderer, texture
	 * 			and frame information.
	 * @param renderer		A pointer to a SDL renderer struct
	 * @param texture		A pointer to valid SDL_Texture struct
	 * @param frameWidth	The width of the frames within the texture
	 * @param frameHeight	The height of the frames within the texture
	 * @param numFrames		The number of frames in the texture
	 * @param friendly 		Is the Projectile friendly?
	 * @param direction     Gives the direction of the projectile
	 * @param velocity      The velocity of the projectile
	 */

    Projectile(SDL_Renderer* renderer, std::string filename, bool friendly, Vector2f direction, int velocity, int kollisionsschaden, Mixer* mixer);

	virtual ~Projectile();

	/// Moves the Projectile in the given \ref level
	virtual void move(Level& level, float dt);

    virtual std::vector<Actor*> onDeath();

    Vector2f m_direction;

    int m_velocity;

private:

	/// True, if the Projectile is friendly to a player. False, if not.
	bool m_friendly;
};

} /* namespace jumper */

#endif /* SRC_PROJECTILE_HPP_ */
