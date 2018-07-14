/**
 * A class representing a "Roboterfisch"-boss.
 *
 * @author Andreas Schroeder, Benedikt Vettelschoss, Eduard Rempel, Henning Diekamp und  Nadine Heere
 */
#include "Boss_Roboterfisch.hpp"
// Using namespace jumper
namespace jumper
{

Boss_Roboterfisch::Boss_Roboterfisch(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Boss_Roboterfisch", mixer)
{
    // is a Boss
    m_isBoss=true;
    // this Bot don't Collide with the Environment
    m_collisionWithE=false;
}


Boss_Roboterfisch::~Boss_Roboterfisch()
{
    // TODO Auto-generated destructor stub
}

void Boss_Roboterfisch::additionalBehaviour(Level &level, float dt)
{
    // creates a Vector which leads to the playerposition
    Vector2f diff = m_physicalProps.position() - m_player->position();
    // Moves the Bot to the y playerposition
    this->moveRotationPoint(0, -diff.y()*dt);
}

std::vector<Actor*> Boss_Roboterfisch::shoot(Vector2f direction, int velocity, float dt, std::vector<Actor*> allprojectiles)
{
    // Timestamps for the firerates
    m_lastShotDT += dt;
    m_lastShotDT2 += dt;
    // Shoot, wehen firerate is reached
    if(m_lastShotDT >= m_firerate)
    {
        // Sets the sprites for the projectiles and shoot in a half-circle
        m_projectilSprite = "..\\res\\Data\\Projektile\\Ball_gruen.spr";
        allprojectiles.push_back(this->newProjectile(Vector2f((float)-300,(float)0), velocity*2));
        m_projectilSprite = "..\\res\\Data\\Projektile\\Ball_rot.spr";
        allprojectiles.push_back(this->newProjectile(Vector2f((float)-300,(float)150), velocity*2));
        allprojectiles.push_back(this->newProjectile(Vector2f((float)-300,(float)-150), velocity*2));
        m_projectilSprite = "..\\res\\Data\\Projektile\\Ball_lila.spr";
        allprojectiles.push_back(this->newProjectile(Vector2f((float)-300,(float)300), velocity*2)); 
        allprojectiles.push_back(this->newProjectile(Vector2f((float)-300,(float)-300), velocity*2));
        m_projectilSprite = "..\\res\\Data\\Projektile\\Ball_blau.spr";
        allprojectiles.push_back(this->newProjectile(Vector2f((float)-150,(float)300), velocity*2));
        allprojectiles.push_back(this->newProjectile(Vector2f((float)-150,(float)-300), velocity*2));
        m_projectilSprite = "..\\res\\Data\\Projektile\\Ball_gold.spr";
        allprojectiles.push_back(this->newProjectile(Vector2f((float)0,(float)300), velocity*2));
        allprojectiles.push_back(this->newProjectile(Vector2f((float)0,(float)-300), velocity*2));
        m_lastShotDT= 0;
    }
    // Tests if this salve is ready
    if(m_salve_count<=m_salve_max)
    {
        // For each shoot in salve
        if (m_lastShotDT2-m_firerate*2 >=(m_salve*m_salve_count+1))
        {
            // reads Vector coordinates to the player
            float new_x = -(m_physicalProps.position().x()-m_player->position().x());
            float new_y =-( m_physicalProps.position().y()-m_player->position().y());

            // colorate the projectiles in different collors
            switch(((int)m_salve_count+1)%((int)m_salve_max+1))
            {
            case 0:
                 m_projectilSprite = "..\\res\\Data\\Projektile\\Ball_gruen_gross.spr";
                break;
            case 1:
                 m_projectilSprite = "..\\res\\Data\\Projektile\\Ball_rot_gross.spr";
                break;
            case 2:
                 m_projectilSprite = "..\\res\\Data\\Projektile\\Ball_lila_gross.spr";
                break;
            case 3:
                 m_projectilSprite = "..\\res\\Data\\Projektile\\Ball_blau_gross.spr";
                break;
            case 4:
                 m_projectilSprite = "..\\res\\Data\\Projektile\\Ball_gold_gross.spr";
                break;
            }

            // puts Projectile in vector
            allprojectiles.push_back(this->newProjectile(Vector2f((float)new_x,(float)new_y), velocity*2));
            // counts the shoots in this salve
            m_salve_count++;
        }
    }else{
        // reset counters
        m_salve_count=0;
        m_lastShotDT2= 0;
    }
    return allprojectiles;
}
} /* namespace jumper */
