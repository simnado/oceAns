/**
 * A class representing a "Hai"-bot.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#include "Bot_Hai.hpp"

namespace jumper
{

Bot_Hai::Bot_Hai(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Hai", mixer)
{
}

Bot_Hai::~Bot_Hai()
{
    // TODO Auto-generated destructor stub
}


void Bot_Hai::additionalBehaviour(Level &level, float dt)
{
    // Position to Player
    Vector2f diff = m_physicalProps.position() - m_player->position();

    // While Bot is ahead of the player
    if(diff.x() > 0)
        // Sets new Rotation Point
        this->moveRotationPoint(0, -diff.y()*0.9*dt);
}
} /* namespace jumper */
