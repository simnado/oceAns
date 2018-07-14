/*
 * Actor.hpp
 *
 *  Created on: Dec 9, 2015
 *      Author: twiemann
 */

#ifndef SRC_ACTOR_HPP_
#define SRC_ACTOR_HPP_

#include "AnimatedRenderable.hpp"
#include "PlayerProperty.hpp"
#include "WorldProperty.hpp"
#include "Level.hpp"
#include "Collidable.hpp"
#include "Vector.hpp"

#include <vector>
#include <string>
#include <thread>
#include <chrono>

namespace jumper
{


enum ActorType
{
    ENEMY,
    PLATFORM,
    ITEM,
    ACTOR,
    PLAYERPROJECTILE,
    BOTPROJECTILE,
    PLAYER
};

/**
 * @brief A class the represents a sprite that is moving and implements
 * collision detection.
 */
class Actor : public AnimatedRenderable
{
public:

    /**
     * Constructs an actor from the given \ref filename
     * for the internal \ref renderer
     * @param renderer      A pointer to a SDL renderer struct
     * @param filename      A filename with animation definitions
     */
    Actor(SDL_Renderer* renderer, std::string filename, Mixer* mixer);

    /**
     * @brief Actor         Constructor
     * @param renderer      the rendere which is used to render the actor
     * @param texture       the texture of the actor
     * @param frameWidth    the framewidth of the sprite
     * @param frameHeight   the frmaeheight of the sprite
     * @param numFrames     the number of frames for the animation of the sprite
     * @param mixer         the mixer used to play the sounds of the actor
     */
    Actor(SDL_Renderer* renderer, SDL_Texture* texture, int frameWidth, int frameHeight, int numFrames, Mixer* mixer);

    /**
     * @brief ~Actor    Destructor
     */
    virtual ~Actor();

    /**
     * @brief collideWithActor  the default method for colliding with a actor decreases the health of the actor and playing a sound
     * @param other             the actor with which the bot collides
     */
    virtual void collideWithActor(Actor& other);

    /**
     * @brief move      a prue virtual fuction determining the movement of the actor (different for the subclasses)
     * @param level     the level the actor moves in
     * @param dt        time past the last call
     */
    virtual void move(Level& level, float dt) = 0;
    
    virtual Collision getCollision(Actor& other);
    
    virtual void resolveCollision(Actor& other);

    /**
     * @brief kollisionsschaden     getter for the collisiondamge that the actor causes
     * @return                      the collisiondamge
     */
    int kollisionsschaden(){return m_kollisionsschaden;}
    
    /**
     * @brief render    to render the actor
     */
    virtual void render();

    /**
     * @brief setPhysics   sets the membervariable for the physical proprties equal to the given object
     * @param p             the properties that shall be set for the actor
     */
    void setPhysics(PlayerProperty p);
    
    /**
     * Sets the player's position
     */
    void setPosition(Vector2f pos);

    /***
     * Returns the player's current position
     */
    Vector2f position();

    /**
     * Returns the player's physical properties
     */
    PlayerProperty &physics();

    void start(Level& level);

    void setFocus(bool focus);

    bool hasFocus();

    ActorType type() { return m_type;}

    void setType(ActorType t) { m_type = t;}

    /**
     * @brief onDeath   the function to call if an actor dies so that the killanimation can be created and the respective sound played
     * @return          the killanimation for the actor
     */
    virtual std::vector<Actor*> onDeath();

    /**
     * @brief setHealth    sets the current health to the given value and checking for coditions (e.g. is the actor dead (then calling setDeath() method) or invincible, the max. health is exceeded)
     * @param newHealth    the new health of the actor
     */
    void setHealth(int newHealth);

    /**
     * @brief getHealth getter for the current health of the actor
     * @return          current health
     */
    int getHealth() { return m_currentHealth; }

    /**
     * @brief isDead    to check whether the actor is dead
     * @return          true if the actor is dead
     */
    bool isDead(){ return m_currentHealth<=0;}

    /**
     * @brief getDistanceXToCamera  getter for the distance between the camera and the actor on the x axis
     * @param level                 the level in which the actor is
     * @return                      the distance
     */
    float getDistanceXToCamera(Level& level){return m_physicalProps.position().x()-(level.m_camera.position().x());}

    /**
     * @brief getDistanceYToCamera  getter for the distance between the camera and the actor on the y axis
     * @param level                 the level in which the actor is
     * @return                      the distance
     */
    float getDistanceYToCamera(Level& level){return m_physicalProps.position().y()-(level.m_camera.position().y());}

    /**
     * @brief setDeath  sets the actors life to zero and the membervariable representing the dead-status on true
     */
    void setDeath() { m_death = true; m_currentHealth = 0; }

    /**
     * @brief calculates des hitbox-width, if the sprite of the actor is to large
     *
     * @return integer-value of the hitbox-width
     */
    int hitboxW();

    /**
     * @brief Returns the recalculated hitbox-height
     *
     * @return integer-value of the hitbox-height
     */
    int hitboxH();

    // Wenn der arme Spieler verwundet wird, darf er netterweise Ausruhen.
    void setWounded(int beginn) {  m_woundCounter = beginn;}

    void decWounded() { m_woundCounter--;}
    // true wenn kürzlich verwundet
    int getWounded() {return  m_woundCounter;}

    /// sets invincible state at player
    void setInvincible(bool invincible){ m_invincible = invincible; }

    /// returns invincible state at player
    bool getInvincible(){return m_invincible; }

    /**
     * @brief setStun   stunnes the actor by setting the velocity to zero
     */
    void setStun();

    /**
     * @brief delStun   dissolves the stun by restoring the velocity of the actor
     */
    void delStun();

    /**
     * @brief getStun   to see if the actor is stunned
     * @return          true if the actor is stunned
     */
    bool getStun() {return m_isStuned;}

    /**
     * @brief setPowerShoot     activates the power shot of the actor and resetting the timer
     * @param newPower          true if the actor shall have from now on a stronger shot
     */
    void setPowerShoot(bool newPower) {m_powerShoot = newPower; m_powerShootTimer = 0; }

    /**
     * @brief setPowerShoot     activates the power shot of the actor and setting the timer to the given value
     * @param newPower          true if the actor shall have from now on a stronger shot
     * @param timi              the value from which the timer shall count
     */
    void setPowerShoot(bool newPower, float timi){m_powerShoot = newPower; m_powerShootTimer = timi;}

    /**
     * @brief getPowerShoot     to see if the actor has increased projectiledamage
     * @return                  true if the actor has increased projectiledamage
     */
    bool getPowerShoot() { return m_powerShoot; }

    /**
     * @brief getPowerShootTimer    to get the time of how long the actor had a stronger shot
     * @return                      time of the active powershot
     */
    float getPowerShootTimer(){return m_powerShootTimer;}
    

protected:

    /// ture if Actors porjectildamage is increase
    bool m_powerShoot = false;

    /// variable to increase if the actor has a higher porjectildamage until the effect is gone
    float m_powerShootTimer = 0;

    /// the velocity an actor had before it got stunned
    Vector2f m_stdVelocity;

    /// variable to increase when the actor is stunned until the stun shall be deleted
    float m_stunTimer = 0;

    /// indecating if the actor is stunned (per deafault he isn't)
    bool m_isStuned = false;

    int m_woundCounter = 0;

    /// The physical properties of the player
    PlayerProperty      m_physicalProps;

    bool                m_focus;

    /// the type of actor
    ActorType           m_type;

    /// the current health of the actor
    int 		m_currentHealth = 100;

    /// damage that this actor does to another actor
    int                 m_kollisionsschaden;

    bool m_death;

    /// true if player is invincible
    bool m_invincible = false;

    std::string m_projectilFilename = "..\\res\\flame.png";

    std::string m_projectilSprite = "..\\res\\Data\\Projektile\\flame.spr";

    int m_hitbox_h;
};

} /* namespace jumper */

#endif /* SRC_ACTOR_HPP_ */
