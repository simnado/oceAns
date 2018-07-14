
#include "Bot_Mine.hpp"

namespace jumper
{

Bot_Mine::Bot_Mine(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Mine", mixer)
{
    // setze die Todesanimation auf die große Exposion
    m_explodeFile = m_explodeFileBig;
}


void Bot_Mine::decTrigger(float time)
{
    // zähle bis zur triggerTime und setzte dann explode
    m_triggerCounter += time;
    if(m_triggerCounter >= m_triggerTime)
        m_expode = true;
}

void Bot_Mine::collisionMine(Actor* actor, std::vector<Bot_Mine*> m_mines)
{
    // wenn eine Mine stirbt, durch Kollision mit dem Spieler oder durch eine andere Mine, dann soll sie auch andere in ihrer Nähe explodieren lassen
    if(actor->type() == PLAYER || actor->type() == ENEMY)
    {
        this->setDeath();
    }
    else
    {
        this->setHealth(this->getHealth() - actor->kollisionsschaden());
    }

    if(this->isDead())
    {
        // erstelle ein rechteckiges Feld um die Mine in welcher Fläche auf andere Minen geprüft wird
        SDL_Rect minesurround;
        minesurround.w = this->w() * 4;
        minesurround.h = this->h() * 4;
        minesurround.x = (this->position().x() + this->w()/2) - minesurround.w / 2;
        minesurround.y = (this->position().y() + this->h()/2) - minesurround.h / 2;

        // pfüfe für alle gegebenen Minen, ob die in der Nähe sind
        for(auto it: m_mines)
        {
            if(!it->isDead())
            {
                SDL_Rect intersection;
                SDL_Rect otherRect;
                otherRect.x = it->position().x();
                otherRect.y = it->position().y();
                otherRect.w = it->w();
                otherRect.h = it->h();
                // wenn eine Mine in dem Bereich liegt trigger sie, so dass sie nach einer kurzen Verzögerung explodiert
                if(SDL_IntersectRect(&minesurround, &otherRect, &intersection))
                {
                    it->trigger(SDL_GetTicks());
                }
            }
        }
    }
    return;
}


Bot_Mine::~Bot_Mine()
{
}



} /* namespace jumper */
