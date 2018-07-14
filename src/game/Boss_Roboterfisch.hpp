/**
 * A class representing a "Roboterfisch"-boss.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#ifndef BOSS_ROBOTERFISCH_HPP
#define BOSS_ROBOTERFISCH_HPP

// include needed headers
#include "Bot.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Projectile.hpp"

#include <vector>

namespace jumper
{

/**
 * @brief An actor that moves in a level according to a predefined movement pattern.
 * 		  The default implementation switches x-direction when it hits an object.
 */
class Boss_Roboterfisch : public Bot
{
public:

    /**
     * @brief   Constructs an "Roboterfisch_boss" with the given parameters.
     * @param   renderer            The renderer, which renders this bot.
     * @param   configFile          The path of the XML-File with the values
     * @param   filename            The path to the ".spr"-data
     * @param   mixer               The mixer, for the sounds for this specific bot
     */
    Boss_Roboterfisch(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief   Destroys this Bot
     */
    virtual ~Boss_Roboterfisch();

    /**
     * @brief   Additional behavior for this Bot
     * @param   level           The level in which the bot moves
     * @param   dt              Timestamp
     */
    void additionalBehaviour(Level &level, float dt);

    /**
     * @brief   Overwrites the shoot-method of Bot
     * @param   direction       the direction for the projectile
     * @param   velocity        the speed of the projectile
     * @param   dt              Timestamp
     * @param   allprojectiles  A Vector of all projectiles
     * @return  returns a vector with all projectiles, which were shoot
     */
    std::vector<Actor*> shoot(Vector2f direction, int velocity, float dt, std::vector<Actor*> allprojectiles);

private:
    /// Timer for salve
    float m_salve =0.3;

    /// counts the given shoots in salve
    float m_salve_count =0;

    /// The maximum of the salveshoots
    float m_salve_max =4;

    /// Timestamp for second shoot
    float m_lastShotDT2;
};
}
#endif
