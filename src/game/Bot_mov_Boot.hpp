/**
 * A class representing a "mov_boot"-bot.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#ifndef BOT_MOV_BOOT_HPP
#define BOT_MOV_BOOT_HPP

#include "Bot.hpp"
#include "Projectile.hpp"
#include <vector>


namespace jumper
{

/**
 * @brief An actor that shoots
 */
class Bot_mov_Boot : public Bot
{
public:

    /**
     * @brief   Constructs an "mov Boot" bot with the given parameters.
     * @param   renderer            The renderer, which renders this bot.
     * @param   configFile          The path of the XML-File with the values
     * @param   filename            The path to the ".spr"-data
     * @param   mixer               The mixer, for the sounds for this specific bot
     */
    Bot_mov_Boot(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief               Overwrites bot_attack method for spezific behavior
     * @param       dt      Timestamp
     * @return              A vector with projectiles
     */
    std::vector<Actor*> bot_attack(float dt);

    /**
     * @brief destroys Bot
     */
    virtual ~Bot_mov_Boot();

};

}



#endif
