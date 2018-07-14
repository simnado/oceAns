
#ifndef BOT_AAL_HPP
#define BOT_AAL_HPP

#include "Bot.hpp"
#include "Projectile.hpp"
#include <vector>

namespace jumper
{

/**
 * @brief An actor that moves in a level according to a predefined movement pattern.
 * 		  The default implementation switches x-direction when it hits an object.
 */
class Bot_Aal : public Bot
{
public:

    /**
     * @brief Bot_Aal       Constructor
     * @param renderer      the renderer which mine render the eel
     * @param configFile    the file in which the properties of the eel are defined
     * @param filename      the filename for the texture of the eel
     * @param mixer         the mixer which is used to play the sounds of the eel
     */
    Bot_Aal(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief collideWithActor  overrides the virtual function (first implementes in the Actor class) stunning the player when colliding
     * @param other             the Actor with which the shell collides
     */
    void collideWithActor(Actor& other);

    /**
     * @brief ~Bot_Aal  Destructor
     */
    virtual ~Bot_Aal();

};

}



#endif
