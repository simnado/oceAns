#include "Mixer.hpp"
#include <iostream>
#include <typeinfo>


namespace jumper
{

Mixer::Mixer()
{
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}

void Mixer::assign(Renderable* entity, std::string tag, std::string path)
{
     //std::cout << "assign: " << entity << " " << tag << " " << path.c_str() << std::endl;
     Mix_Chunk* sound = Mix_LoadWAV( path.c_str() );
     if( sound == NULL )
     {
         //std::cout << entity << ": Failed to load " << tag << " sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
     }
     m_sounds[entity][tag] = sound;
}

void Mixer::play(Renderable* entity, std::string tag)
{
    //std::cout << "play: " << entity << " " << tag << std::endl;
    auto it1 = m_sounds.find(entity);
    if(it1 != m_sounds.end())
    {
    	//std::cout << "entity found" << std::endl;
        auto it2 = it1->second.find(tag);

        if(it2 != it1->second.end())
        {
        	//std::cout << "tag found" << std::endl;
        	//std::cout << it2->second << std::endl;
            Mix_PlayChannel(-1, it2->second, 0);
        }
        else
        {
            //std::cout << tag << ": No such sound could be found for " << entity << "." << std::endl;
        }
    }
    else
    {
        //std::cout << tag << ": No such sound could be found for " << entity << "." << std::endl;
    }
}

void Mixer::setScore(Renderable* entity, std::string tag, std::string path)
{
    ///std::cout << "setScore: " << entity << " " << tag << " " << path << std::endl;
    Mix_Music* score = Mix_LoadMUS( path.c_str() );
    if( score == NULL )
    {
        ///std::cout << entity << ": Failed to load '" << tag << "' soundtrack! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    m_scores[entity][tag] = score;
}

void Mixer::playScore(Renderable* entity, std::string tag)
{
    ///std::cout << "playScore: " << entity << " " << tag << std::endl;

    auto it1 = m_scores.find(entity);
    if(it1 != m_scores.end())
    {
        auto it2 = it1->second.find(tag);

        if(it2 != it1->second.end())
        {
            Mix_PlayMusic(it2->second, -1);
        }
        else
        {
            ///std::cout << tag << ": Score could not be found for " << entity << "." << std::endl;
        }
    }
    else
    {
        ///std::cout << tag << ": No such sound could be found for " << entity << "." << std::endl;
    }
}

void Mixer::stopScore()
{
    Mix_HaltMusic();
}

void Mixer::pauseAndResume()
{
    if(Mix_PausedMusic()) Mix_ResumeMusic();
    else Mix_PauseMusic();
}

void Mixer::deleteFx(Actor* entity)
{
    ///std::cout << "delete " << entity << std::endl;
    auto it1 = m_sounds.find((Renderable*) entity);
    if(it1 != m_sounds.end())
    {
        for(auto it2 : (*it1).second)
        {
            Mix_FreeChunk(it2.second);
        }
        m_sounds.erase(it1);
    }
}

Mixer::~Mixer()
{
    for(auto it1 = m_sounds.begin(); it1 != m_sounds.end(); it1++)
    {
        for(auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
        {
            Mix_FreeChunk((*it2).second);
            m_sounds[(*it1).first].erase(it2);
        }
        m_sounds.erase(it1);
    }

    for(auto it1 = m_scores.begin(); it1 != m_scores.end(); it1++)
    {
        for(auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
        {
            Mix_FreeMusic((*it2).second);
            m_scores[(*it1).first].erase(it2);
        }
        m_scores.erase(it1);
    }
}

} //namespace jumper
