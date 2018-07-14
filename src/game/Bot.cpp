/*
 * Bot.cpp
 *
 *  Created on: 4. April 2016
 *      Author: Bot-Team
 */

#include "Bot.hpp"
#include "Bot_stat_Boot.hpp"
#include "Bot_Mine.hpp"
#include "Bot_Seeigel.hpp"
#include "Bot_mov_Boot.hpp"
#include "Bot_Kugelfisch.hpp"
#include "Bot_Seepferdchen.hpp"
#include "Bot_Schwertfisch.hpp"
#include "Bot_Hai.hpp"
#include "Bot_Qualle.hpp"
#include "Bot_Schlange.hpp"
#include "Bot_Aal.hpp"
#include "Collision.hpp"
#include "Player.hpp"
#include "Bot_Meerjungfrau.hpp"
#include "Bot_Muschel.hpp"

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include <typeinfo>
#include <time.h>
#include <unistd.h>
#include <iostream>

#define PI          3.14

using namespace std;

namespace jumper
{

template <typename T> int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}


Bot::Bot(SDL_Renderer* renderer, std::string configFile, std::string filename, std::string name, Mixer* mixer)
    : Actor(renderer, filename, mixer)
{
    std::size_t found = configFile.find_last_of("/\\");
    string path = configFile.substr(0,found);

    // Benutze Boost zum Einlesen des XML-Tree
    using boost::property_tree::ptree;
    ptree pt;
    read_xml(configFile, pt);
    // Zufallszahl für Default-Bots. Standard 1
    float random = 1;
    //Falls Bot ein DefaultBot
    if (name == "default")
    {
        // Setze Standard Zahl zwischen 0 und 0.5
        random = 1 + ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) / 2);
    }

    BOOST_FOREACH(const ptree::value_type&  v, pt.get_child("bot") )
    {
        if( v.first == name  )
        {
            // Get frame definitions
            m_xSpeed = v.second.get<float>("xSpeed",70)*random;
            m_ySpeed = v.second.get<float>("ySpeed",0)*random;
            m_amplitude = v.second.get<float>("amplitude",20)*random;
            m_freq = v.second.get<float>("freq",0.7)/random;
            m_isStatic =v.second.get<bool>("isStatic",false);
            m_isAttacking = v.second.get<bool>("isAttacking",false);
            m_swingX = v.second.get<bool>("swingX",false);
            m_swingY = v.second.get<bool>("swingY",false);
            m_firerate = v.second.get<float>("firerate",1)/random;
            m_currentHealth = v.second.get<float>("maxHealth",100)*random;
            m_points = v.second.get<float>("points",100)*random;
            m_attackdmg =v.second.get<float>("attackdmg",10)*random;
            m_kollisionsschaden = v.second.get<float>("kollisionsschaden",20)*random;
            // Lade Explosion anhand der größe des Sprite
            setupExplosion();

            // Assign bot sounds
            try
            {
                ptree sounds = v.second.get_child("sounds");
                for(auto it: sounds)
                {
                    m_mixer->assign(this, it.first, path + it.second.get("<xmlattr>.filename", ""));
                }

            }
            catch(boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::property_tree::ptree_bad_path> > e)
            {
            }
        }
    }
    m_currentAmplitude = m_amplitude;
    m_maxHealth = getHealth();

}


Projectile* Bot::newProjectile(Vector2f direction, int velocity)
{
    //Erzeuge ein Projectile. Renderer, Textur, Breite, Höhe, Animationsstufen, Friendly?, Richtung, Geschwindigkeit
    Projectile* projectile = new Projectile(m_renderer, m_projectilSprite, false, direction, velocity, m_kollisionsschaden, m_mixer);
    //Setze das Projektil an die Position des Bots
    projectile->setPosition(physics().position()+Vector2f(10,10));
    //Das Projektil des Bots soll mit 8FPS animiert werden.
    projectile->setFPS(8);
    return projectile;
}

std::vector<Actor*> Bot::shoot(Vector2f direction, int velocity, float dt, std::vector<Actor*> allprojectiles)
{
    // Speichert Zeit des Letzten Schusses
    m_lastShotDT += dt;
    // Falls Feuerrate erreicht, schieße
    if(m_lastShotDT >= m_firerate)
    {
        // Speichere Projektlie in Vektor
        allprojectiles.push_back(this->newProjectile(direction, velocity));
        // Setze Zeit seit letztem Schuss zurück
        m_lastShotDT = 0;
    }
    // Gib Vektor zurück+-
    return allprojectiles;
}

void Bot::setRotationPoint(Vector2f pos)
{
    // Zu setzender Punkt
    m_RotationPoint = pos;
    m_physicalProps.setPosition(pos);
}


std::vector<Actor*> Bot::onDeath()
{
    std::vector<Actor*> animation;
    KillAnimation* k = new KillAnimation(this,m_explodeFile, m_mixer);
    animation.push_back(k);
    m_mixer->play(this, "death");
    return animation;
}


void Bot::move(Level &level, float dt)
{
    if(m_RotationPoint.x() - level.m_camera.position().x() < m_camera.w() + 5)
    {
        if (dt > 0 )
        {
            m_alpha += dt * 2 * PI * m_freq * m_swingSign;
            nextFrame();
            if (!m_isStatic)
            {
                Vector2f v = Vector2f(0, 0);
                //auf frequenz angepasste Schwingdauer

                if (m_isMovingRegular)
                {
                    v += regularMovement(dt);
                    additionalBehaviour(level, dt);
                }
                else
                {
                    v+= irregularMovement(dt);
                }

                m_physicalProps.setPosition(v+ m_RotationPoint);

                if(m_collisionWithE)
                {
                     Collision c = level.resolveCollision(this);
                    planAvoiding(c,dt);
                }
            }
        }
    }
}


void Bot::planAvoiding(Collision c, float dt)
{
    if(c.type() != NONE)
    {
        m_RotationPoint = (m_physicalProps.position() - circle(m_alpha, m_amplitude, m_swingX, m_swingY));
        m_swingSign *= -1;
        if (m_isMovingRegular)
        {
            m_isMovingRegular = false;
            m_currentAmplitude = m_amplitude;
            if(m_amplitude == 0)
                m_currentAmplitude = 30;
        }
        if(c.type() == RIGHT)
        {
            m_isMovingRegular = true;
            m_currentAmplitude = m_amplitude;

        }
        else if (c.type() == DOWN || c.type() == UP)
        {
            m_currentAmplitude = m_amplitude;
            if(m_amplitude == 0)
               m_currentAmplitude = 30;
        }
    }
}

Vector2f Bot::irregularMovement(float dt)
{
    int dir = -1;
    if(m_player != NULL)
       dir = -sgn(m_RotationPoint.y()-m_player->position().y());

    float tmpAmplitude = m_amplitude;
    if(m_amplitude == 0)
        tmpAmplitude = 30;
    if(m_currentAmplitude >= 0)
    {
        m_currentAmplitude -= dt * 0.5 * tmpAmplitude;
        moveRotationPoint(m_xSpeed*dt,dir*20*dt);
    }
    else
    {
        m_isMovingRegular = true;
    }
    return circle(m_alpha, m_currentAmplitude, m_swingX, m_swingY);
}

Vector2f Bot::regularMovement(float dt)
{
    float tmpAmplitude = m_amplitude;
    if(m_amplitude == 0)
        tmpAmplitude = 30;
    if(m_currentAmplitude < m_amplitude)
        m_currentAmplitude += dt * 0.25 * tmpAmplitude;
    if(m_currentAmplitude > m_amplitude)
        m_currentAmplitude = m_amplitude;
    Vector2f v= circle(m_alpha, m_currentAmplitude, m_swingX, m_swingY);
    moveRotationPoint(-m_xSpeed*dt,m_ySpeed*dt);
    return v;
}


Vector2f Bot::circle(float radiant, float radius, bool swingX, bool swingY)
{
    float x = 0;
    float y = 0;
    if(swingX)
        x = cos(radiant);
    if(swingY)
        y = -sin(radiant);
    return Vector2f(x,y)*radius;
}


void Bot::moveRotationPoint(float x, float y)
{
    m_RotationPoint +=Vector2f(x, y);
}


std::vector<Actor*> Bot::bot_attack(float dt)
{
    std::vector<Actor*> projectiles;
    std::vector<Actor*> projectiles2;
    if (m_isAttacking)
    {
        projectiles2 = (shoot(Vector2f(-300,0), 100,  dt,  projectiles));
    }
    return projectiles2;
}

void Bot::collideWithActor(Actor& other)
{
    if(other.type()==PLAYER)
    {
        this->setDeath();
    }
    else if(other.type()==PLAYERPROJECTILE)
    {
        setHealth(m_currentHealth - other.kollisionsschaden());
    }
}

Bot::~Bot()
{
}


} /* namespace jumper */
