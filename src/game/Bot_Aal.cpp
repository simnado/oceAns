#include "Bot_Aal.hpp"
#include "Game.hpp"

namespace jumper
{

Bot_Aal::Bot_Aal(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Aal", mixer)
{
}

void Bot_Aal::collideWithActor(Actor& other)
{
    // if it collides with the player stun the player and play the respective sound
    if(other.type() == PLAYER)
    {
        other.setHealth(other.getHealth() - m_kollisionsschaden);
        other.setStun();
        m_mixer->play(this,"stun");
    }
    // if it collides with a projectile decrease health and play the hit sound
    else if(other.type() == PLAYERPROJECTILE)
    {
        setHealth(getHealth() - other.kollisionsschaden());
        m_mixer->play(this,"hit");
    }
}

Bot_Aal::~Bot_Aal()
{
}



}
