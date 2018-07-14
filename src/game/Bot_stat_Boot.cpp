
#include "Bot_stat_Boot.hpp"
#include "Projectile.hpp"
#include "Game.hpp"

namespace jumper
{


Bot_stat_Boot::Bot_stat_Boot(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_stat_Boot", mixer)
{
    // Change Projectile sprite to an anchor
    m_projectilSprite="..\\res\\Data\\Projektile\\Anker.spr";
}

std::vector<Actor*> Bot_stat_Boot::bot_attack(float dt)
{
    // Throws an anchor
    std::vector<Actor*> projectiles;
    projectiles = shoot( Vector2f(0,500), 200, dt, projectiles);
    return projectiles;
}

Bot_stat_Boot::~Bot_stat_Boot()
{
}



} /* namespace jumper */
