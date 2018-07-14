
#include "Bot_Kugelfisch.hpp"
#include "Game.hpp"

namespace jumper
{

Bot_Kugelfisch::Bot_Kugelfisch(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Kugelfisch", mixer)
{
    // Sets Frequenz borders
    m_minFreq = 0.9 * m_freq;
    m_maxFreq = 3 * m_freq;
}

std::vector<Actor*> Bot_Kugelfisch::bot_attack(float dt)
{
    // shoot
    std::vector<Actor*> projectiles;
    projectiles = shoot( Vector2f(-500,0), 70, dt, projectiles);
    return projectiles;
}

Bot_Kugelfisch::~Bot_Kugelfisch()
{
    // TODO Auto-generated destructor stub
}

void Bot_Kugelfisch::additionalBehaviour(Level &level, float dt){
    // increase and decrease Frequenz of circeling
    if(m_sign == 1){
        m_freq += dt*0.3;
        if(m_freq > m_maxFreq)
            m_sign = -1;
    }
    else if(m_sign == -1){
        m_freq -= dt*0.3;
        if(m_freq < m_minFreq)
            m_sign = 1;
    }
}


} /* namespace jumper */
