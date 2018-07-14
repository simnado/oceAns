/*
 * Game.cpp
 *
 *  Created on: Dec 11, 2015
 *      Author: twiemann
 *      Author: tstahn
 *      Author: ledelkott
 */

#include "Game.hpp"

#include "Player.hpp"
#include "KillAnimation.hpp"

#include "Boss_Roboterfisch.hpp"
#include "Boss_Tentakel.hpp"
#include "Bot_Aal.hpp"
#include "Bot_Hai.hpp"
#include "Bot_Kugelfisch.hpp"
#include "Bot_mov_Boot.hpp"
#include "Bot_Qualle.hpp"
#include "Bot_Schlange.hpp"
#include "Bot_Schlange_klein.hpp"
#include "Bot_Schwertfisch.hpp"
#include "Bot_Schwarm.hpp"
#include "Bot_Seeigel.hpp"
#include "Bot_Seepferdchen.hpp"
#include "Bot_Krabbe.hpp"
#include "Bot_Meerjungfrau.hpp"
#include "Bot_Muschel.hpp"
#include "Bot_stat_Boot.hpp"
#include "Bot_Seestern.hpp"
#include "Menu.hpp"

#include "LifeBarBoss.hpp"


#include <typeinfo>
#include <iostream>
#include <algorithm>

#include <SDL2/SDL_ttf.h>

using std::set;
using std::cout;
using std::endl;

namespace jumper
{

Game::Game(MainWindow* mainWindow)
{
    m_player = 0;
    m_level = 0;
    m_layer = 0;
    m_renderer = mainWindow->getRenderer();
    m_scoreBoard = 0;
    m_currentScore = 0;
    m_lifeBoard = 0;
    m_window = mainWindow;
    m_windowWidth = mainWindow->w();
    m_windowHeight = mainWindow->h();
    m_lastTime = 0;
    m_currentTime = 0;
    m_started = false;
    m_ended = false;
    m_forceQuit = false;
    m_startTicks = 0;
    m_dt = 0;
    m_currentLevelName = "";
    m_nextLevelName = "";
    m_mixer = new Mixer();
    setLevelCompleted(false);
    setCameraFreeze(false);

    m_bossRendering = 0;
    m_bossCounter = 0;
}

Game::~Game()
{
    delete m_mixer;
    for (auto it: m_delete)
        delete it;
    m_delete.clear();
}

void Game::clearGame()
{
    m_renderables.clear();
    m_actors.clear();
    m_bots.clear();
    m_projectiles.clear();
    m_inactiveBots.clear();
    m_anim.clear();
    m_items.clear();
    m_player = 0;
    m_level = 0;
    m_layer = 0;
    m_scoreBoard = 0;
    m_lifeBoard = 0;
    m_dt = 0;
    m_ivDt = 0;
    m_lastInv= SDL_GetTicks();
    m_lebenLightning = 0;
    m_bossCounter =0;
    m_bossRendering=0;
    m_nextLevelName = "";
    setLevelCompleted(false);
    setCameraFreeze(false);

    // Prevent time lap
    getElapsedTime();

    if (Camera* camera = &Renderable::m_camera)
    {
        Vector2i vec = Vector2i(0,0);
        camera->move(vec);
    }
}

void Game::getPlayerProperty(const boost::property_tree::ptree::value_type& v, PlayerProperty& p)
{
    int pos_x = v.second.get<int>("positionX", 0);
    int pos_y = v.second.get<int>("positionY", 0);
    float jumpForceX = v.second.get<float>("jumpForceX", 0.0);
    float jumpForceY = v.second.get<float>("jumpForceY", 0.0);
    float moveForceX = v.second.get<float>("moveForceX", 0.0);
    float moveForceY = v.second.get<float>("moveForceY", 0.0);
    float maxVelRun = v.second.get<float>("maxVelRun", 0.0);
    float maxVelFall = v.second.get<float>("maxVelFall", 0.0);
    float maxJumpHeight = v.second.get<float>("maxJumpHeight", 0.0);
    p.setPosition(Vector2f(pos_x, pos_y));
    p.setJumpForce(Vector2f(jumpForceX, jumpForceY));
    p.setMoveForce(Vector2f(moveForceX, moveForceY));
    p.setMaxFallVelocity(maxVelFall);
    p.setMaxRunVelocity(maxVelRun);
    p.setMaxJumpHeight(maxJumpHeight);
}



void Game::setupGame(string filename)
{
    m_botfile="..\\res\\Bots.xml";
    m_ivDt = 0;
    m_lastInv= SDL_GetTicks();
    clearGame();
    setCurrentLevelName(filename);
    std::size_t found = filename.find_last_of("/\\");
    string path = filename.substr(0,found);
    using boost::property_tree::ptree;
    ptree pt;
    // read the xml file and set the Level, Player, GameParameters,Actors, etc.
    read_xml(filename, pt);
    BOOST_FOREACH(const ptree::value_type&  v, pt.get_child("level") )
    {

        if( v.first == "bot" || v.first == "player" || v.first == "stone" || v.first == "heart" ||
                v.first == "lightning" || v.first == "coin" || v.first == "ending"|| v.first == "boss" || v.first == "evolve")
        {
            // Get frame definitions
            string filename = v.second.get("<xmlattr>.filename", "");
            string sprite = v.second.get("<xmlattr>.sprite", "..\\res\\Data\\Default_Bots\\Blumenkohlfisch.spr");
            int numFrames = v.second.get<int>("numFrames", 0);
            int pos_x = v.second.get<int>("positionX", 0);
            int pos_y = v.second.get<int>("positionY", 0);
            int frameWidth = v.second.get<int>("frameWidth", 0);
            int frameHeight = v.second.get<int>("frameHeight", 0);
            int fps = v.second.get<int>("fps", 14);
            int type = v.second.get<int>("type", -1);

            Actor* actor = 0;
            SDL_Texture* texture = 0;

            // Block for boss
            if (v.first == "boss")
            {
                // Increase  Boss counter
                m_bossCounter++;
                Bot* boss = 0;
                // Bestimme den Typ des Bosses und weise ihm die korrekten                          Daten zu
                switch (type)
                {
                //Roboterfisch
                case 0:
                    boss = new Boss_Roboterfisch(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Bosse\\Roboterfisch.spr", m_mixer);
                    break;
                    //Tentakel
                case 1:
                    boss = new Boss_Tentakel(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Bosse\\Tentakel.spr", m_mixer);
                    break;

                }
                // sets the rotation Point for the Boss
                boss->setRotationPoint(Vector2f(pos_x,pos_y));
                // Add boss to list
                actor = boss;
                addActor(boss);
                // Set Type
                actor->setType(ENEMY);
                // Set FPS
                actor->setFPS(fps);

            }

            else if (v.first == "bot")
            {


                Bot* bot = 0;
                // Bestimme den Typ des Bots und weise ihm die korrekten                          Daten zu
                switch (type)
                {

                // Aal
                case 0:
                    bot = new Bot_Aal(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Aal.spr", m_mixer);
                    break;

                    // Hai
                case 1:
                    bot = new Bot_Hai(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Hai.spr", m_mixer);
                    break;

                    //Kugelfisch
                case 2:
                    bot = new Bot_Kugelfisch(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Kugelfisch.spr", m_mixer);
                    break;

                    //Mine
                case 3:
                    bot = new Bot_Mine(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Miene.spr", m_mixer);
                    break;

                    //mov_Boot
                case 4:
                    bot = new Bot_mov_Boot(m_window->getRenderer(),m_botfile,"..\\res\\Data\\mov_Boot.spr", m_mixer);
                    break;

                    //Qualle
                case 5:
                    bot = new Bot_Qualle(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Qualle.spr", m_mixer);
                    break;

                    //Schlange
                case 6:
                    bot = new Bot_Schlange(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Schlange.spr", m_mixer);
                    break;

                    //Schwertfisch
                case 7:
                    bot = new Bot_Schwertfisch(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Schwertfisch.spr", m_mixer);
                    break;

                    //Seeigel
                case 8:
                    bot = new Bot_Seeigel(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Seeigel.spr", m_mixer);
                    break;

                    //Seepferd
                case 9:
                    bot = new Bot_Seepferdchen(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Seepferd.spr", m_mixer);
                    break;

                    //stat_Boot
                case 10:
                    bot = new Bot_stat_Boot(m_window->getRenderer(),m_botfile,"..\\res\\Data\\stat_Boot.spr", m_mixer);
                    break;

                    //Meerjungfrau
                case 11:
                    bot = new Bot_Meerjungfrau(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Meerjungfrau.spr", m_mixer);
                    break;

                    //Muschel
                case 12:
                    bot = new Bot_Muschel(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Muschel.spr", m_mixer);
                    break;

                    //Seestern
                case 13:
                    bot = new Bot_Seestern(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Seestern.spr", m_mixer);
                    break;
                    //Schlange_klein
                case 14:
                    bot = new Bot_Schlange_klein(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Schlange_klein.spr", m_mixer);
                    break;
                    //Fischschwarm
                case 15:
                    bot = new Bot_Schwarm(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Schwarm.spr", m_mixer);
                    createSwarm(pos_x, pos_y, fps);
                    break;
                    //Krabbe
                case 16:
                    bot = new Bot_Krabbe(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Krabbe.spr", m_mixer);
                    break;
                    //Default-Bots
                default:
                    bot = new Bot(m_window->getRenderer(),m_botfile,sprite,"default", m_mixer);
                    break;
                }
                bot->setRotationPoint(Vector2f(pos_x,pos_y));
                actor = bot;
                addActor(bot);
                actor->setType(ENEMY);
                actor->setFPS(fps);

            }
            // Get texture
            else
            {
                texture = TextureFactory::instance(m_window->getRenderer()).getTexture(path + "\\" + filename);
            }

            if (v.first == "player") {
                Player *player = new Player(m_window->getRenderer(), texture, frameWidth, frameHeight, numFrames, m_mixer);
                m_delete.push_back(player);
                setPlayer(player);
                player->setFocus(true);
                actor = player;
                actor->setType(PLAYER);
            }
            else if (v.first == "stone") {

                // In der XML übergebener Entwicklungsstein wird gesetzt.

                Item *item = new Item(m_window->getRenderer(), texture, frameWidth, frameHeight, numFrames, m_mixer);
                actor = item;
                addActor(item);
                item->setType(ITEM);
                item->setItemType(STONE);
            }
            else if (v.first == "heart") {
                Item *item = new Item(m_window->getRenderer(), texture, frameWidth, frameHeight, numFrames, m_mixer);
                actor = item;
                addActor(item);
                item->setType(ITEM);
                item->setItemType(HEART);

                /*try
                {
				   ptree sounds = v.second.get_child("sounds");
				   for(auto it: sounds)
				   {
					   getMixer()->assign(item, it.first, it.second.get("<xmlattr>.filename", ""));
				   }

                }
                catch(boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::property_tree::ptree_bad_path> > e)
                {
                }*/

            }
            else if (v.first == "lightning") {
                Item *item = new Item(m_window->getRenderer(), texture, frameWidth, frameHeight, numFrames, m_mixer);
                actor = item;
                addActor(item);
                item->setType(ITEM);
                item->setItemType(LIGHTNING);
            }
            else if (v.first == "coin") {
                Item *item = new Item(m_window->getRenderer(), texture, frameWidth, frameHeight, numFrames, m_mixer);
                actor = item;
                addActor(item);
                item->setType(ITEM);
                item->setItemType(COIN);
            }
            else if (v.first == "ending") {
                Item *item = new Item(m_window->getRenderer(), texture, frameWidth, frameHeight, numFrames, m_mixer);
                actor = item;
                addActor(item);
                item->setType(ITEM);
                item->setItemType(ENDING);
            }
            else if (v.first == "evolve") {

                m_evolve = new Player(m_window->getRenderer(), texture, frameWidth, frameHeight, numFrames, m_mixer);
                m_evolve->setEvolved(true);

                m_delete.push_back(m_evolve);
                actor = m_evolve;
            }
            // Setup actor properties
            if(actor)
            {
                PlayerProperty p;
                getPlayerProperty(v, p);
                actor->setPhysics(p);
                actor->setFPS(fps);


                // Assign actor sounds
                try{
                    ptree sounds = v.second.get_child("sounds");
                    for(auto it: sounds)

                    {
                        m_mixer->assign(actor, it.first, path + it.second.get("<xmlattr>.filename", ""));
                    }
                }
                catch(boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::property_tree::ptree_bad_path> > e)
                {

                }

            }
        }

        if( v.first == "tileset")
        {
            string filename = v.second.get("<xmlattr>.filename", "");
            Level* level = new Level(m_window->getRenderer(), path + "\\" + filename);
            setLevel(level);
        }

        if (v.first == "sounds")
        {
            // Assign level sounds
            try{
                for(auto it: v.second)
                {
                    m_mixer->setScore(m_level, it.first, path + it.second.get("<xmlattr>.filename", ""));
                }
            }
            catch(...)
            {
                //TODO: Exception präzisieren, Reagieren
            }
        }

        if( v.first == "upperlayer")
        {
            string filename = v.second.get("<xmlattr>.filename", "");
            // check whether upperlayer is not defined
            if (filename.find(".png") != string::npos) {
                SDL_Texture *texture = TextureFactory::instance(m_window->getRenderer()).getTexture(
                            path + "\\" + filename);
                float s = v.second.get<float>("scrollSpeed", 1.0);

                if (getLayer()) {
                    getLayer()->setUpperTexture(texture);
                    getLayer()->setUpperScrollSpeed(s);
                }
                else {
                    TexturedLayer *layer = new TexturedLayer(m_window->getRenderer());
                    m_delete.push_back(layer);
                    layer->setUpperTexture(texture);
                    layer->setUpperScrollSpeed(s);
                    setLayer(layer);
                }
            }
        }
        if( v.first == "layer")
        {
            string filename = v.second.get("<xmlattr>.filename", "");
            SDL_Texture* texture = TextureFactory::instance(m_window->getRenderer()).getTexture(path + "\\" + filename);
            float s = v.second.get<float>("scrollSpeed", 1.0);

            if (getLayer())
            {
                getLayer()->setTexture(texture);
                getLayer()->setScrollSpeed(s);
            }
            else
            {
                TexturedLayer* layer = new TexturedLayer(m_window->getRenderer());
                m_delete.push_back(layer);
                layer->setTexture(texture);
                layer->setScrollSpeed(s);
                setLayer(layer);
            }
        }
        if( v.first == "scoreBoard")
        {
            string filename = v.second.get("<xmlattr>.filename", "");
            SDL_Texture* texture = TextureFactory::instance(m_window->getRenderer()).getTexture(path + "\\" + filename);
            int x = v.second.get<int>("xPos", 10);
            int y = v.second.get<int>("yPos", 10);
            int dw = v.second.get<int>("digitWidth", 10);
            int dh = v.second.get<int>("digitHeight", 10);
            ScoreBoard* board = new ScoreBoard(m_window->getRenderer(), texture, dw, dh);
            m_delete.push_back(board);
            board->setPosition(Vector2i(x,y));
            board->setScore(m_currentScore);
            setScoreBoard(board);
        }
        if( v.first == "lifeBoard")
        {
            string filename = v.second.get("<xmlattr>.filename", "");
            SDL_Texture* texture = TextureFactory::instance(m_window->getRenderer()).getTexture(path + "\\" + filename);
            int x = v.second.get<int>("xPos", 20);
            int y = v.second.get<int>("yPos", 20);
            int dw = v.second.get<int>("digitWidth", 10);
            int dh = v.second.get<int>("digitHeight", 10);
            LifeBoard* board = new LifeBoard(m_window->getRenderer(), texture, dw, dh);
            m_delete.push_back(board);
            board->setPosition(Vector2i(x,y));
            setLifeBoard(board);
        }
        if( v.first == "nextlevel")
        {
            string filename = v.second.get("<xmlattr>.filename", "");
            setNextLevelName(path + "\\" + filename);
        }

        if (!m_inactiveBots.empty())
        {
            for(auto it: m_inactiveBots)
            {
                it->setPlayer(m_player);
            }
        }

    }
    m_mixer->playScore(m_level, "soundtrack");
}


void Game::createEnding(Bot* bot)
{
    // Create Texture
    SDL_Texture* texture = TextureFactory::instance(m_window->getRenderer()).getTexture("..\\res\\crystal.png");
    // Sets the Item
    Item* newItem = new Item(m_window->getRenderer(), texture, 48, 48, 8, m_mixer);
    // Sets the position at the bots location
    newItem->setPosition(Vector2f((bot)->position().x() + 5, (bot)->position().y() + 15));
    // Add Actors
    addActor(newItem);
    // Set type
    newItem->setType(ITEM);
    // Set ItemType
    newItem->setItemType(ENDING);
    // Set FPS
    newItem->setFPS(5);
}

void Game::createSwarm(int pos_x, int pos_y, int fps)
{
    // Bestimme Größe der Arrays
    int x_size =(int)(2+((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)))*5);
    int y_size =(int)(2+((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)))*5);

    // Arrays für die Positionierung der einzelnden Fische
    float x_add [x_size];
    float y_add [y_size];

    // Fülle restliche Werte
    for (int i = 0; i < x_size; i++)
    {
        // Setze Werte
        x_add [i] = 18 * i;//18
    }
    for (int j = 5; j < y_size; j++)
    {
        // Setze Werte
        y_add [j] = 19 * j;//19
    }
    // Alle Positionen durchgehen
    for (int i = 0; i < x_size; i++)
    {
        for (int j = 0; j < y_size; j++)
        {
            // Wird immer gesetzt
            if (!(i == 0 && j == 0))
            {
                // Zufallszahl bestimmt, ob Fisch gesetzt wird
                if (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) < 0.65)
                {
                    // Zufallszahl um "Gitterschwärme zu vermeiden
                    float variation =(( static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
                    if (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) > 0.5)
                        variation = variation * -1;
                    // Speichere Offsets
                    float x_offset= x_add [i] + 30 * variation;
                    float y_offset= y_add [j] + 30 * variation;

                    // Neuer Bot wird erstellt und gerendert
                    Bot* bot_temp = new Bot_Schwarm(m_window->getRenderer(),m_botfile,"..\\res\\Data\\Schwarm.spr", m_mixer);
                    bot_temp->setRotationPoint(Vector2f(pos_x+x_offset,pos_y+y_offset));
                    Actor* actor_temp = bot_temp;
                    addActor(bot_temp);
                    actor_temp->setType(ENEMY);
                    PlayerProperty p;
                    p.setPosition(Vector2f(pos_x+x_offset, pos_y+y_offset));
                    actor_temp->setPhysics(p);
                    actor_temp->setFPS(fps);
                }
            }
        }
    }
}

void Game::setPlayer(Player* player)
{
    m_player = player;
    addActor(m_player);
    m_player->setGame(this);
    m_player->setMixer(m_mixer);


    if(!m_bots.empty())
    {
        for(auto it: m_bots)
            it->setPlayer(m_player);
    }

    if(!m_inactiveBots.empty())
    {
        for(auto it: m_inactiveBots)
            it->setPlayer(m_player);
    }
}

void Game::setLevel(Level* level)
{
    m_level = level;
    // should always be rendered first
    m_renderables.insert(m_renderables.begin(), level);
}

void Game::setLevelCompleted(bool completed)
{
    m_levelCompleted = completed;

    if (m_levelCompleted && getScoreBoard())
        m_currentScore = getScoreBoard()->getScore();
}

void Game::addActor(Actor* actor)
{
    bool inactive = false;
    // actor->setMixer(m_mixer);
    if(dynamic_cast<Bot*>(actor))
    {
        m_inactiveBots.push_back((Bot*)(actor));
        inactive =true;
    }
    else if(dynamic_cast<Projectile*>(actor))
    {
        m_projectiles.push_back((Projectile*)actor);
    }
    else if(dynamic_cast<KillAnimation*>(actor))
    {
        m_anim.push_back((KillAnimation*)actor);
    }
    else if(dynamic_cast<Item*>(actor))
    {
        m_items.push_back((Item*)actor);
    }
    else
    {
        m_actors.push_back(actor);
    }

    if(!inactive)
    {
        m_renderables.push_back(actor);
    }

    // All actors except KillAnimations are allocated
    // player handled separately
    if (!dynamic_cast<KillAnimation*>(actor) && !dynamic_cast<Player*>(actor))
        m_delete.push_back(actor);

}

void Game::start()
{
    m_started = true;
}

void Game::pause()
{
    if (!m_window->getMenu())
        return;

    vector<string> vec = {"Continue", "Restart Level","Settings", "Exit"};
    int result = m_window->getMenu()->showMenu(vec, 1, false);

    switch (result)
    {
    case -1:
        // if SDL_Quit
        setEnded(true);
        setForceQuit(true);
        break;
    case 0:
        break;
    case 1:
        setupGame(getCurrentLevelName());
        break;
    case 2:
        m_window->openSettingsMenu();
        SDL_SetRenderDrawColor(m_renderer, 10, 76, 124, 255);
        break;
    case 3:
        setEnded(true);
        break;
    default:
        break;
    }

    // Prevent time lap
    getElapsedTime();
}

void Game::setEnded(bool ended)
{
    if (ended)
        saveHighScore();

    m_ended = ended;
}

void Game::setForceQuit(bool forceQuit)
{
    m_forceQuit = forceQuit;
}

void Game::update(const Uint8* &currentKeyStates)
{
    if (getLevel())
    {
        // Start next level if current level is completed
        // or end game if no following game exists
        if (isLevelCompleted())
        {
            m_mixer->stopScore();
            if (getNextLevelName().find(".xml")==string::npos)
            {
            	//durchgespielt:
            	m_window->openWaitingScreen("..\\res\\nextLevel4_font.png");
                setEnded(true);
                return;
            }
            else if (getNextLevelName().find("lvl3.xml")==string::npos)
            {
                m_window->openWaitingScreen("..\\res\\nextLevel2_font.png");
                setupGame(getNextLevelName());
            }
            else {
                m_window->openWaitingScreen("..\\res\\nextLevel3_font.png");
                setupGame(getNextLevelName());
            }
        }

        // Restart level on player death
        if (getPlayer() && getPlayer()->isDead())
        {
        	getMixer()->play(getPlayer(), "death");
            saveHighScore();
            m_window->openWaitingScreen("..\\res\\nextLevel5_font.png");
            setupGame(getCurrentLevelName());
            return;
        }
    }

    if (m_started)
    {
        float dt = getElapsedTime();
        if(m_player->getInvincible()) checkInvincible();
        botHandling(dt);

        m_player->update(currentKeyStates, dt);
        resolveUpperLayerCollision();
        moveActors(dt);
        checkPlayerCollision();
        checkProjectileCollision();
        checkCameraCollision();
        checkBoss();
        checkKills();
        render();

        if(m_player->getEvolved() && m_player->getHealth() < m_leben)
        {
            evolve();
        }
        if(m_player->getLightning() && m_player->getHealth() < m_lebenLightning)
        {
            m_player->setLightning(false);
        }

        updateCameraPosition(dt);

    }
}

void Game::render()
{
    SDL_RenderClear(m_renderer);

    if (getLayer())
    {
        getLayer()->render();
    }
    if(!m_renderables.empty())
    {
        for (size_t i = 0; i < m_renderables.size(); i++)
        {
            m_renderables[i]->render();
        }
    }
    if (getScoreBoard())
        m_scoreBoard->render();

    if (getLifeBoard())
    {
        m_lifeBoard->setScore(m_player->getHealth());
        m_lifeBoard->render();
    }

    // Update screen
    SDL_RenderPresent(m_renderer);
}

void Game::removeActor(Actor* a)
{
    if(dynamic_cast<Bot*>(a))
    {
        if(!m_bots.empty())
        {
            auto it = std::find(m_bots.begin(), m_bots.end(), a);
            if (((Bot*)a)->isABoss())
            {
                m_bossRendering--;
                m_bossCounter--;
                if (isCameraFreezed()&&m_bossRendering==0)
                {
                    setCameraFreeze(false);
                }

                if(m_bossCounter ==0){
                    createEnding(((Bot*)a));
                }
            }
            m_bots.erase(it);
            if(dynamic_cast<Bot_Mine*>(a))
            {
                auto it = std::find(m_mines.begin(), m_mines.end(), a);
                m_mines.erase(it);
            }
        }
    }
    else if (dynamic_cast<Projectile*>(a))
    {
        if(!m_projectiles.empty())
        {
            auto it = std::find(m_projectiles.begin(), m_projectiles.end(), a);
            m_projectiles.erase(it);
        }
    }
    else if (dynamic_cast<KillAnimation*>(a))
    {
        if(!m_anim.empty())
        {
            auto it = std::find(m_anim.begin(), m_anim.end(), a);
            m_anim.erase(it);
        }
    }
    else if (dynamic_cast<Item*>(a))
    {
        if(!m_items.empty())
        {

            auto it = std::find(m_items.begin(), m_items.end(), a);
            m_items.erase(it);
        }
    }
    else
    {
        if(!m_actors.empty())
        {
            auto it = std::find(m_actors.begin(), m_actors.end(), a);
            m_actors.erase(it);
        }
    }

    if(!m_renderables.empty())
    {
        auto it2 = std::find(m_renderables.begin(), m_renderables.end(), a);
        m_renderables.erase(it2);
    }

    m_mixer->deleteFx(a);
}

void Game::botHandling(float dt)
{
    Camera* camera = &Renderable::m_camera;

    vector<Actor*> to_remove_inactive;

    if(!m_inactiveBots.empty())
    {
        for (auto it: m_inactiveBots)
        {
            if (((Bot*)it)->isABoss())
            {
                if(it->position().x() < camera->x() + camera->w() - 150)
                {
                    m_renderables.push_back(it);
                    m_bots.push_back(it);
                    to_remove_inactive.push_back(it);
                }
            }
            else
            {
                if(it->position().x() < camera->x() + camera->w() + 30)
                {
                    if(dynamic_cast<Bot_Mine*>(it))
                        m_mines.push_back((Bot_Mine*)(it));
                    m_bots.push_back(it);
                    m_renderables.push_back(it);
                    to_remove_inactive.push_back(it);
                }
            }
        }
    }

    if(!to_remove_inactive.empty())
    {
        for(auto i: to_remove_inactive)
        {
            auto it = std::find(m_inactiveBots.begin(), m_inactiveBots.end(), i);
            m_inactiveBots.erase(it);
        }
        to_remove_inactive.clear();
    }

    if(!m_bots.empty())
    {
        for (auto it: m_bots)
        {
            std::vector<Actor *> projektile = it->bot_attack(dt);
            if (!projektile.empty())
            {
                for (auto pit: projektile)
                {
                    pit->setType(BOTPROJECTILE);
                    addActor((Projectile *) pit);
                }
            }
        }
    }
}


void Game::checkBoss()
{
    Camera* camera = &Renderable::m_camera;
    for(auto it: m_bots)
    {
        if(((Bot*)it)->isABoss())
        {
            if(m_bossRendering == 0)
            {
                m_bossRendering++;
                m_renderables.push_back((Renderable*)new LifeBarBoss(m_renderer, it));
            }

            if(it->position().x() < camera->x() + camera->w() - 150)
            {
                setCameraFreeze(true);
            }
        }
    }
}

void Game::checkCameraCollision()
{
    // Set für Aktoren, die gelöscht werden sollen
    set<Actor*> to_remove;
    Actor *a;

    // Nur Falls Set nicht leer ist
    if (!m_bots.empty())
    {
        // Itteriere durch alle Bots
        for(auto it: m_bots)
        {
            // Setze Actor auf aktuelles Element
            a = it;
            // Check for self ision
            if(a != m_player)
            {
                // Falls Bot Camera Oben oder links passiert, wird er gelöscht
                if ((a->getDistanceXToCamera(*m_level)) < -50 ||(a->position().y() < 1))
                {
                    // Hinzufügen zur Löschliste
                    to_remove.insert(a);
                }
            }
        }
    }
    // Nur Falls Set nicht leer ist
    if (!m_projectiles.empty())
    {
        //Itteriere durch alle Projektile
        for (auto it: m_projectiles)
            //Setze Actor auf aktuelles Element
            a = it;
        {
            if(a != m_player)
            {
                // Falls Projektil Kamera links oder das Level oben, unten oder rechts verlässt, wird es gelöscht
                bool left = ((a->getDistanceXToCamera(*m_level)) <0);
                bool up = (a->position().y()) <10;
                bool right = a->position().x() >= (m_level->levelWidth()*m_level->tileWidth());
                bool down = a->position().y() >= (m_level->levelHeight()*m_level->tileHeight());
                if (left||up||right||down)
                {
                    // Hinzufügen zur Löschliste
                    to_remove.insert(a);
                }
            }
        }
    }

    // Grenzen für den Spieler
    int left_border =  m_level->m_camera.position().x()+1;
    int right_border = m_level->m_camera.w()+m_level->m_camera.position().x()-m_player->w()-8;
    int upper_border = 0;
    int lower_border = m_level->tileHeight()*m_level->levelHeight()-m_player->h();
    // Falls Spieler linken Rand (Kamera) verlassen will
    if(m_player->position().x() <left_border)
    {
        // Verhindere Überschreiten
        Vector2f new_position((float)(left_border),m_player->position().y());
        m_player->setPosition(new_position);
    }
    //Falls Spieler rechten Rand (Kamera) verlassen will
    if(m_player->position().x() > right_border)
    {
        // Verhindere Überschreiten
        Vector2f new_position((float)(right_border),m_player->position().y());
        m_player->setPosition(new_position);
    }
    //Falls Spieler oberen Rand (Kamera) verlassen will
    if(m_player->position().y() < upper_border)
    {
        // Verhindere Überschreiten
        Vector2f new_position(m_player->position().x(),(float)upper_border);
        m_player->setPosition(new_position);
    }
    //Falls Spieler unteren Rand (Kamera) verlassen will
    if(m_player->position().y() > lower_border)
    {
        // Verhindere Überschreiten
        Vector2f new_position(m_player->position().x(),(float)(lower_border));
        m_player->setPosition(new_position);
    }
    // Lösche zu löschende Elemente
    for(auto i: to_remove)
        removeActor(i);
}


vector<Actor*> Game::checkMines(vector<Actor*> to_remove, float mine_timer)
{
    // checke ob andere Minen bei Tod einer Mine getriggert wurden und explodieren sollen
    for (auto it: m_mines)
    {
        // Mine ist getriggert und bereit zu explodieren also soll sie sterben und ggf. andere auslösen
        if(it->istriggered() && it->timeToExplode())
        {
            dynamic_cast<Bot_Mine*>(it)->collisionMine(it, m_mines);
            to_remove.push_back(it);
            std::vector<Actor*> killanimations = it->onDeath();
            addActor((KillAnimation*)*(killanimations.begin()));
        }
        // Mine ist getriggert aber Verzögerung bis zur Explosion ist noch nicht abgelaufen
        else if(it->istriggered())
        {
            it->decTrigger(mine_timer - it->getTriggerStart());
        }
    }
    return to_remove;
}


void Game::checkPlayerCollision()
{
    set<Actor*> to_remove;
    KillAnimation* anim = 0;
    Collision c;
    Actor *a = 0;

    if(!m_bots.empty())
    {
        for(auto it: m_bots)
        {
            a = it;

            // Check for self collision
            if(a != m_player)
            {
                c = m_player->getCollision(*a);
                // If an collection with an enemy occured, check
                // who killed whom (Player only can kill enemies
                // when falling down.

                if(a->type() == ENEMY && c.type() != NONE )
                {
                    if (SDL_GetTicks()-m_lastInv>500 && dynamic_cast<Bot*>(a) && !m_player->getInvincible())
                    {
                        m_player->collideWithActor(*a);

                        SDL_Texture* texture;

                        if(!(m_player->getEvolved()))
                        {
                            texture = TextureFactory::instance(m_renderer).getTexture("..\\res\\anne1_blink.png");
                        }
                        else
                        {
                            texture = TextureFactory::instance(m_renderer).getTexture("..\\res\\anne2_klein.png");
                        }

                        m_ivDt = SDL_GetTicks();
                        m_lastInv =SDL_GetTicks();
                        m_player->setInvincible(true);
                        m_player->set_InvincibleSecs(0.5);
                        m_player->setTexture(texture);

                    }
                    else
                    {
                        if (!(dynamic_cast<Bot*>(a)))
                            m_player->collideWithActor(*a);

                        if(!(((Bot*)a)->isABoss()))
                        {
                            // wenn a eine Mine ist dann soll auf alle weiteren Minen geprüft werden, ob sie in dem triggerradius der Mine sind
                            if(dynamic_cast<Bot_Mine*>(a))
                            {
                                dynamic_cast<Bot_Mine*>(a)->collisionMine(m_player, m_mines);
                            }
                            // wenn a ein Aal ist und der Player noch nciht gestunned ist, dann soll der Player gestunned werden
                            else if(dynamic_cast<Bot_Aal*>(a) && !m_player->getStun())
                            {
                                a->collideWithActor(*m_player);
                                KillAnimation* stunAnim = new KillAnimation(a, "..\\res\\Data\\Explosionen\\blau_gross.spr", m_mixer);
                                addActor(stunAnim);
                            }
                            else
                            {
                                a->collideWithActor(*m_player);
                            }
                        }

                        // a ist bei der Kollision gestorben d.h. sollen die Todesanimationen erstellt und die Punkte berechnet werden
                        if(a->isDead())
                        {
                            to_remove.insert(a);
                            // wenn eine Schlange gestorben ist halte fest, wie is gestorben ist um ggf. Nachkommen zu erstellen (hier Tod durch Playerkollision d.h. normaler Tod)
                            if(dynamic_cast<Bot_Schlange*>(a))
                                dynamic_cast<Bot_Schlange*>(a)->setKindOfDeath(false);
                            std::vector<Actor*> killanimations = a->onDeath();
                            if(!killanimations.empty())
                                anim = (KillAnimation*)*killanimations.begin();
                            m_scoreBoard->setScore(m_scoreBoard->getScore()+((Bot*)(a))->getPoints());
                        }
                    }
                }
            }
        }
    }

    if(anim)
        addActor(anim);


    if (!m_items.empty())
    {
        for(auto it: m_items)
        {
            a = it;
            c = m_player->getCollision(*a);
            // Simple items can be collected on the fly
            // Different Itemtyps
            if (a->type() == ITEM && c.type() != NONE && ((Item*)a)->getItemType() == STONE)
            {
                if(!m_player->getEvolved())
                    evolve();
                m_scoreBoard->setScore(m_scoreBoard->getScore()+1000);
                to_remove.insert(a);
                addActor((KillAnimation*)*((a->onDeath()).begin()));
            }
            else if(a->type() == ITEM && c.type() != NONE && ((Item*)a)->getItemType() == HEART)
            {
                m_player->setHealth(m_player->getHealth()+10);
                // Spieler erhält 1000 Score Punkte.
                m_scoreBoard->setScore(m_scoreBoard->getScore()+1000);
                to_remove.insert(a);
                addActor((KillAnimation*)*((a->onDeath()).begin()));
            }
            else if(a->type() == ITEM && c.type() != NONE && ((Item*)a)->getItemType() == LIGHTNING)
            {
                m_player->setLightning(true);
                m_lebenLightning = m_player->getHealth();
                to_remove.insert(a);
                addActor((KillAnimation*)*((a->onDeath()).begin()));
            }
            else if(a->type() == ITEM && c.type() != NONE && ((Item*)a)->getItemType() == COIN)
            {
                SDL_Texture* texture;

                if(!(m_player->getEvolved()))
                {
                    texture = TextureFactory::instance(m_renderer).getTexture("..\\res\\anne1_blink.png");
                }
                else
                {
                    texture = TextureFactory::instance(m_renderer).getTexture("..\\res\\anne2_blink.png");
                }
                // Spieler erhält 1000 Score Punkte.
                m_scoreBoard->setScore(m_scoreBoard->getScore()+1000);
                m_ivDt = SDL_GetTicks();
                m_player->setInvincible(true);
                m_player->set_InvincibleSecs(10);
                m_player->setTexture(texture);
                to_remove.insert(a);
                ((KillAnimation*)*((a->onDeath()).begin()));
            }
            else if(a->type() == ITEM && c.type() != NONE && ((Item*)a)->getItemType() == ENDING)
            {
                setLevelCompleted(true);
                to_remove.insert(a);
            }
        }
    }

    // Remove actors that were killed in this loop. We have to
    // store them separately because otherwise we would corrupt
    // to loop structure

    for(auto i: to_remove)
        removeActor(i);
}

void Game::checkKills()
{
    // Vektor von Animations and Mines die gelöscht werden können
    vector<Actor*> to_remove;

    // Falls m_anim nicht leer ist
    if(!m_anim.empty())
    {
        // Iterriere durch
        for (auto it: m_anim)
        {
            // Prüft ob Animation durchgelaufen ist
            if(it->isFinished()){
                // Fügt Animation Liste hinzu
                to_remove.push_back(it);
            }
        }
    }

    // prüfe, ob es für eine der Minen Zeit wird zu explodieren
    float mine_timer = SDL_GetTicks();
    to_remove = checkMines(to_remove, mine_timer);

    // Falls Löschvektor nicht leer ist
    if(!to_remove.empty())
    {
        // Iteriere durch
        for(auto it: to_remove)
            // Lösche Element
            removeActor(it);
    }
    // Clear Vektro
    to_remove.clear();
}


void Game::checkProjectileCollision()
{
    std::vector<Actor*> to_add;
    set<Actor *> to_remove;
    KillAnimation *anim = 0;
    Collision c;
    if (!m_projectiles.empty())
    {
        for (auto it: m_projectiles)
        {
            Actor *projectile = it;

            std::vector<Actor*> killanimations = projectile->onDeath();
            //Kollision mit Level
            c = m_level->resolveCollision(it);

            if(c.type() != NONE)
            {
                to_remove.insert(projectile);
                anim = (KillAnimation*)*killanimations.begin();
            }

            //Kollision von Botprojektilen mit Player
            if (c.type() == NONE)
            {
                c = m_player->getCollision(*projectile);
                if(projectile->type() == BOTPROJECTILE && c.type()!=NONE)
                {
                    if (SDL_GetTicks()-m_lastInv>500&& !m_player->getInvincible())
                    {
                        m_player->collideWithActor(*projectile);

                        anim = (KillAnimation*)*killanimations.begin();
                        to_remove.insert(projectile);
                        SDL_Texture* texture;

                        if(!(m_player->getEvolved()))
                        {
                            texture = TextureFactory::instance(m_renderer).getTexture("..\\res\\anne1_blink.png");
                        }
                        else
                        {
                            texture = TextureFactory::instance(m_renderer).getTexture("..\\res\\anne2_klein.png");
                        }
                        m_ivDt = SDL_GetTicks();
                        m_lastInv =SDL_GetTicks();
                        m_player->setInvincible(true);
                        m_player->set_InvincibleSecs(0.5);
                        m_player->setTexture(texture);
                        if(! getPlayer()->isDead()){
                        	m_mixer->play(m_player, "hit");
                        }
                    }
                }
            }
            if (anim)
                addActor(anim);

            //ansonsten alle Bots durchlaufen
            if(c.type() == NONE)
            {
                for (auto it2: m_bots)
                {
                    Actor *bot = it2;
                    c = projectile->getCollision(*bot);

                    if (projectile->type() == PLAYERPROJECTILE && c.type() != NONE)
                    {
                        // wenn eine Mine abgeschossen wird prüfe auf ihr Umfeld um ggf. weitere Minen auszulösen
                        if(dynamic_cast<Bot_Mine*>(bot))
                        {
                            dynamic_cast<Bot_Mine*>(bot)->collisionMine(projectile, m_mines);
                            anim = (KillAnimation*)*killanimations.begin();
                        }
                        else
                        {
                            bot->collideWithActor(*projectile);
                            anim = (KillAnimation*)*killanimations.begin();
                        }

                        to_remove.insert(projectile);
                        if (bot->isDead())
                        {
                            to_remove.insert(bot);
                            m_scoreBoard->setScore(m_scoreBoard->getScore()+((Bot*)(bot))->getPoints());

                            // wenn bot eine gestorbene Schlange ist, setzte die Todesart auf true da sie durch ein Projektil gestorben ist und dann Nachkommen hinterlässt
                            if(dynamic_cast<Bot_Schlange*>(bot))
                                dynamic_cast<Bot_Schlange*>(bot)->setKindOfDeath(true);

                            std::vector<Actor*> killanimations = bot->onDeath();
                            // wenn der Tod nur eine Todesanimation hinterlässt
                            if(killanimations.size() == 1)
                            {
                                addActor((KillAnimation *)*(killanimations.begin()));
                            }
                            // wenn der Tod mehrere Animationen hinterlässt, wie z.B. bei der Schlange
                            else if (killanimations.size() > 1)
                            {
                                for (auto kit: killanimations)
                                {
                                    to_add.push_back(kit);
                                    (kit)->setType(ENEMY);
                                }
                            }
                        }
                    }
                }
            }
            if (anim)
                addActor(anim);
        }
    }

    for(auto i: to_remove)
        removeActor(i);

    for(auto i: to_add)
        addActor(i);
}

// je nach Zustandsänderung der Muschel wird ein neues Item erstellt oder das vorhandene gelöscht
set<Actor*> Game::muschelBoostUpdate(set<Actor*> remove, Bot* bot)
{
    Bot_Muschel* muschel = dynamic_cast<Bot_Muschel*>(bot);

    // wenn die Muschel sich gerade öffnet
    if(muschel->getOpening())
    {
        // erstelle ein neues Item
        SDL_Texture* texture = TextureFactory::instance(m_window->getRenderer()).getTexture("..\\res\\stone.png");
        Item* newItem = new Item(m_window->getRenderer(), texture, 24, 24, 1, m_mixer);
        newItem->setPosition(Vector2f((bot)->position().x() + 5, (bot)->position().y() + 15));
        addActor(newItem);
        newItem->setType(ITEM);
        newItem->setItemType(STONE);
        newItem->setFPS(5);

        //setze das Item als Boost der Muschel
        muschel->setBoost(newItem);
    }

    // wenn die Muschel sich gerade schließt entferne den Boost der Muschel
    else if(muschel->getClosing())
    {
        auto it = std::find(m_items.begin(), m_items.end(), muschel->getBoost());
        remove.insert(*it);
    }

    return remove;
}

void Game::moveActors(float dt)
{
    set<Actor*> to_remove;
    for (auto it: m_bots)
    {
        it->move(*m_level, dt);
        // checke, ob der Zustand der Muschel sich geändert hat und entferne ggf. die Items der einzelnen Muscheln
        if(dynamic_cast<Bot_Muschel*>(it))
            to_remove = muschelBoostUpdate(to_remove, it);
    }
    for (auto it: m_actors)
        it->move(*m_level, dt);
    for (auto it: m_projectiles)
        it->move(*m_level, dt);
    for (auto it: m_items)
        it->move(*m_level, dt);

    if(!to_remove.empty())
    {
        for(auto i: to_remove)
            removeActor(i);
    }
}

void Game::updateCameraPosition(float dt)
{
    m_dt += dt;

    if (Camera* camera = &Renderable::m_camera)
    {
        // Move camera in y direction, dependent on player position
        if ((m_player->position().y() > m_windowHeight / 2) &&
                (m_player->position().y() < ((m_level->levelHeight() * m_level->tileHeight()) - m_windowHeight / 2)))
        {
            Vector2i position(camera->x(), m_player->position().y() - m_windowHeight / 2);
            camera->move(position);
        }

        // Stop if camera is freezed
        if (isCameraFreezed())
            return;

        // Freeze camera on level end
        if (camera->x() + camera->w() >= m_level->levelWidth() * m_level->tileWidth())
        {
            setCameraFreeze(true);
            return;
        }

        // Move camera in x direction
        if (m_dt >= 0.02)
        {
            Vector2i position(camera->x() + 1, camera->y());
            camera->move(position);
            m_player->setPosition(m_player->position()+ Vector2f(1,0));
            m_dt = 0;
        }
    }
}

void Game::resolveUpperLayerCollision()
{
    if (!getPlayer() || !getLayer() || !getLayer()->getUpperTextureHeight())
        return;

    Vector2f desiredPosition;
    desiredPosition = getPlayer()->position();

    if (desiredPosition.y() + getPlayer()->h() / 4 < getLayer()->getUpperTextureHeight()) // +2 ggf entfernen
        desiredPosition.setY(desiredPosition.y() + 1);

    getPlayer()->setPosition(Vector2f(desiredPosition.x(), desiredPosition.y()));
}



void Game::evolve()
{
    Player* tmp;
    SDL_Texture* texture;
    SDL_Texture* textureOld;

    // Übertrage Informationen über aktuellen Spieler an den folgenden.
    m_evolve->setHealth(m_player->getHealth());
    m_evolve->setPosition(m_player->position());
    m_evolve->setLightning(m_player->getLightning());
    m_evolve->setInvincible(m_player->getInvincible());
    m_evolve->set_InvincibleSecs(m_player->getInvincibleSecs());
    m_evolve->setPowerShoot(m_player->getPowerShoot(),m_player->getPowerShootTimer());

    // Speichere alten Spieler für Zurückentwicklung.
    tmp = m_player;

    // Entferne alten Spieler und setze neuen.
    removeActor(m_player);
    m_evolve->setType(PLAYER);
    setPlayer(m_evolve);


    // Lade blinkende Texture falls invincible
    if(m_player->getInvincible())
    {
        if(m_player->getEvolved())
        {
            texture = TextureFactory::instance(m_renderer).getTexture("..\\res\\anne2_blink.png");
            textureOld = TextureFactory::instance(m_renderer).getTexture("..\\res\\anne1_klein.png");
            tmp->setTexture(textureOld);

        }
        else
        {
            texture = TextureFactory::instance(m_renderer).getTexture("..\\res\\anne1_blink.png");
        }

        m_player->setTexture(texture);
    }

    tmp->setInvincible(false);
    tmp->set_InvincibleSecs(0);
    m_player->setFocus(true);
    m_evolve = tmp;
    m_leben= m_player->getHealth();

}

float Game::getElapsedTime()
{
    if (m_startTicks == 0)
    {
        m_startTicks = SDL_GetTicks();
    }
    Uint32 ticks = SDL_GetTicks();
    float time = (ticks - m_startTicks) / 1000.0;
    m_startTicks = ticks;
    return time;
}

void Game::saveHighScore()
{
    vector<std::pair<string, int> > scoreVector;

    string text = "Highscore - Enter your name:";

    using boost::property_tree::ptree;

    // try to open highscorefile
    try
    {
        ptree pt;

        read_xml(".\\highscore.xml", pt);

        BOOST_FOREACH (const ptree::value_type&  v, pt.get_child("highscores"))
        {
            if (v.first == "score")
            {
                string name = v.second.get("<xmlattr>.name", "");
                int score = v.second.get<int>("score", 0);
                scoreVector.push_back(std::make_pair(name, score));
            }
        }
    }
    catch (...) {}

    if (scoreVector.empty())
    {
        if (getScoreBoard())
            scoreVector.push_back(std::make_pair(textInput(text), getScoreBoard()->getScore()));
    }
    else
    {
        sort(scoreVector.begin(), scoreVector.end(),
             boost::bind(&std::pair<string, int>::second, _1) >
             boost::bind(&std::pair<string, int>::second, _2));

        if (getScoreBoard())
        {
            bool inHighscore = false;
            for(auto it = scoreVector.begin(); it != scoreVector.end(); it++)
            {
                if ((it)->second < getScoreBoard()->getScore() || scoreVector.size() < 10)
                {
                    inHighscore = true;
                    break;
                }
            }
            if (inHighscore) scoreVector.push_back(std::make_pair(textInput(text), getScoreBoard()->getScore()));
        }

        sort(scoreVector.begin(), scoreVector.end(),
             boost::bind(&std::pair<string, int>::second, _1) >
             boost::bind(&std::pair<string, int>::second, _2));

        while (scoreVector.size() > 10)
            scoreVector.pop_back();
    }

    // Write new XML File
    ptree newPt;

    for (auto it: scoreVector)
    {
        ptree& scoreNode = newPt.add("highscores.score", "");
        scoreNode.put("<xmlattr>.name", it.first);
        scoreNode.put("score", it.second);
    }

    boost::property_tree::xml_writer_settings<char> settings('\t', 1);
    write_xml(".\\highscore.xml", newPt, std::locale(), settings);
}

string Game::textInput(string output)
{
    // initialise the font
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    SDL_Color color = {255, 255, 255};
    TTF_Font *font = TTF_OpenFont("..\\res\\HotSweat.ttf", 25);
    SDL_Surface* outputSurface = TTF_RenderText_Solid(font, output.c_str(), color);
    SDL_Rect outputPos;
    outputPos.x = m_windowWidth / 2 - outputSurface ->clip_rect.w / 2;
    outputPos.y = 20;
    outputPos.h = outputSurface->clip_rect.h;
    outputPos.w = outputSurface->clip_rect.w;
    SDL_Texture* outputTexture = SDL_CreateTextureFromSurface(m_renderer, outputSurface);
    SDL_RenderCopy(m_renderer, outputTexture, NULL, &outputPos);
    SDL_FreeSurface(outputSurface);
    SDL_RenderPresent(m_renderer);

    string input;

    SDL_bool done = SDL_FALSE;

    SDL_StartTextInput();
    // get the textInput and render it to the screen
    while (!done)
    {
        SDL_Event event;
        if (SDL_WaitEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                done = SDL_TRUE;
                break;
            case SDL_TEXTINPUT:
                if (input.length() > 25)
                    break;
                input.append(event.text.text);
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)
                {
                    SDL_StopTextInput();
                    return input;
                }
                if (event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if (input.length() > 0)
                        input.pop_back();
                }
                break;
            }

            SDL_RenderClear(m_renderer);
            SDL_RenderCopy(m_renderer, outputTexture, NULL, &outputPos);
            SDL_Surface* inputSurface = NULL;
            if (input == "")
                inputSurface = TTF_RenderText_Solid(font, " ", color);
            else
                inputSurface = TTF_RenderText_Solid(font, input.c_str(), color);
            // render the new inputSurface to the screen at the right position
            SDL_Rect inputPos;
            inputPos.x = m_windowWidth / 2 - inputSurface ->clip_rect.w / 2;
            inputPos.y = 100;
            inputPos.h = inputSurface->clip_rect.h;
            inputPos.w = inputSurface->clip_rect.w;
            SDL_Texture* inputTexture = SDL_CreateTextureFromSurface(m_renderer, inputSurface);
            SDL_RenderCopy(m_renderer, inputTexture, NULL, &inputPos);
            SDL_FreeSurface(inputSurface);
            SDL_RenderPresent(m_renderer);
        }
    }

    SDL_StopTextInput();
    return input;
}

void Game::checkInvincible()
{
    SDL_Texture* texture;
    float dt = SDL_GetTicks();

    // player is invincible for 10 seconds
    if(dt - m_ivDt > 1000 * m_player->getInvincibleSecs())
    {
        m_player->setInvincible(false);
        m_ivDt = 0;

        // Lade und setze nicht blinkende Texturen für Garados oder Karpador
        if(m_player->getEvolved())
        {
            texture = TextureFactory::instance(m_renderer).getTexture("..\\res\\anne2_klein.png");
        }
        else
        {
            texture = TextureFactory::instance(m_renderer).getTexture("..\\res\\anne1_klein.png");
        }
        m_player->setTexture(texture);
    }
}

}/* namespace jumper */


