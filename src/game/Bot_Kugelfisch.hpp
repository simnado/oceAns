/**
 * A class representing a "Kugelfisch"-bot.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#ifndef BOT_KUGELFISCH_HPP
#define BOT_KUGELFISCH_HPP

#include "Bot.hpp"
#include "Projectile.hpp"
#include <vector>

namespace jumper
{

/**
 * @brief The Bot_Qualle class
 */
class Bot_Kugelfisch : public Bot
{
public:

    /**
     * @brief   Constructs an "Kugelfisch"boss with the given parameters.
     * @param   renderer            The renderer, which renders this bot.
     * @param   configFile          The path of the XML-File with the values
     * @param   filename            The path to the ".spr"-data
     * @param   mixer               The mixer, for the sounds for this specific bot
     */
    Bot_Kugelfisch(SDL_Renderer* renderer, std::string filename, std::string configFile, Mixer* mixer);

    /**
     * @brief               Overwrites bot_attack method for spezific behavior, shoots three projectiles
     * @param       dt      Timestamp
     * @return              A vector with projectiles
     */
    std::vector<Actor*> bot_attack(float dt);

    /**
     * @brief   Destroys this Bot
     */
    virtual ~Bot_Kugelfisch();

private:

    /// saves the maxFrequenz
    float m_maxFreq;

    /// saves the minFrequenz
    float m_minFreq;

    /// The sign
    int m_sign =1;

protected:

    /**
     * @brief   Additional behavior for this Bot
     * @param   level           The level in which the bot moves
     * @param   dt              Timestamp
     */
    virtual void additionalBehaviour(Level &level, float dt);



};

}



#endif
