
#include "Bot_Meerjungfrau.hpp"

namespace jumper
{

Bot_Meerjungfrau::Bot_Meerjungfrau(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Meerjungfrau", mixer)
{
}


void Bot_Meerjungfrau::collideWithActor(Actor& other) {

    // if it collides with a player give him a and let the mermaid disappear
    if(other.type()==PLAYER)
    {
        this->setDeath();
        other.setPowerShoot(true);
        m_playerProjectilDeath = false;
    }
    else
        // if it collides with another actor decrease health
    {
        setHealth(m_currentHealth-other.kollisionsschaden());
        if(m_currentHealth <= 0)
            m_playerProjectilDeath = true;
    }
}

std::vector<Actor*> Bot_Meerjungfrau::onDeath()
{
    std::vector<Actor*> animation;
    // sparkling deathanimation if the player picks the mermaid up
    if(!m_playerProjectilDeath)
    {
        KillAnimation* k = new KillAnimation(this,m_explodeFilOrange, m_mixer);
        animation.push_back(k);
    }
    else
        // exposion death animation if the mermaid gets shot
    {
        KillAnimation* k = new KillAnimation(this,m_explodeFileBig, m_mixer);
        animation.push_back(k);
    }
    return animation;
}

Bot_Meerjungfrau::~Bot_Meerjungfrau()
{
}



} /* namespace jumper */
