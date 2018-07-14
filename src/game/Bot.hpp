/*
 * Bot.hpp
 *
 *  Created on: Dec 9, 2015
 *      Author: twiemann
 */

#ifndef SRC_BOT_HPP_
#define SRC_BOT_HPP_

#include "Actor.hpp"
#include "Level.hpp"
#include "KillAnimation.hpp"
#include "Projectile.hpp"

namespace jumper
{

class Player;

/**
 * @brief An actor that moves in a level according to a predefined movement pattern.
 * 		  The default implementation switches x-direction when it hits an object.
 */
class Bot : public Actor
{
public:

    /***
     * @brief 	Constructs a bot from the given \ref filename
     * 			for the internal \ref renderer
     * @param renderer		A pointer to a SDL renderer struct
     * @param filename		A filename with animation definitions
     */
    Bot(SDL_Renderer* renderer, std::string filename, std::string configFile, std::string name);
    Bot(SDL_Renderer* renderer, std::string filename, std::string configFile, std::string name, Mixer* mixer);
    /**
     * @brief	Constructs a bot from given renderer, texture
     * 			and frame information.
     * @param renderer		A pointer to a SDL renderer struct
     * @param texture		A pointer to valid SDL_Texture struct
     * @param frameWidth	The width of the frames within the texture
     * @param frameHeight	The height of the frames within the texture
     * @param numFrames		The number of frames in the texture
     */
    Bot(SDL_Renderer* renderer, SDL_Texture* texture, int frameWidth, int frameHeight, int numFrames);

    /**
     * @brief ~Bot Destructor
     */
    virtual ~Bot();

    void setRotationPoint(Vector2f pos);

    /// texture for the projectile
    SDL_Texture* m_ProjektilTexture = TextureFactory::instance(m_renderer).getTexture(m_projectilFilename);

    /// firerate of the bot
    float m_firerate;

    /// time gone after the last shot
    float m_lastShotDT;

    /// Set the Player
    void setPlayer(Player *pl){m_player = pl; }

    /**
     * @brief move          Bots are swinging around their rotationpoint. This way they can do sinus or circular flight
     *                      curves.
     * @param level         The level the bot is in
     * @param dt            Amount of time that passed since last frame
     */
    virtual void move(Level& level, float dt);

    /// returns if the bot is a boss
    bool isABoss(){return m_isBoss;}

    ///returns max Health of the bot
    int getMaxHealth(){ return m_maxHealth;}

    int getPoints(){return m_points;}

    /// Projectile, which got shoot
    /**
    * @brief newProjectile  creates a new projectile with the given direction and velocity at the position of the bot and 8 fbs
    * @param direction      the direction of the projectile
    * @param velocity       the velocity of the projectile
    * @return               the created projectile
    */
    Projectile* newProjectile(Vector2f direction, int velocity);

    /**
     * @brief shoot             extends a vector of projectiles with new projectiles with the given direction and velocity if the firerate is reached
     * @param direction         the direction of the projectiles
     * @param velocity          the velocity of the projectiles
     * @param dt                the time past the last call
     * @param allprojectiles    the vector of projectiles to extend
     * @return                  the new vector of projectiles
     */
    virtual std::vector<Actor*> shoot(Vector2f direction, int velocity, float dt, std::vector<Actor*> allprojectiles);

    /**
     * @brief bot_attack    the default method of bots which creates a vector with one projectile in it which moves horizontal to the right with a velocity of 100
     * @param dt            the time past the last call to pass on to the shoot method
     * @return              a vector with the default projectile
     */
    virtual std::vector<Actor*> bot_attack(float dt);

    /**
     * @brief collideWithActor  the default method for colliding with a bot overriding the one of the actor calss decreases the health of the bot according to the type of actor
     * @param other             the actor with which the bot collides
     */
    virtual void collideWithActor(Actor& other);

    /**
    * @brief onDeath    the default methodes for the bots what shall happen when the bots died overriding the one from the actor class
    * @return           a vector of actors which are in this case the deathanimations of the bot
    */
   virtual std::vector<Actor*> onDeath();

    /**
     * @brief setKindOfDeath        to remember how the bot died: sets m_playerProjectilDeath to true if the bot was shot by the player
     * @param isPlayerProjectile    the kind of dead (see above)
     */
    void setKindOfDeath(bool isPlayerProjectile) {m_playerProjectilDeath = isPlayerProjectile;}

    /**
     * @brief getKindOfDeath    gives the was the bot died
     * @return                  returns true when the bot was shot by the player
     */
    bool getKindOfDeath() { return m_playerProjectilDeath;}


private:

    /**
     * @brief circle    calculates cartesian coordinates from given polar coordinates
     * @param radiant   the radiant of the polar coordinate
     * @param radius    the radius od the polar coordinates
     * @param swingX    determines whether the x value of the cartesian coordinate gets set to 0 (false if it should
     *                  be 0)
     * @param swingY    determines whether the y value of the cartesian coordinate gets set to 0 (false if it should
     *                  be 0)
     * @return          a Vector which contains the cartesian coordinatess
     */
    Vector2f circle(float radiant, float radius, bool swingX, bool swingY);


    /**
     * @brief           this function gets called if the bot is moving normally
     * @param dt        the amount of time that passed since the last frame
     */
    Vector2f regularMovement(float dt);

    /**
    * @brief           this function gets called if the bot is not moving normally.
    * @param dt        the amount of time that passed since the last frame
    */
    Vector2f irregularMovement(float dt);

    /**
     * @brief           This function gets called if a collision gets detected. The bots will set his rotationpoint
     *                  according to his corrected position. Additionally his swingsign gets inverted. This ensures
     *                  that the bot won't collide in the next frame.
     * @param c         The collision that gets detected
     * @param dt        The amount of time that passed since the last frame
     */
    void planAvoiding(Collision c, float dt);

protected:

    ///damage of the projectiles
    float m_attackdmg;

    /// true if bot was shot by the player
    bool m_playerProjectilDeath;

    ///Zustand des Bots. Wenn true normaler sinuskurvenflug
    bool m_isMovingRegular=true;

    ///speed of the
    float m_xSpeed=70;

    ///Speed in Y
    float m_ySpeed=0;

    ///Amplitude of sinus
    float m_amplitude=20;

    ///frequency in Hertz
    float m_freq=0.7;

    ///determines whether alpha gets increased or decreased
    float m_swingSign=1;

    ///angle of the circle
    float m_alpha=0;

    ///static or non static
    bool m_isStatic=false;

    ///attacking or not attacking
    bool m_isAttacking;

    ///if this is true the bot will swing on the x axis
    bool m_swingX=false;
    ///if this is true the bot will swing on the y axis
    bool m_swingY=true;

    ///the bot rotates around this Point
    Vector2f m_RotationPoint;


    ///Ist ein Boss
    bool m_isBoss=false;

    ///amplitude beim ausweichen
    float m_currentAmplitude;

    ///Player in the game
    Player* m_player;

    ///points that the player scores on kill
    int m_points;

    ///Schaden bei Zusammenstoß mit Umgebung
    bool m_collisionWithE=true;

    /// Maximales Leben des Bots
    int m_maxHealth;

    /**
     * @brief additionalBehavour    This method gets called if the bot is moving normally. This function serves that
     *                              every bot can act diferrently (i.e. change his amplitude or frequency)
     * @param level                 level the bot is in
     * @param dt                    amount of time that passed by since last frame
     */
    virtual void additionalBehaviour(Level &level, float dt){};

    /**
     * @brief moveRotationPoint     moves the Rotationpoint of the bot
     * @param x                     shift in x direction
     * @param y                     shift in y direction
     */
    void moveRotationPoint(float x, float y);
};
} /* namespace jumper */
#endif /* SRC_BOT_HPP_ */
