//
// Created by isaak on 26.11.15.
//

#include "PlayerProperty.hpp"
namespace jumper
{

PlayerProperty::PlayerProperty() :
		m_pos(0.0, 0.0),
		m_vel(0.0, 0.0),
		m_jumpForce(0, -440),
		m_moveForce(800, 0),
		m_maxVelJmp(122.0),
		m_maxVelRun(122.0),
		m_maxVelFall(122.0),
		m_jumpHeight(122)
{
		m_stdMaxRun = 122;
		m_stdMaxFall = 122;
		m_stdMaxJump = 122;

}

PlayerProperty::PlayerProperty(
		const Vector2f &position,
		const Vector2f &moveForce,
		const Vector2f &velocity,
		double maxVel,
		double maxJumpVel,
		double maxJumpHeight,
	    double maxFallVel
		)
: m_pos(position), m_vel(velocity), m_jumpForce(0.0, 0.0), m_moveForce(moveForce),
  m_maxVelJmp(maxJumpVel), m_maxVelRun(maxVel), m_maxVelFall(maxFallVel), m_jumpHeight(maxJumpHeight)
{
	m_stdMaxRun = maxVel;
	m_stdMaxFall = maxFallVel;
	m_stdMaxJump = maxJumpVel;
}

Vector2f& PlayerProperty::position()
{
	return m_pos;
}

void PlayerProperty::setPosition(const Vector2f &pos)
{
	m_pos = pos;
}


Vector2f& PlayerProperty::velocity()
{
	return m_vel;
}

void PlayerProperty::setVelocity(const Vector2f &vel)
{
	m_vel = vel;
}

const Vector2f & PlayerProperty::jumpForce() const
{
	return m_jumpForce;
}

void PlayerProperty::setJumpForce(const Vector2f &jump)
{
	m_jumpForce = jump;
}

const Vector2f & PlayerProperty::moveForce() const
{
	return m_moveForce;
}

void PlayerProperty::setMoveForce(const Vector2f &move)
{
	m_moveForce = move;
}


double PlayerProperty::maxJumpVelocity() const
{
	return m_maxVelJmp;
}

void PlayerProperty::setMaxJumpVelocity(double maxVelJmp)
{
	m_maxVelJmp = maxVelJmp;
}

double PlayerProperty::maxRunVelocity() const
{
	return m_maxVelRun;
}

void PlayerProperty::setMaxRunVelocity(double maxVelRun)
{
	m_maxVelRun = maxVelRun;
}

double PlayerProperty::maxFallVelocity() const
{
	return m_maxVelFall;
}

void PlayerProperty::setMaxFallVelocity(double maxVelFall)
{
	 m_maxVelFall = maxVelFall;
}

double PlayerProperty::maxJumpHeight() const
{
	return m_jumpHeight;
}

void PlayerProperty::setMaxJumpHeight(double jumpHeight)
{
	m_jumpHeight = jumpHeight;
}

}
