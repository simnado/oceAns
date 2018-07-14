/*
 * Collidable.hpp
 *
 *  Created on: Feb 21, 2016
 *      Author: twiemann
 */

#ifndef SRC_GAME_COLLIDABLE_HPP_
#define SRC_GAME_COLLIDABLE_HPP_

#include "Level.hpp"
#include "Collision.hpp"

namespace jumper
{


class Collidable
{
public:
	Collidable();
	virtual ~Collidable();

	virtual Collision getCollision(Collidable& other) = 0;

protected:

};

} /* namespace jumper */

#endif /* SRC_GAME_COLLIDABLE_HPP_ */
