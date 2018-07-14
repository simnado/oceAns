
#ifndef BOT_SCHLANGE_KLEIN_HPP
#define BOT_SCHLANGE_KLEIN_HPP

#include "Bot.hpp"
#include "Projectile.hpp"
#include <vector>


namespace jumper
{

/**
 * @brief An actor that moves in a level according to a predefined movement pattern.
 * 		  The default implementation switches x-direction when it hits an object.
 */
class Bot_Schlange_klein : public Bot
{
public:

    /**
     * @brief Bot_Schlange_klein    Constructor
     * @param renderer              the renderer which shell render the small snake
     * @param configFile            the file in which the properties of the small snake are defined
     * @param filename              the filename for the texture of the small snake
     * @param mixer                 the mixer which is used to play the sounds of the small snake
     */
    Bot_Schlange_klein(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief bot_attack    overrides the virtual function (first implementes in the Bot class) with special (weaker) parameters
     * @param dt            the time between the last call
     * @return              a vector of actors containing the fired projectiles
     */
    std::vector<Actor*> bot_attack(float dt);


    /**
     * @brief ~Bot_Schlange_klein   Destructor
     */
    virtual ~Bot_Schlange_klein();


};

}



#endif
