
#ifndef BOT_MEERJUNGFRAU_HPP
#define BOT_MEERJUNGFRAU_HPP

#include "Bot.hpp"


namespace jumper
{

/**
 * @brief An actor that moves in a level according to a predefined movement pattern.
 * 		  The default implementation switches x-direction when it hits an object.
 */
class Bot_Meerjungfrau : public Bot
{
public:

    /**
     * @brief Bot_Meerjungfrau  Constructor
     * @param renderer          the renderer which mine render the mermaid
     * @param configFile        the file in which the properties of the mermaid are defined
     * @param filename          the filename for the texture of the mermaid
     * @param mixer             the mixer which is used to play the sounds of the mermaid
     */
    Bot_Meerjungfrau(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief collideWithActor  overrides the virtual function (first implementes in the Actor class) to give the player by collsision a boost
     * @param other             the Actor with which the mermaid collides
     */
    virtual void collideWithActor(Actor& other);

    /**
     * @brief onDeath       overrides the virtual function (first implementes in the Actor class) now handling the deathanimation in dependence of the kind of death
     * @return              a vector of actors with the death animations which are dieffernet for the different kinds of death
     */
    std::vector<Actor*> onDeath();

    /**
     * @brief ~Bot_Meerjungfrau     Destructor
     */
    virtual ~Bot_Meerjungfrau();

};

}



#endif
