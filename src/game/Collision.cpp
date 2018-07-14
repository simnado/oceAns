/*
 * Collision.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: twiemann
 */

#include "Collision.hpp"

namespace jumper
{

Collision::Collision()
    : m_delta(0, 0), m_damage(0), m_type(NONE), m_tiletype(BLOCK)
{

}

Collision::Collision(const Vector2i& delta)
    : m_delta(delta), m_damage(0), m_type(NONE), m_tiletype(BLOCK)
{
}

Collision::Collision(const Vector2i& delta, float damage)
    : m_delta(delta), m_damage(damage), m_type(NONE), m_tiletype(BLOCK)
{
}

Collision::~Collision()
{
	// TODO Auto-generated destructor stub
}

Vector2i& Collision::delta()
{
	return m_delta;
}

float Collision::damage()
{
	return m_damage;
}

bool Collision::collides()
{
	Vector2i zero(0, 0);
	return m_delta == zero;
}

} /* namespace jumper */
