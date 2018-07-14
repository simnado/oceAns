/**
 * @author Team 2
 */

#ifndef SRC_GAME_HPP_
#define SRC_GAME_HPP_

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include "MainWindow.hpp"
#include "Actor.hpp"
#include "Bot.hpp"
#include "Projectile.hpp"
#include "Level.hpp"
#include "TexturedLayer.hpp"
#include "ScoreBoard.hpp"
#include "LifeBoard.hpp"
#include "Mixer.hpp"
#include "Bot_Mine.hpp"


#include "Collidable.hpp"

#include <vector>
#include <set>
using std::vector;
using std::set;

namespace jumper
{

class Player;
class MainWindow;

/**
 * @brief   Represents a game instance.
 */
class Game
{
public:

    /**
     * Constructor
     */
    Game(MainWindow* window);

    /**
     * Destructor
     */
    virtual ~Game();

    /**
     * Setup the game, initialize the game/level attributes read from a given xml file
     *
     * @param filename      Name of the xml file which contains the level attributes
     */
    void setupGame(string filename);

    /**
     * Sets player
     *
     * @param player        Pointer to the player
     */
    void setPlayer(Player* player);

    /**
     * Creates the Ending Item
     *
     * @param bot   The Bot which spawns the item after dead
     */
    void createEnding(Bot* bot);

    /**
     * Returns the player
     *
     * @return player
     */
    Player* getPlayer() const { return m_player; };

    /**
     * Sets level
     *
     * @param level        Pointer to the level
     */
    void setLevel(Level* level);

    /**
     * Returns the level
     *
     * @return level
     */
    Level* getLevel() const { return m_level; };

    /**
     * Returns the current level name
     *
     * @return levelname
     */
    string getCurrentLevelName() const { return m_currentLevelName; };

    /**
     * Sets the next level name
     *
     * @param levelname
     */
    void setNextLevelName(string levelname) { m_nextLevelName = levelname; };

    /**
     * Returns the next level name
     *
     * @return nextLevelName
     */
    string getNextLevelName() const { return m_nextLevelName; };

    /**
     * Adds actor to the game
     *
     * @param actor     Actor which should be administered by the game
     */
    void addActor(Actor* actor);

    /**
     * Updates the current state of the game
     * handles player according to the given key states
     * as well as all renderables
     *
     * @param currentKeyStates
     */
    void update(const Uint8* &currentKeyStates);

    /**
     * Starts the game
     */
    void start();

    /**
     * Pause the game and shows the pause menu
     */
    void pause();

    /**
     * Sets the layer for background rendering
     */
    void setLayer(TexturedLayer* layer) { m_layer = layer; };

    /**
     * Gets layer for background rendering
     */
    TexturedLayer* getLayer() { return m_layer; };

    /**
     * Sets the score board
     *
     * @param scoreBoard
     */
    void setScoreBoard(ScoreBoard* b) { m_scoreBoard = b; };

    /**
     * Adds the life board
     *
     * @param lifeBoard
     */
    void setLifeBoard(LifeBoard* b) { m_lifeBoard = b; };

    /**
     * Returns the score board
     *
     * @return scoreBoard
     */
    ScoreBoard* getScoreBoard() { return m_scoreBoard; };

    /**
     * Returns the life board
     *
     * @return lifeBoard
     */
    LifeBoard* getLifeBoard() { return m_lifeBoard; };

    /**
     * Returns the mixer
     * 
     * @return m_mixer
    **/

    Mixer* getMixer() { return m_mixer; };

    /**
     * Turns camera movement on or off
     *
     * @param freeze
     */
    void setCameraFreeze(bool freeze) { m_cameraFreeze = freeze; };

    /**
     * Returns whether camera movement is on or off
     *
     * @return cameraFreeze
     */
    bool isCameraFreezed() const { return m_cameraFreeze; };

    /**
     * Sets level completed
     *
     * @param completed
     */
    void setLevelCompleted(bool completed);

    /**
     * Returns true, if the level is completed otherwise false
     *
     * @return levelCompleted
     */
    bool isLevelCompleted() const { return m_levelCompleted; };

    /**
     * Sets game ended and save highscore if ended
     *
     * @param ended
     */
    void setEnded(bool ended);

    /**
     * Sets, if the Game should be closed directly without entering the main Menu
     *
     * @param forceQuit
     */
    void setForceQuit(bool forceQuit);

    /**
     * Returns true, if the level ended otherwise false
     *
     * @return ended
     */
    bool isEnded() const { return m_ended; };

    /**
     * Returns true, if the Game needs to be closed directly
     *
     * @return forceQuit
     */
    bool forceQuit() const { return m_forceQuit; };

    /**
     * Method to evolve the player
     */
    void evolve();

    /**
     * Method to block damage on player while invincible
     */
    void checkInvincible();

private:

    /**
     * Resets level attributes for next level
     */
    void clearGame();

    /**
     * Sets current level name
     *
     * @param levelname
     */
    void setCurrentLevelName(string levelname) { m_currentLevelName = levelname; };

    /**
     * Reads player properties from xml file
     */
    void getPlayerProperty(const boost::property_tree::ptree::value_type& v, PlayerProperty& p);

    /**
     * Updates camera position dependent on time step
     *
     * @param dt    time step since last update
     */
    void updateCameraPosition(float dt);

    /**
     * Creates fish swarm on given position and with given fps
     *
     * @param pos_x
     * @param pos_y
     * @param fps
     */
    void createSwarm(int pos_x, int pos_y, int fps);

    /**
     * Calls move functios from all actors in game
     *
     * @param dt    time step since last update
     */
    void moveActors(float dt);

    /**
     * Removes all finished kill animations and dead mines
     */
    void checkKills();

    /**
     * Checks if player collided with bots or items and handles results
     */
    void checkPlayerCollision();

    /**
     * Checks if player collided with projectiles and handles results
     */
    void checkProjectileCollision();

    /**
     * Checks if player collided with camera and prevent player movement beyond the screen borders
     */
    void checkCameraCollision();

    /**
     * Checks if spawned bot is boss and freeze camera if necessary
     */
    void checkBoss();

    /**
     * Resolves player collision with upperlayer
     */
    void resolveUpperLayerCollision();

    /**
     * Removes actor from game
     *
     * @param actor
     */
    void removeActor(Actor* a);

    /**
     * Defers inactive bots to active bots if they are in camera range
     */
    void botHandling(float dt);

    /**
     * Returns time since last call
     *
     * @return time
     */
    float getElapsedTime();

    /**
     * Renders all renderables in game
     */
    void render();

    /**
     * @brief checkMines goes through all mines and looks if the are triggered or ready to explode
     * @param remove a vector actors that have to be removed and to which the dead mines can be added
     * @param mine_timer the current sdl_ticks
     * @return the expended vector of actors that have to be removed
     */
    vector<Actor *> checkMines(vector<Actor *> remove, float mine_timer);

    /**
     * Saves player score if its a new highscore
     */
    void saveHighScore();

    /**
     * Prints output on the screen and waits for player input, returns a string
     *
     * @param output    String which should be printed on the screen
     *
     * @return input    Player input as string
     */
    string textInput(string output);

    // Lifepoints when evolving
    int                     m_leben;

    /**
     * @brief muschelBoostUpdate    checks if the shell is open or not and creates or delets the item offered by the shell
     * @param remove                a vector of all the actors that will be removed afterwards and to which a to rmoving item can be added
     * @param bot                   the shell it self
     * @return                      the vector of actors to remove, which is expanded by the item of the shell, if the shell is now closed
     */
    set<Actor*> muschelBoostUpdate(set<Actor*> remove, Bot* bot);

   // All bots in scope
    vector<Bot*>            m_bots;

    // All m_anim in scope
    vector<KillAnimation*>  m_anim;

    // All Items
    vector<Item*>           m_items;

    // All bots not in scope
    vector<Bot*>            m_inactiveBots;

    // all mines
    vector<Bot_Mine*>       m_mines;

    // All projectiles
    vector<Projectile*>     m_projectiles;

    // All renderables in the game
    vector<Renderable*>     m_renderables;

    // All actors
    vector<Actor*>          m_actors;

    // All rendereables which should be deleted in destructor
    // all actors are added in addActor except Player which are handled separately, so are Killanimations which are not allocated
    // the remaining allocations are also handled separatly while being created
    vector<Renderable*>     m_delete;

    // The user controlled player
    Player*                 m_player;

    // The current level
    Level*                  m_level;

    // A Layer
    TexturedLayer*          m_layer;

    // A score board
    ScoreBoard*             m_scoreBoard;

    // A life board
    LifeBoard*              m_lifeBoard;

    // Pointer to the main window of the game
    SDL_Renderer*           m_renderer;

    // SDL Ticks since start
    Uint32                  m_startTicks;

    // Health points when collecting lightning
    int                     m_lebenLightning;

    // Time steps
    float                   m_dt;

    float                   m_ivDt;

    float                   m_lastInv;

    // Window width
    int                     m_windowWidth;

    // Window height
    int                     m_windowHeight;

    bool                    m_started;

    // SDL_Ticks of the last time the player shooted
    unsigned int            m_lastTime;

    // SDL Ticks now
    unsigned int            m_currentTime;

    unsigned int            m_currentScore;

    bool                    m_cameraFreeze;

    bool                    m_ended;

    bool                    m_levelCompleted;

    bool                    m_forceQuit;

    string                  m_nextLevelName;

    string                  m_currentLevelName;

    MainWindow*             m_window;

    Mixer*                  m_mixer;

    int                     m_bossCounter;

    int                     m_bossRendering;

    string                  m_botfile;

    // The other player, our current player will evolve to
    Player*                 m_evolve;
};

} /* namespace jumper */

#endif /* SRC_GAME_HPP_ */
