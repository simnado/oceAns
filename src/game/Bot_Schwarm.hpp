/**
 * A class representing a "swarm"-bot.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#ifndef BOT_SCHWARM_HPP
#define BOT_SCHWARM_HPP

#include "Bot.hpp"

namespace jumper
{

/**
 * @brief The Bot_Schwarm class
 */
class Bot_Schwarm : public Bot
{
public:

    /**
     * @brief   Constructs an "Schwarm"boss with the given parameters.
     * @param   renderer            The renderer, which renders this bot.
     * @param   configFile          The path of the XML-File with the values
     * @param   filename            The path to the ".spr"-data
     * @param   mixer               The mixer, for the sounds for this specific bot
     */
    Bot_Schwarm(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief   Destroys this Bot
     */
    virtual ~Bot_Schwarm();
};
}
#endif
