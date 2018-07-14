
#include "Bot_mov_Boot.hpp"
#include "Game.hpp"
#include "Projectile.hpp"

namespace jumper
{

Bot_mov_Boot::Bot_mov_Boot(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_mov_Boot", mixer)
{
}

std::vector<Actor*> Bot_mov_Boot::bot_attack(float dt)
{
    // Shoot
    std::vector<Actor*> projectiles;
    projectiles = shoot( Vector2f(-500,0), 180, dt, projectiles);
    return projectiles;
}

Bot_mov_Boot::~Bot_mov_Boot()
{
    // TODO Auto-generated destructor stub
}



} /* namespace jumper */
