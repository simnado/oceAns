/**
 * A class representing a "Tentakel"-boss.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#include "Boss_Tentakel.hpp"


namespace jumper
{
/**
 * @brief   Constructs an "Tentakel_boss" with the given parameters.
 * @param   renderer            The renderer, which renders this bot.
 * @param   configFile          The path of the XML-File with the values
 * @param   filename            The path to the ".spr"-data
 * @param   mixer               The mixer, for the sounds for this specific bot
 */
Boss_Tentakel::Boss_Tentakel(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Boss_Tentakel", mixer)
{
    // is a Boss
    m_isBoss=true;

    // Don't collide with Environment
    m_collisionWithE=false;
}

/**
 * @brief   Destroys this Bot
 */
Boss_Tentakel::~Boss_Tentakel()
{
    // TODO Auto-generated destructor stub
}

/**
 * @brief   Overwrites the shoot-method of Bot
 * @param   direction       the direction for the projectile
 * @param   velocity        the speed of the projectile
 * @param   dt              Timestamp
 * @param   allprojectiles  A Vector of all projectiles
 * @return  returns a vector with all projectiles, which were shoot
 */
std::vector<Actor*> Boss_Tentakel::shoot(Vector2f direction, int velocity, float dt, std::vector<Actor*> allprojectiles)
{
    // Timestamps for firerates
    m_lastShotDT += dt;
    m_lastShotDT2 += dt;

    // if firerate is reached
    if(m_lastShotDT >= m_firerate)
    {
        // Coordinates, which leads to player position
        float new_x = -(m_physicalProps.position().x()-m_player->position().x());
        float new_y =-( m_physicalProps.position().y()-m_player->position().y());

        // If savle max isn't reached
        if (m_salve_count<=m_salve_max){
            // Next shoot in salve
            if (m_lastShotDT-m_firerate >=(m_salve*m_salve_count+1))
            {
                // Loads sprite for projectile and put in vector
                m_projectilSprite = "..\\res\\Data\\Projektile\\Ball_lila_gross.spr";
                allprojectiles.push_back(this->newProjectile(Vector2f((float)new_x,(float)new_y), velocity*2));

                //increase counter
                m_salve_count++;
            }
        }else{
            // reset counters
            m_salve_count=0;
            m_lastShotDT = 0;
        }
    }
    // if firerate is reached
    if(m_lastShotDT2 >= m_firerate*1)
    {
         // If savle max isn't reached
        if (m_salve_max2>m_salve_count2)
        {
             // Next shoot in salve
            if (m_lastShotDT2-m_firerate*1 >=(m_salve2*m_salve_count2))
            {
                // Loads sprite for projectile and put in vector
                m_projectilSprite = "..\\res\\Data\\Projektile\\Flamme_180.spr";
                allprojectiles.push_back(this->newProjectile(Vector2f((float)-300,(float)0), velocity*2));

                 //increase counter
                m_salve_count2++;
            }
        }else{
            // reset counter
            m_salve_count2=0;
            m_lastShotDT2 = 0;
        }
    }
    return allprojectiles;
}
} /* namespace jumper */
