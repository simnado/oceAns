/**
 * A class representing a "Tentakel"-boss.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#ifndef BOSS_TENTAKEL_HPP
#define BOSS_TENTAKEL_HPP

#include "Bot.hpp"
#include "Game.hpp"
#include "Projectile.hpp"
#include "Player.hpp"

#include <vector>

namespace jumper
{

class Boss_Tentakel : public Bot
{

public:

    Boss_Tentakel(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer *mixer);

    virtual ~Boss_Tentakel();

    std::vector<Actor*> shoot(Vector2f direction, int velocity, float dt, std::vector<Actor*> allprojectiles);

private:

    // rate for first salve
    float m_salve =0.2;

    // rate for second salve
    float m_salve2 =0.2;

    // counter for first salve
    float m_salve_count =0;

    // first salve max
    float m_salve_max =10;

    // counter for second salve
    float m_salve_count2 =0;

    // first salve max
    float m_salve_max2 =50;

    // Timestamp for second shoot
    float m_lastShotDT2 = 0;
};

}



#endif
