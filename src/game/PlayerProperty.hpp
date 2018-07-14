//
// Created by isaak on 26.11.15.
//

#ifndef PLAYER_PROPERTIES_H
#define PLAYER_PROPERTIES_H

#include "Vector.hpp"

namespace jumper
{

/**
 * @brief		A class to represent the physical properties of
 * 				a player.
 */
class PlayerProperty
{
public:

	/**
	 * Constructor.
	 */
    PlayerProperty();

    /**
     * Constructor
     *
     * @param position		Initial position
     * @param moveForce		Initial move force
     * @param velocity		Initial velocity
     * @param maxVel		Maximum velocity
     * @param maxJumpVel	Maximum jump velocity
     * @param maxJumpHeight Maximum jump height
     * @param maxRunVel		Maximum run velocity
     * @param maxFallVel	Maximum fall velocity
     */
    PlayerProperty(
    		const Vector2f &position,
			const Vector2f &moveForce,
			const Vector2f &velocity,
			double maxVel,
			double maxJumpVel,
			double maxJumpHeight,
		    double maxFallVel
			);

    /**
     * Gets the force for jumping
     */
    const Vector2f& jumpForce() const;

    /**
     * Sets the force for jumping
     */
    void setJumpForce(const Vector2f &jump);

    /**
     * Returns the move force to push the player
     */
    const Vector2f& moveForce() const;

    /**
     * Sets the move force to push the player
     */
    void setMoveForce(const Vector2f &move);

    /**
     * Returns the maximum velocity when jumping
     */
    double maxJumpVelocity() const;

    /**
     * Sets the maximum velocity when jumping
     */
    void setMaxJumpVelocity(double maxVelJmp);

    /**
     * Returns the maximum running velocity
     */
    double maxRunVelocity() const;

    /**
     * Sets the maximum running velocity
     */
    void setMaxRunVelocity(double maxVelRun);

    /**
     * Returns the maximum falling velocity
     */
    double maxFallVelocity() const;

    /**
     * Sets the maximum falling velocity
     */
    void setMaxFallVelocity(double maxVelFall);

    /**
     * Returns the maximum jump height
     */
    double maxJumpHeight() const;

    /**
     * Sets the maximum jump height.
     *
     * @param jumpHeight
     */
    void setMaxJumpHeight(double jumpHeight);


    /**
     * Sets the player position
     */
    void setPosition(const Vector2f &m_pos);

    /**
     * Sets the player's velocity
     */
    void setVelocity(const Vector2f &m_vel);

    /**
     * Returns the player's velocity
     */
    Vector2f &velocity();

    /**
     * Returns the player's position
     */
    Vector2f &position();

    double m_stdMaxRun;
    double m_stdMaxFall;
    double m_stdMaxJump;    

private:

    /// Position
    Vector2f m_pos;

    /// Velocity
    Vector2f m_vel;

    /// Jump force
    Vector2f m_jumpForce;

    /// Force that pushes the player
    Vector2f m_moveForce;

    /// Maximum jump velocity
    double m_maxVelJmp;

    /// Maximum run velocity
    double m_maxVelRun;

    /// Maxumin fall velocity
    double m_maxVelFall;

    /// Maximum jump height
    double m_jumpHeight;

};

}

#endif //JUMPER_PHYSICPLAYER_H
