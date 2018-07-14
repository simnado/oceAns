
#ifndef BOT_MUSCHEL_HPP
#define BOT_MUSCHEL_HPP

#include "Bot.hpp"
#include "Projectile.hpp"
#include "Item.hpp"
#include <vector>


namespace jumper
{
class Player;
/**
 * @brief An actor that moves in a level according to a predefined movement pattern.
 * 		  The default implementation switches x-direction when it hits an object.
 */
class Bot_Muschel : public Bot
{
public:

    /***
     * @brief 	Constructs a bot from the given \ref filename
     * 			for the internal \ref renderer
     * @param renderer		A pointer to a SDL renderer struct
     */

    /**
     * @brief Bot_Muschel   Constructor
     * @param renderer      the renderer which shell render the shell
     * @param configFile    the file in which the properties of the shell are defined
     * @param filename      the filename for the texture of the shell
     * @param mixer         the mixer which is used to play the sounds of the shell
     */
    Bot_Muschel(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer);

    /**
     * @brief ~Bot_Muschel  Destructor
     */
    virtual ~Bot_Muschel();

    /**
     * @brief bot_attack    overrides the virtual function (first implementes in the Bot class) and determins the opening and closing of the shell
     * @param dt            the time past between the calls of the function
     * @return              a vector of Actors which is empty in this case since the shell is not shooting anything
     */
    std::vector<Actor*> bot_attack(float dt);

    /**
     * @brief onDeath   overrides the virtual function (first implementes in the Actor class) now handling the death in dependence to the status of the shell
     * @return          a vector of actors with the death animations which are dieffernet for the different kinds of death
     */
    std::vector<Actor*> onDeath();

    /**
     * @brief move      overrides the virtual function (first implementes in the Actor class) now handling the frames in dependence to the status of the shell but not moving it in postion
     * @param level     the level in which the shell is rendered
     * @param dt        the time past between the calls of the function
     */
    void move(Level &level, float dt);

    /**
     * @brief getOpening    to get the short moment in which the shell is open
     * @return              ture when the shell is opening
     */
    bool getOpening() {return posOpenFlanke; }

    /**
     * @brief getClosing    to get the short moment in which the shell is closed
     * @return              ture when the shell is closing
     */
    bool getClosing() {return negOpenFlanke; }

    /**
     * @brief getBoost  to get exess to the item the shell holds
     * @return          the item that the shell is offering
     */
    Item* getBoost() {return m_boost;}

    /**
     * @brief setBoost  to set the item the shell holds
     * @param           the item that the shell should offer
     */
    void setBoost(Item* newBoost) {m_boost = newBoost; }

    /**
     * @brief isOpen    to get the status of the shell determining if it is open
     * @return          true if the shell is open
     */
    bool isOpen(){return m_isOpen;}

private:

    /**
     * @brief m_isOpen  bool for the status of the shell: ture = open , false = closed
     */
    bool m_isOpen = false;

    /**
     * @brief m_maxKollisionsschaden    the normal collisiondamge of the shell (to remember when it is set to 0 when the shell is open)
     */
    int m_maxKollisionsschaden;

    /**
     * @brief m_changingCounter     counter to run trough all frames when the shell is closing or opening
     */
    int m_changingCounter = 0;

    /**
     * @brief m_boost   the item that the shell is offering
     */
    Item* m_boost;

    /**
     * @brief posOpenFlanke     ture in the short moment when the shell opening
     */
    bool posOpenFlanke = false;

    /**
     * @brief posOpenFlanke     ture in the short moment when the shell closing
     */
    bool negOpenFlanke = false;
};

}



#endif
