/**
 * A class representing a "Schwertfisch"-bot.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#ifndef BOT_SCHWERTFISCH_HPP
#define BOT_SCHWERTFISCH_HPP

#include "Bot.hpp"
#include "Projectile.hpp"
#include "Player.hpp"
#include <vector>


namespace jumper
{

/**
 * @brief An actor that shoots
 */
class Bot_Schwertfisch : public Bot
{
public:

    /**
     * @brief   Constructs an "Schwertfisch" bot with the given parameters.
     * @param   renderer            The renderer, which renders this bot.
     * @param   configFile          The path of the XML-File with the values
     * @param   filename            The path to the ".spr"-data
     * @param   mixer               The mixer, for the sounds for this specific bot
     */
    Bot_Schwertfisch(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief               Overwrites bot_attack method for spezific behavior
     * @param       dt      Timestamp
     * @return              A vector with projectiles
     */
    std::vector<Actor*> attack(float dt);

    /**
     * @brief   Additional behavior for this Bot
     * @param   level           The level in which the bot moves
     * @param   dt              Timestamp
     */
    void additionalBehaviour(Level &level, float dt);

    /**
     * @brief destroys Bot
     */
    virtual ~Bot_Schwertfisch();


private:

    /// is the swordfish started
    bool m_started = false;

};

}



#endif
