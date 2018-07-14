
#include "Bot_Muschel.hpp"

namespace jumper
{

Bot_Muschel::Bot_Muschel(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Muschel", mixer)
{
    //lege Anfangsframe fest und merke den normalen Kollisionsschaden, da dieser bei der Muschel sich je nach Zustand ändert
    m_currentFrame = 8;
    m_maxKollisionsschaden = m_kollisionsschaden;
}


std::vector<Actor*> Bot_Muschel::bot_attack(float dt)
{
    m_lastShotDT += dt;
    std::vector<Actor*> projektiles;

    // die firerate der Muschel wurde erreicht und sie invertiert ihren Zustand
    if(m_lastShotDT >= m_firerate)
    {
        m_isOpen = !m_isOpen;
        if(m_isOpen)
        {
            // Muschle öffnet sich -> positive Flanke für Öffnen, deaktiviere Kollisionsschaden und funkelde Todesanimation
            posOpenFlanke = true;
            m_explodeFile = m_explodeFileGreen;
            m_kollisionsschaden = 0;
        }
        else
        {
            // Muschel schließt sich -> negative Flanke fürs Öffnen, aktiviere Kollisionsschaden und Exposion als Todesanimation
            negOpenFlanke = true;
            m_explodeFile = m_explodeFileMedium;
            m_kollisionsschaden = m_maxKollisionsschaden;
        }
        // Counter auf 16 setzen um danach ein mal alle Frames zu durchlafen um die Zustandsänderung der Muschel zu animieren
        m_changingCounter = 16;
        m_lastShotDT = 0;
    }
    else
    {
        // wenn fierrate nicht erreicht wurde ist keine Flanke vorhanden
        negOpenFlanke = posOpenFlanke = false;
    }

    return projektiles;
}

void Bot_Muschel::move(Level &level, float dt)
{
    // Muschel defindet sich links von der Kamera
    if(m_RotationPoint.x()-level.m_camera.position().x()<-200) {
        m_death = true;
        return;
    }

    // wenn die Muschel in der Kamera ist
    if( m_RotationPoint.x()-level.m_camera.position().x()< m_camera.w()+5)
    {
        if (dt > 0 )
        {
            // wenn Counter größer 0 soll die Muschel animiert sein, da sie sich gerade öffnet oder schließt
            if(m_changingCounter > 0)
            {
                m_changingCounter--;
                nextFrame();
            }
            else
            {
                // statischer Zustand der Muschel, je nachdem, ob sie offen oder zu ist
                if(m_isOpen)
                {
                    m_currentFrame = 16;
                }
                else
                {
                    m_currentFrame = 8;
                }
            }
        }
    }
    if(m_RotationPoint.x()-level.m_camera.position().x()>m_camera.w()+5)
    {
        m_lastShotDT=-1;
    }
}


std::vector<Actor*> Bot_Muschel::onDeath()
{
    // creating a killanimation for the shell depending on its status
    std::vector<Actor*> animation;
    KillAnimation* a = new KillAnimation(this, m_explodeFile, m_mixer);
    animation.push_back(a);
    // playing the sound for the death of the shell
    m_mixer->play(this,"death");
    return animation;
}

Bot_Muschel::~Bot_Muschel()
{
}



} /* namespace jumper */
