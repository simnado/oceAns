
#ifndef BOT_SEEIGEL_HPP
#define BOT_SEEIGEL_HPP

#include "Bot.hpp"
#include "Projectile.hpp"
#include <vector>


namespace jumper
{

/**
 * @brief An actor that moves in a level according to a predefined movement pattern.
 * 		  The default implementation switches x-direction when it hits an object.
 */
class Bot_Seeigel : public Bot
{
public:

    /**
     * @brief Bot_Seeigel   Constructor
     * @param renderer      the renderer which mine render the sea urchin
     * @param configFile    the file in which the properties of the sea urchin are defined
     * @param filename      the filename for the texture of the sea urchin
     * @param mixer         the mixer which is used to play the sounds of the sea urchin
     */
    Bot_Seeigel(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief bot_attack    overrides the virtual function (first implementes in the Bot class) shooting three projectiles at the same time
     * @param dt            the time between the last call
     * @return              a vector of actors containing all the fired projectiles
     */
    std::vector<Actor*>  bot_attack(float dt);

    /**
     * @brief ~Bot_Seeigel  Destructor
     */
    virtual ~Bot_Seeigel();

};

}



#endif
