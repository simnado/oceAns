//
// Created by isaak on 26.11.15.
//


#include "Player.hpp"

#include <iostream>
using std::cout;
using std::endl;

namespace jumper
{

Player::Player(SDL_Renderer *renderer, std::string filename, Mixer* mixer)
    : Actor(renderer, filename, mixer)
{
    setHealth(20);
    m_death = false;
    m_game = NULL;
    m_shootTimer = 0;
}

Player::Player(SDL_Renderer* renderer, SDL_Texture* texture, int frameWidth, int frameHeight, int numFrames, Mixer* mixer)
    : Actor(renderer, texture, frameWidth, frameHeight, numFrames, mixer)
{
    setHealth(100);
    m_death = false;
    m_game = NULL;
    m_shootTimer = 0;
}

void Player::move(Level& level, float dt)
{
    if(m_isStuned)
    {
        m_stunTimer += dt;
        if(m_stunTimer >= 3)
            this->delStun();
    }
    else
    {
        nextFrame();
        if(dt > 0)
        {
            Vector2f d_move;

            d_move = (physics().moveForce() * dt);

            // Update velocity
            physics().setVelocity(physics().velocity() + d_move);

            // Damp velocity according to extrinsic level damping
            physics().setVelocity(physics().velocity() * level.physics().damping());

            // Clamp velocities
            if(physics().velocity().x() > physics().maxRunVelocity() * dt)
            {
                physics().setVelocity(Vector2f(physics().maxRunVelocity() * dt,
                                               physics().velocity().y()));
            }

            if(physics().velocity().x() < -physics().maxRunVelocity() * dt)
            {
                physics().setVelocity(Vector2f(-physics().maxRunVelocity() * dt,
                                               physics().velocity().y()));
            }

            if(physics().velocity().y() > physics().maxFallVelocity() * dt)
            {
                physics().setVelocity(
                            Vector2f(physics().velocity().x(), physics().maxFallVelocity() * dt));
            }

            if(physics().velocity().y() < -physics().maxJumpVelocity() * dt)
            {
                physics().setVelocity(
                            Vector2f(physics().velocity().x(), -physics().maxJumpVelocity() * dt));
            }
		
            // Set new player position
            physics().setPosition(physics().position() + physics().velocity());

            //Reset Player Position, damit kein Auto-Move
            physics().setVelocity(Vector2f(0, 0));

        }
    }

        bool collide = false;

        Collision c = level.resolveCollision(this);

        // if there is a collision between player and level
        if(c.type() != NONE)
        {
            collide = true;
        }

        c = m_camera.resolveCameraCollision(this);

        // player dies if it is squeezed between camera and level
        if(c.type() == LEFT)
        {
                if(collide)
                {                   
                    setDeath();
                }
            }
        }




void Player::update(const Uint8* &currentKeyStates, float dt)
{
    //Set bool Variables
    bool shoot = false;

    if(m_powerShoot)
    {
        m_powerShootTimer += dt;
        if(m_powerShootTimer >= 10)
            this->setPowerShoot(false);
    }

    // Reset forces and jump flags
    physics().setMoveForce(Vector2f(0.0, 0.0));

    /// Set shoot true if shootTimer > 0.5
    /// blocks constant fire
    m_shootTimer += dt;
    if( currentKeyStates[ SDL_SCANCODE_SPACE ] )
    {
        if (m_shootTimer > 0.5)
        {
            shoot = true;
            m_shootTimer = 0;
        }
    }

    if ( currentKeyStates[ SDL_SCANCODE_UP ])
    {
        physics().setMoveForce(physics().moveForce() + Vector2f(0.0, -200.0));
    }

    if ( currentKeyStates[ SDL_SCANCODE_DOWN ] )
    {
        physics().setMoveForce(physics().moveForce()  + Vector2f(0.0, 200.0));
    }
    if ( currentKeyStates[ SDL_SCANCODE_LEFT ] )
    {
        physics().setMoveForce(physics().moveForce() + Vector2f(-800.0, 0.0));
    }
    if ( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
    {
        physics().setMoveForce(physics().moveForce()  + Vector2f(800.0, 0.0));
    }

    /// If player collected lighnting item,
    /// move physics are multiplicated by 1.5
    if(getLightning())
    {
        physics().setMaxRunVelocity(physics().m_stdMaxRun*1.5);
        physics().setMaxFallVelocity(physics().m_stdMaxFall*1.5);
        physics().setMaxJumpVelocity(physics().m_stdMaxJump*1.5);

    }else{
        physics().setMaxRunVelocity(physics().m_stdMaxRun);
        physics().setMaxFallVelocity(physics().m_stdMaxFall);
        physics().setMaxJumpVelocity(physics().m_stdMaxJump);

    }

    if ( currentKeyStates[ SDL_SCANCODE_ESCAPE ] )
        m_game->pause();

    /// Fire projectile
    if (shoot)
    {
        /// Fire projectile and add it to actors
        Projectile* p = this->shoot();
        p->setType(PLAYERPROJECTILE);
        /// Füge das Projektil zu den Actorn hinzu.
        if (m_game)
            m_game->addActor(p);
    }

}

Projectile* Player::shoot()
{
    //In den nächsten Zeilen folgt der Shoot Aufruf für den Player
    Projectile* projectile;

    // Unterschiedliche Schusstypen für Karpador und Garados mit und ohne Powershoot
    if(getEvolved() && m_powerShoot)
    {
        //Erzeuge ein Projectile. Renderer, Textur, Breite, Höhe, Animationsstufen, Friendly?, Richtung, Geschwindigkeit
        projectile = new Projectile(m_renderer, "..\\res\\Data\\Projektile\\Ball_gold_gross.spr", true, Vector2f(500,0), 400, 60, m_mixer);
        //Setze das Projektil an die Position des Players
        projectile->setPosition(physics().position()+Vector2f(60,45));
    }
    else if(getEvolved() && !m_powerShoot)
    {
        //Erzeuge ein Projectile. Renderer, Textur, Breite, Höhe, Animationsstufen, Friendly?, Richtung, Geschwindigkeit, Kollisionsschaden
        projectile = new Projectile(m_renderer, "..\\res\\Data\\Projektile\\Ball_blau_gross.spr", true, Vector2f(500,0), 400, 50, m_mixer);
        projectile->setPosition(physics().position()+Vector2f(60,45));

    }
    else if(!getEvolved() && m_powerShoot)
    {
        //Erzeuge ein Projectile. Renderer, Textur, Breite, Höhe, Animationsstufen, Friendly?, Richtung, Geschwindigkeit, Kollisionsschaden
        projectile = new Projectile(m_renderer, "..\\res\\Data\\Projektile\\Ball_gold_gross.spr", true, Vector2f(500,0), 400, 60, m_mixer);
        projectile->setPosition(physics().position()+Vector2f(60,35));

    }
    else
    {
        //Erzeuge ein Projectile. Renderer, Textur, Breite, Höhe, Animationsstufen, Friendly?, Richtung, Geschwindigkeit, Kollisionsschaden
        projectile = new Projectile(m_renderer, "..\\res\\Data\\Projektile\\Ball_rot_gross.spr", true, Vector2f(500,0), 200, 25, m_mixer);
        projectile->setPosition(physics().position()+Vector2f(60,35));

    }
    
    //Setze das Projektil an die Position des Players
//    projectile->setPosition(physics().position()+Vector2f(60,35));


    //Das Projektil des Players soll mit 8FPS animiert werden.
    projectile->setFPS(8);

    // Abspielen des entsprechenden Sounds
    m_mixer->play(this, "shoot");
    return projectile;
}

void Player::setDeath()
{
    m_death = true;
}

}/* namespace jumper */
