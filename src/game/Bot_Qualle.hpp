/**
 * A class representing a "Qualle"-bot.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#ifndef BOT_QUALLE_HPP
#define BOT_QUALLE_HPP

#include "Bot.hpp"


namespace jumper
{

/**
 * @brief The Bot_Qualle class
 */
class Bot_Qualle : public Bot
{
public:

    /**
     * @brief   Constructs an "Qualle"boss with the given parameters.
     * @param   renderer            The renderer, which renders this bot.
     * @param   configFile          The path of the XML-File with the values
     * @param   filename            The path to the ".spr"-data
     * @param   mixer               The mixer, for the sounds for this specific bot
     */
    Bot_Qualle(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief   Destroys this Bot
     */
    virtual ~Bot_Qualle();

};

}



#endif
