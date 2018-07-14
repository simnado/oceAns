//
// Created by isaak on 26.11.15.
//

#ifndef JUMPER_PLAYER_H
#define JUMPER_PLAYER_H

#include <fstream>
#include <iostream>
#include <string>

#include <SDL2/SDL.h>

#include "Actor.hpp"
#include "Projectile.hpp"
#include "Vector.hpp"
#include "Game.hpp"

namespace jumper
{

/**
 * @brief   A class to represent an animated sprite controlled
 *          by the user.
 */
class Player : public Actor
{
public:

    /**
     * Constructor.
     *
     * @param renderer      A pointer to a valid SDL_Renderer struct that is
     *                      used for rendering
     * @param filename      The name of a file with sprite definitions
     */
    Player(SDL_Renderer* renderer, std::string filename, Mixer* mixer);

    /**
     * Constructs a player.
     *
     * @param renderer      A pointer to a valid SDL_Renderer struct that is
     *                      used for rendering
     * @param texture       A pointer to a valid SDL_Texture struct
     * @param frameWidth    Frame width of the animation frames
     * @param frameHeight   Frame height of the animation frames
     * @param numFrames     Number of frames
     */
    Player(SDL_Renderer* renderer, SDL_Texture* texture, int frameWidth, int frameHeight, int numFrames, Mixer* mixer);

    /**
     * Moves the player in the given level.
     *
     * @param level         A level object
     */
    virtual void move(Level& level, float dt);

    /// Prints the player's position to the given stream
    template<typename T>
    friend std::ostream& operator<< (std::ostream& stream, const Vector2<T> & vec);

    /// Creates a projectile and returns it
    Projectile* shoot();

    /// sets evolved state
    void setEvolved(bool evolved){ m_evolved = evolved; }

    /// returns evolved state
    bool getEvolved(){return m_evolved;}

    /// sets lightning state
    void setLightning(bool light){ m_lightning = light; }

    /// returns lightning state
    bool getLightning(){return m_lightning; }

    void setGame(Game* game) { m_game = game; }

    void update(const Uint8* &currentKeyStates, float dt);


    void set_InvincibleSecs(float sec){m_invincibleSecs = sec;}

    float getInvincibleSecs(){return m_invincibleSecs;}

    bool isDead(){ return m_currentHealth<=0 || m_death;}


private:

    /// true if player collected lightning item
    bool m_lightning = false;

    /// true if player ist evolved
    bool m_evolved = false;

    void setDeath();

    ///represents health from player
    int m_currentHealth = 100;

    /// blocks constant fire
    float m_shootTimer;

    Game* m_game;

    float m_invincibleSecs = 0;



};

}

#endif //JUMPER_PLAYER_H
