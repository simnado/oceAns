/**
 * A class representing a "swarm"-bot.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#include "Bot_Schwarm.hpp"

namespace jumper
{

Bot_Schwarm::Bot_Schwarm(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Schwarm", mixer)
    
{
    // Don't collide with environment
    m_collisionWithE = false;
}


Bot_Schwarm::~Bot_Schwarm()
{
    // TODO Auto-generated destructor stub
}

} /* namespace jumper */
