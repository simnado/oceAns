
#include "Bot_Schlange_klein.hpp"
#include "Game.hpp"

namespace jumper
{

Bot_Schlange_klein::Bot_Schlange_klein(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Schlange_klein", mixer)
{
}

std::vector<Actor*> Bot_Schlange_klein::bot_attack(float dt)
{
    std::vector<Actor*> projectiles;
    projectiles = shoot( Vector2f(-500,0), 50, dt, projectiles);
    return projectiles;
}

Bot_Schlange_klein::~Bot_Schlange_klein()
{
}

} /* namespace jumper */


