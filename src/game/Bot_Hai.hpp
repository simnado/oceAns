/**
 * A class representing a "Hai"-bot.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#ifndef BOT_HAI_HPP
#define BOT_HAI_HPP

#include "Bot.hpp"
#include "Player.hpp"

namespace jumper
{

class Bot_Hai : public Bot
{
public:

    /**
     * @brief   Constructs an "Hai_bot" with the given parameters.
     * @param   renderer            The renderer, which renders this bot.
     * @param   configFile          The path of the XML-File with the values
     * @param   filename            The path to the ".spr"-data
     * @param   mixer               The mixer, for the sounds for this specific bot
     */
    Bot_Hai(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief   Additional behavior for this Bot
     * @param   level           The level in which the bot moves
     * @param   dt              Timestamp
     */
    void additionalBehaviour(Level &level, float dt);

    /**
     * @brief   Destroys this Bot
     */
    virtual ~Bot_Hai();
};
}
#endif
