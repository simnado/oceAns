/*
 * MainWindow.hpp
 *
 *  Created on: Nov 13, 2015
 *      Author: twiemann
 *      Author: ledelkott
 *      Author: tstahn
 */

#ifndef SRC_MAINWINDOW_HPP_
#define SRC_MAINWINDOW_HPP_

#include <string>
#include <SDL2/SDL.h>

#include "Game.hpp"
#include "WorldProperty.hpp"

namespace jumper
{

class Game;
class Menu;

/**
 *  @brief Represents the main window of the game.
 */
class MainWindow
{
public:

    /**
     * Creates a main window with given \ref title, width \ref w and height \ref h
     *
     * @param title     Title of the window
     * @param w         Width
     * @param h         Height
     */
    MainWindow(std::string title, int w, int h);

    /**
    * The Destructor
    */
    ~MainWindow();

     /**
      * Handles user events and renders the current level, includes the main Loop
      *
      * @return an boolean which informs whether the game should be closed immediately
      */
    bool run();

    /**
     * loads an image and renders it to the screen
     *
     * @param file      the path from which the background_image should be loaded
     * *return          an SDL_Texture of the image with alpha_mode activated
     */
    SDL_Texture* loadBackground(std::string file);

    /**
     * returns the associated Menu
     */
    Menu* getMenu();

    /**
     * returns the current Renderer
     */
    SDL_Renderer*   getRenderer();
    /**
     * returns the associated Window
     */
    SDL_Window*     getWindow();

    /**
     * set the associated Game with this window
     * @param game      the Game which will be set
     */
    void setGame(Game* game);

    /**
     * Toggles fullscreen
     */
    void setFullscreen();

    /**
     * opens the SettingsMenu()
     */
    void openSettingsMenu();

    /**
     * opens an image and wait for user input to blend it out again, to create a smooth switchover
     *
     * @param path      the image which will be used as background
     */
    void openWaitingScreen(std::string path);

    /**
     * Returns the window width
     *
     * @return          the window width
     */
    int w();

    /**
     * Returns the window height
     *
     * @return          the window height
     */
    int h();

    /**
     * Quits SDL and frees all resources
     */
    void quitSDL();

private:


    /**
     * Initializes all needed SDL resources
     */
    void initSDL();

    /// SDL renderer struct
    SDL_Renderer*       m_renderer;

    /// SDL main window struct
    SDL_Window*         m_window;

    /// Window width
    int                 m_width;

    /// Window height
    int                 m_height;
    // The Game which is associated with the Window
    Game*               m_game;
    // The Menu associated with this window
    Menu*               m_menu;
    // Boolean which determines the fullscreen option
    bool                m_fullscreen;

};

} /* namespace jumper */

#endif /* SRC_MAINWINDOW_HPP_ */
