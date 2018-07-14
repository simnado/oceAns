/**
 * A class representing a "Seestern"-bot.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#include "Bot_Seestern.hpp"

namespace jumper
{

Bot_Seestern::Bot_Seestern(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Seestern", mixer)
{
    m_collisionWithE = false;
}

Bot_Seestern::~Bot_Seestern()
{
    // TODO Auto-generated destructor stub
}

void Bot_Seestern::additionalBehaviour(Level &level, float dt)
{
    Vector2f diff = m_physicalProps.position() - m_player->position();
    if(diff.x() > 0)
        this->moveRotationPoint(0, -diff.y()*0.9*dt);
}
} /* namespace jumper */
