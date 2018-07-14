//
// Created by isaak on 26.11.15.
//

#include "WorldProperty.hpp"
namespace jumper
{

WorldProperty::WorldProperty()
	: m_gravity(0,0), m_damping(0.9,1.0)
{

}

WorldProperty::WorldProperty(const Vector2f &gravity, const Vector2f &damping)
 	 : m_gravity(gravity),  m_damping(damping)
{

}

const Vector2f & WorldProperty::gravity() const
{
	return m_gravity;
}

void WorldProperty::setGravity(const Vector2f &gravity)
{
	m_gravity = gravity;
}

const Vector2f& jumper::WorldProperty::damping() const
{
	return m_damping;
}

void jumper::WorldProperty::setDamping(const Vector2f& damping)
{
	m_damping = damping;
}

}


