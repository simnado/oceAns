
#include "Bot_Schlange.hpp"
#include "Bot_Schlange_klein.hpp"
#include "Game.hpp"

namespace jumper
{

Bot_Schlange::Bot_Schlange(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Schlange", mixer)
{
}


std::vector<Actor*> Bot_Schlange::bot_attack(float dt)
{
    std::vector<Actor*> projectiles;
    projectiles = shoot( Vector2f(-500,0), 80, dt, projectiles);
    return projectiles;
}


std::vector<Actor*> Bot_Schlange::onDeath()
{
    std::vector<Actor*> nachkommen;
    // if the snake is killed by a playerprojectil create two new small snakes at the same position with a slightly different rotaion point, that they are not on top of each other
    if(m_playerProjectilDeath)
    {
        this->setDeath();
        // first snake
        Bot_Schlange_klein* kl_Schlange1 = new Bot_Schlange_klein(this->m_renderer, "..\\res\\Bots.xml", "..\\res\\Data\\Schlange_klein.spr", m_mixer);
        kl_Schlange1->setPosition(Vector2f(500 , 300));
        kl_Schlange1->setRotationPoint(Vector2f(this->m_RotationPoint.x(), this->m_RotationPoint.y() -12 ));
        kl_Schlange1->setFPS(8);
        nachkommen.push_back(kl_Schlange1);
        // second snake
        Bot_Schlange_klein* kl_Schlange2 = new Bot_Schlange_klein(this->m_renderer, "..\\res\\Bots.xml", "..\\res\\Data\\Schlange_klein.spr", m_mixer);
        kl_Schlange2->setPosition(Vector2f(500 , 300));
        kl_Schlange2->setRotationPoint(Vector2f(this->m_RotationPoint.x() , this->m_RotationPoint.y() + 12 ));
        kl_Schlange2->setFPS(8);
        nachkommen.push_back(kl_Schlange2);
    }
    else
    {
        // if the sanke is killed not by a playerprojectile return the normal killanimation
        KillAnimation* a = new KillAnimation(this,m_explodeFile, m_mixer);
        nachkommen.push_back(a);
    }
    // play the death sound
     m_mixer->play(this,"death");
    return nachkommen;
}

Bot_Schlange::~Bot_Schlange()
{
}



} /* namespace jumper */


