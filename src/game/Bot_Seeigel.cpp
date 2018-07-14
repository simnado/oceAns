
#include "Bot_Seeigel.hpp"
#include "Game.hpp"
#include "Projectile.hpp"

namespace jumper
{

Bot_Seeigel::Bot_Seeigel(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Seeigel", mixer)
{
}


std::vector<Actor*> Bot_Seeigel::bot_attack(float dt)
{
    m_lastShotDT += dt;
    std::vector<Actor*> Projectiles;

    if(m_lastShotDT >= m_firerate)
    {
        // the left projectil
        m_projectilSprite = "..\\res\\Data\\Projektile\\Stachel_225.spr";
        Projectiles.push_back(this->newProjectile( Vector2f(-300,-300), 60));
        // the middel projectil
        m_projectilSprite = "..\\res\\Data\\Projektile\\Stachel_270.spr";
        Projectiles.push_back(this->newProjectile( Vector2f(0,-500), 60));
        // the right projectil
        m_projectilSprite = "..\\res\\Data\\Projektile\\Stachel_315.spr";
        Projectiles.push_back(this->newProjectile( Vector2f(300,-300), 60));

        m_lastShotDT = 0;
    }
    return Projectiles;
}

Bot_Seeigel::~Bot_Seeigel()
{
}



} /* namespace jumper */
