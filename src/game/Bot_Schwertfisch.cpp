
#include "Bot_Schwertfisch.hpp"

namespace jumper
{

Bot_Schwertfisch::Bot_Schwertfisch(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Schwertfisch", mixer)
{
    // Don't collide withe environment
    m_collisionWithE = false;
}

void Bot_Schwertfisch::additionalBehaviour(Level &level, float dt)
{
    // Position of Player
    Vector2f diff = m_physicalProps.position() - m_player->position();

    // if Player is near
    if(diff.x() < 500 && diff.x() > 0 && !m_started)
        // set new bot position
        this->moveRotationPoint(0, -diff.y()*1.7*dt);

    // If swordfish "sees" player
    if(fabs(diff.y()) < 20 && diff.x()<450)
    {
        // start
        m_started = true;
        m_xSpeed = 500.0;
    }

}

Bot_Schwertfisch::~Bot_Schwertfisch()
{
    // TODO Auto-generated destructor stub
}



} /* namespace jumper */
