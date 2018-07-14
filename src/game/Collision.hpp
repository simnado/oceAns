/*
 * Collision.hpp
 *
 *  Created on: Dec 9, 2015
 *      Author: twiemann
 */

#ifndef SRC_COLLISION_HPP_
#define SRC_COLLISION_HPP_

#include "Vector.hpp"

namespace jumper
{

enum CollisionType
{
    UPPERLEFT,
    UP,
    UPPERRIGHT,
    LEFT,
    RIGHT,
    LOWERLEFT,
    DOWN,
    LOWERRIGHT,
	LEVEL,
    BOOM,
    NONE
};

enum TileType
{
    BLOCK,                      //######
                                //######
                                //######

    SchraegeAbwaerts,           //##
                                //####
                                //######

    SchraegeAufwaerts,          //    ##
                                //  ####
                                //######

    SchraegeUmgekehrtAbwaerts,  //######
                                //####
                                //##

    SchraegeUmgekehrtAufwaerts  //######
                                //  ####
                                //    ##
};

/**
 * @brief 	Class to represent collision between two objects. If two objects
 * 			intersect, the width and height of the intersection is represented
 * 			as delta() offset. Optionally, a damage value can be encoded.
 */
class Collision
{
public:

	/**
	 * Creates an empty collision.
	 */
	Collision();

	/**
	 * Creates a collision object with offset \ref delta and
	 * no damage.
	 */
	Collision(const Vector2i& delta);

	/**
	 * Creates a collision representation with offset \ref delta
	 * and damage \ref damage
	 */
	Collision(const Vector2i& delta, float damage);

	/// Destructor
	virtual ~Collision();

	/***
	 * Returns the intersection width and height
	 */
	Vector2i& delta();

	/**
	 * Returns a damage value for the collision.
	 */
	float damage();

	/***
	 * Returns if a collision was detected.
	 */
	bool collides();

	/// Sets the type of the collision
	void setType(CollisionType t) {m_type = t;}

	/// Returns the type of the collision
	CollisionType type() {return m_type;}

    /// Sets the type of the tile
    void setTileType(TileType t) {m_tiletype = t;}

    /// Returns the type of the tile
    TileType tiletype() {return m_tiletype;}

private:

	/// Intersection between to objects in pixel units
	Vector2i 	m_delta;

	/// Damage estimation
	float 		m_damage;

	/// Type of collision
	CollisionType m_type;

    /// Type of the tile
    TileType m_tiletype;
};

} /* namespace jumper */

#endif /* SRC_COLLISION_HPP_ */
