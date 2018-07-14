
#ifndef BOT_SCHLANGE_HPP
#define BOT_SCHLANGE_HPP

#include "Bot.hpp"
#include "Projectile.hpp"
#include <vector>


namespace jumper
{

/**
 * @brief An actor that moves in a level according to a predefined movement pattern.
 * 		  The default implementation switches x-direction when it hits an object.
 */
class Bot_Schlange : public Bot
{
public:

    /**
     * @brief Bot_Schlange  Constructor
     * @param renderer      the renderer which shell render the snake
     * @param configFile    the file in which the properties of the snake are defined
     * @param filename      the filename for the texture of the snake
     * @param mixer         the mixer which is used to play the sounds of the snake
     */
    Bot_Schlange(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief bot_attack    overrides the virtual function (first implementes in the Bot class) with special parameters
     * @param dt            the time between the last call
     * @return              a vector of actors containing the fired projectiles
     */
    std::vector<Actor*> bot_attack(float dt);

    /**
     * @brief onDeath   overrides the virtual function (first implementes in the Actor class) ceating two new smal snakes
     * @return          a vector of actors with the the new snakes
     */
    std::vector<Actor*> onDeath();

    /**
     * @brief ~Bot_Schlange     Destructor
     */
    virtual ~Bot_Schlange();

};

}



#endif
