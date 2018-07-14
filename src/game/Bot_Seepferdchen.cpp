
#include "Bot_Seepferdchen.hpp"
#include "Game.hpp"
#include "Projectile.hpp"

namespace jumper
{

Bot_Seepferdchen::Bot_Seepferdchen(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Seepferd", mixer)
{
}

std::vector<Actor*> Bot_Seepferdchen::bot_attack(float dt)
{
    //shoot
    std::vector<Actor*> projectiles;
    projectiles = shoot( Vector2f(-500,0), 150, dt, projectiles);
    return projectiles;
}

Bot_Seepferdchen::~Bot_Seepferdchen()
{
    // TODO Auto-generated destructor stub
}



} /* namespace jumper */
