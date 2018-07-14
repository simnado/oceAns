/**
 * A class representing a "Krabbe"-bot.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#include "Bot_Krabbe.hpp"
#include "Game.hpp"

namespace jumper
{

Bot_Krabbe::Bot_Krabbe(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Krabbe", mixer)
{
    m_moveDir = Vector2f(-30,80);
}


Bot_Krabbe::~Bot_Krabbe()
{
	// TODO Auto-generated destructor stub
}


void Bot_Krabbe::move(Level &level, float dt){
    // next Animation step
    nextFrame();
    // if Time is passed
    if(dt > 0)
    {
        // Check collison
        Collision c = level.resolveCollision(this);

        // bounce if bot collides with solid tile
        if(c.tiletype() == BLOCK && (c.type() == LEFT || c.type() == RIGHT))
            m_moveDir.setX(m_moveDir.x() * -1);

        // set new position
        physics().setPosition(physics().position() + (m_moveDir * dt));
    }
}


} /* namespace jumper */
