
#ifndef BOT_MINE_HPP
#define BOT_MINE_HPP

#include "Bot.hpp"

namespace jumper
{

/**
 * @brief An actor that moves in a level according to a predefined movement pattern.
 * 		  The default implementation switches x-direction when it hits an object.
 */
class Bot_Mine  : public Bot
{
public:

    /**
     * @brief Bot_Mine      Constructor
     * @param renderer      the renderer which mine render the mine
     * @param configFile    the file in which the properties of the mine are defined
     * @param filename      the filename for the texture of the mine
     * @param mixer         the mixer which is used to play the sounds of the mine
     */
    Bot_Mine(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief istriggered   gives the status of the mine, if it is triggered by another mine
     * @return              true if it is triggered
     */
    bool istriggered() { return m_triggered; }

    /**
     * @brief trigger   sets the trigger bool of the mine to true
     */
    void trigger(float dt) {m_triggered = true; m_triggerStart = dt; }

    /**
     * @brief decTrigger    decreasing the trigger time of the mine until it is time to explode
     * @param time          the time past the last call
     */
    void decTrigger(float time);

    /**
     * @brief timeToExplode     checks if it is time for the mine to expode because of the (trigger time reached)
     * @return                  true if it is time to explode
     */
    bool timeToExplode() { return m_expode; };

    /**
     * @brief getTriggerStart   to see when the mine got triggered
     * @return                  the time when the mine got triggered
     */
    float getTriggerStart() { return m_triggerStart; }

    /**
     * Checks explosion range of mines and leds to explosion of near mines
     *
     * @param actor     actor which collided with mine
     * @param m_mines   all mines in the game
     */
    void collisionMine(Actor* actor,  std::vector<Bot_Mine*> m_mines);

    /**
     * @brief ~Bot_Mine Destructor
     */
    virtual ~Bot_Mine();

private:

    /**
     * @brief m_triggered   true if the mine is triggered by another one
     */
    bool m_triggered = false;

    /**
     * @brief m_triggerCounter  counts to m_triggerTime
     */
    float m_triggerCounter = 0;

    /**
     * @brief m_triggerTime     the time between the mine is triggerd and the moment when it expodes
     */
    float m_triggerTime = 100000;

    /**
     * @brief m_expode  true when it is time to expode
     */
    bool m_expode = false;

    /**
     * @brief m_triggerStart    time when the mine got triggered
     */
    float m_triggerStart;




};

}



#endif
