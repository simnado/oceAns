/**
 * A class representing a "Krabbe"-bot.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#ifndef BOT_KRABBE_HPP
#define BOT_KRABBE_HPP

#include "Bot.hpp"
#include "Projectile.hpp"

#include <vector>

namespace jumper
{

class Bot_Krabbe : public Bot
{
public:

    /**
     * @brief   Constructs an "Krabbe" bot with the given parameters.
     * @param   renderer            The renderer, which renders this bot.
     * @param   configFile          The path of the XML-File with the values
     * @param   filename            The path to the ".spr"-data
     * @param   mixer               The mixer, for the sounds for this specific bot
     */
    Bot_Krabbe(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief   Destroys this Bot
     */
    virtual ~Bot_Krabbe();

    /**
     * @brief                       Overwrites method. Handle the movements of crabs
     * @param   level               The Level in which the crab moves
     * @param   dt                  The Timestamp
     */
    virtual void move(Level &level, float dt);

private:

    /// The move Direction
    Vector2f m_moveDir;

};

}

#endif
