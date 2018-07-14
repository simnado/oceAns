/*
 * Actor.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: twiemann
 */

#include <SDL2/SDL.h>
#include "KillAnimation.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include "Actor.hpp"

namespace jumper
{

Actor::Actor(SDL_Renderer* renderer, std::string filename, Mixer* mixer)
    : AnimatedRenderable(renderer, filename)
{
    m_mixer = mixer;
    m_focus = false;
    //m_startTicks = 0;
    if(m_numFrames == 0)
         m_numFrames = 1;
    m_type = ACTOR;
}

Actor::Actor(SDL_Renderer* renderer, SDL_Texture* texture, int frameWidth, int frameHeight, int numFrames, Mixer* mixer)
    : AnimatedRenderable(renderer, texture, frameWidth, frameHeight, numFrames)
{
    m_mixer = mixer;
    m_focus = false;
    //m_startTicks = 0;
    m_type = ACTOR;
}

void Actor::setPhysics(PlayerProperty p)
{
    m_physicalProps = p;
}

Actor::~Actor()
{

}


void Actor::render()
{
    SDL_Rect target;

    target.x = floor(m_physicalProps.position().x()) - m_camera.x();
    target.y = floor(m_physicalProps.position().y()) - m_camera.y();
    target.w = m_frameWidth;
    target.h = m_frameHeight;

    // Do not render if actor is outside frustrum
    if(target.x + target.h > 0 && target.x + target.h < m_camera.w())
    {
        SDL_RendererFlip flip;
        if(m_physicalProps.velocity().x() > 0)
        {
            flip = SDL_FLIP_HORIZONTAL;

        }
        else
        {
            flip = SDL_FLIP_NONE;
        }

        // Render current animation frame
        SDL_RenderCopyEx( getRenderer(), m_texture, &m_sourceRect, &target, 0, NULL, flip);
    }

}

void Actor::setPosition(Vector2f pos)
{
    m_physicalProps.position() = pos;
}


void Actor::setHealth(int newHealth)
{
    /// Falls Player invincible blockiere Schaden aber erlaube Healtherhöhung
    if(!(type() == PLAYER && getInvincible()))
    {
        if (newHealth < 0)
        {
            m_currentHealth = 0;
        }
        else
        {
            m_currentHealth = newHealth;
        }

        if(getHealth() == 0)
        {
            setDeath();
        }
    }
    else if(newHealth > m_currentHealth)
    {
        m_currentHealth = newHealth;
    }

    /// Spieler darf nicht mehr als 100 Health haben
    if(m_currentHealth > 100 && type() == PLAYER)
    {
        m_currentHealth = 100;
    }
}

PlayerProperty& Actor::physics()
{
    return m_physicalProps;
}

Vector2f Actor:: position()
{
    return m_physicalProps.position();
}


void Actor::setStun()
{
    m_isStuned = true;
    m_stdVelocity = physics().velocity();
    physics().setVelocity(0);
}

void Actor::delStun()
{
    m_isStuned = false;
    physics().setVelocity(m_stdVelocity);
    m_stunTimer = 0;
}


void Actor::collideWithActor(Actor& other) {
    setHealth(m_currentHealth-other.kollisionsschaden());
    m_mixer->play(this, "hit");
}

void Actor::resolveCollision(Actor& other)
{
    SDL_Rect myRect;
    myRect.x = position().x();
    myRect.y = position().y();
    myRect.w = w();
    myRect.h = h();

    SDL_Rect otherRect;
    otherRect.x = other.position().x();
    otherRect.y = other.position().y();
    otherRect.w = other.w();
    otherRect.h = other.h();

    SDL_Rect intersection;
    SDL_IntersectRect(&myRect, &otherRect, &intersection);

    if(intersection.h > 0 && intersection.w > 0)
    {
        Vector2f tmp = position();
        Vector2f tmp_v = m_physicalProps.velocity();
        tmp_v.setY(0);

        if(m_physicalProps.velocity().y() > 0)
        {
            tmp.setY(position().y() - intersection.h);
        }
        else
        {
            tmp.setY(position().y() + intersection.h);
        }
        setPosition(tmp);
        m_physicalProps.setVelocity(tmp_v);
    }
}

Collision Actor::getCollision(Actor& other)
{
    Collision c;

    static float prozent_hitbox = 0.75;

    // width and height of the hitbox
    float hit_w = (float)w() * prozent_hitbox;
    float hit_h = (float)h() * prozent_hitbox;

    // coordinates of the hitbox
    Vector2f v = m_physicalProps.velocity();
    float hitboxX = (position().x() + (float)w() / 2.0) - (hit_w / 2.0);
    float hitboxY = ((float)position().y() + (float)h() / 2.0) - (hit_h / 2.0);

    SDL_Rect myRect;
    myRect.x = hitboxX;
    myRect.y = hitboxY;
    myRect.w = hit_w;
    myRect.h = hit_h;

    // width and height of the hitbox of the other actor
    float hit_o_w = (float)other.w() * prozent_hitbox;
    float hit_o_h = (float)other.h() * prozent_hitbox;

    // coordinates of the hitbox of the other actor
    float otherHitboxX = ((float)other.position().x() + (float)other.w() / 2.0) - (hit_o_w / 2.0);
    float otherHitboxY = ((float)other.position().y() + (float)other.h() / 2.0) - (hit_o_h / 2.0);

    SDL_Rect otherRect;
    otherRect.x = otherHitboxX;
    otherRect.y = otherHitboxY;
    otherRect.w = hit_o_w;
    otherRect.h = hit_o_h;

    // Check for collision
    SDL_Rect intersection;
    if(SDL_IntersectRect(&myRect, &otherRect, &intersection))
    {
        if(fabs(intersection.w) < otherRect.w && intersection.h > 0)
        {
            if(v.y() > 0)
            {
                if(intersection.h < otherRect.h / 2)
                {
                    c.setType(DOWN);
                }
                else
                {
                    c.setType(BOOM);
                }
            }
            else
            {
                if(intersection.h < otherRect.h / 2)
                {
                    c.setType(UP);
                }
                else
                {
                    c.setType(BOOM);
                }
            }
        }
    }

    return c;
}

void jumper::Actor::setFocus(bool focus)
{
    m_focus = focus;
}

bool jumper::Actor::hasFocus()
{
    return m_focus;
}

std::vector<Actor*> Actor::onDeath()
{
    std::vector<Actor*> animation;
    KillAnimation* k = new KillAnimation(this,m_explodeFile, m_mixer);
    animation.push_back(k);

    m_mixer->play(this,"death");
    return animation;
}

int jumper::Actor::hitboxW()
{
    // min- and max-values of the hitbox
    static float max = 30;
    static float min = 19;

    // resize hitbox, if actor-size is to large
    if(frameHeight() > max || frameWidth() > max)
    {
        // get bigger value
        if(frameWidth() > frameHeight())
        {
            // set hitbox-width to max and rescale hitbox-height
            m_hitbox_h = (float)frameHeight() / ((float)frameWidth() / max);
            if(m_hitbox_h < min)
                m_hitbox_h = min;
            return max;
        }
        else
        {
            // set hitbox-height to max and rescale hitbox-width
            m_hitbox_h = max;
            if((frameWidth() / (frameHeight() / max)) < min)
                return min;
            return frameWidth() / (frameHeight() / max);
        }
    }
    else
        m_hitbox_h = frameHeight();
    return frameWidth();
}

int jumper::Actor::hitboxH()
{
    return m_hitbox_h;
}


} /* namespace jumper */

