/*
 * MainWindow.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: twiemann
 *      Author: ledelkott
 *      Author: tstahn
 */

#include "MainWindow.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Vector.hpp"
#include "Renderable.hpp"
#include <SDL2/SDL_mixer.h>
#include "Menu.hpp"
#include <unistd.h>

namespace jumper
{


MainWindow::MainWindow(std::string title, int w, int h)
{
    /// Init width and height
    m_width = w;
    m_height = h;
    // Init fullscreen-option
    m_fullscreen = false;
    /// Set pointer to NULL
    m_renderer = 0;

    /// Init the camera for all renderables
    Renderable::m_camera.m_width = w;
    Renderable::m_camera.m_height = h;
    /// Initialize SDL stuff
    initSDL();
    m_menu = new Menu(this);
    m_game = NULL;
}

MainWindow::~MainWindow()
{
    quitSDL();
}

SDL_Renderer* MainWindow::getRenderer()
{
    return m_renderer;
}
SDL_Window* MainWindow::getWindow()
{
    return m_window;
}
Menu* MainWindow::getMenu() {
    return m_menu;
}
void MainWindow::setFullscreen() {
    m_fullscreen = !m_fullscreen;
    if (!m_fullscreen) SDL_SetWindowFullscreen(m_window,0);
    else SDL_SetWindowFullscreen(m_window,SDL_WINDOW_FULLSCREEN_DESKTOP);

}
bool MainWindow::run()
{
    bool quit = false;
    SDL_Event e;
    const Uint8* currentKeyStates;

    // Start main loop and event handling
    while(!quit && m_renderer && !m_game->isEnded())
    {
        // Process events, detect quit signal for window closing
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
                m_game->setForceQuit(true);
            }
        }

        currentKeyStates = SDL_GetKeyboardState( NULL );
        m_game->update(currentKeyStates);
    }
    return m_game->forceQuit();
}

void MainWindow::setGame(Game* game)
{
    m_game = game;
}
void MainWindow::initSDL()
{
    // Initialize SDL Video
    if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0)
    {
        std::cout << "SDL could not initialize: " << SDL_GetError() << std::endl;
        return;
    }

    // Generate SDL main window
    m_window = SDL_CreateWindow(
                "oceANS",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                m_width,
                m_height,
                SDL_WINDOW_SHOWN );

    if(m_window == NULL)
    {
        std::cout << "SDL window could not be generated: " << SDL_GetError() << std::endl;
    }
    else
    {

        // Create renderer for the SDL main window
        m_renderer = SDL_CreateRenderer( m_window, -1, SDL_RENDERER_ACCELERATED );

        if(m_renderer == NULL)
        {
            std::cout << "SDL could not generate renderer: " << SDL_GetError() << std::endl;
        }
        else {
            // Set background color for renderer
            SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
            // set logicalSize in order to scale in Fullscreen
            SDL_RenderSetLogicalSize(m_renderer, m_width, m_height);
            SDL_RenderClear(m_renderer);
        }
    }

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
    }
}


void MainWindow::quitSDL()
{
    // Destroy window and renderer
    if(m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = 0;
    }

    if(m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = 0;
    }
    if(m_game) m_game->setForceQuit(true);

    // Quit SDL and SDL_Image
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* MainWindow::loadBackground(string file) {
    // change background color to blue
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    SDL_Surface* background = NULL;
    background = IMG_Load(file.c_str());
    if( background == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", file.c_str(), IMG_GetError() );
    }
    SDL_Texture* bTexture = SDL_CreateTextureFromSurface(m_renderer,background);
    // activate BlendMode and set an alpha value for blend effects
    SDL_SetTextureBlendMode(bTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(bTexture, SDL_ALPHA_OPAQUE);
    SDL_FreeSurface(background);
    SDL_RenderCopy(m_renderer,bTexture,NULL,NULL);
    SDL_RenderPresent(m_renderer);
    return bTexture;
}

void MainWindow::openSettingsMenu()
{
    SDL_SetRenderDrawColor(m_renderer,0,0,0,255);
    vector<string> temp_vec = {"fullscreen on/off", "music on/off", "back"};

    int temp_selected = 0;
    do {
        temp_selected = m_menu->showMenu(temp_vec, 1, true);
        if (temp_selected == 0) setFullscreen();
        else if (temp_selected == 1 && m_game) m_game->getMixer()->pauseAndResume();
    } while (temp_selected != 2);
}

void MainWindow::openWaitingScreen(string path)
{
    SDL_Texture* background = loadBackground(path);
    vector<string> vec = {"Press Enter to continue"};
    getMenu()->showMenu(vec, 0, false);
    // blend the image out in order to get a smoother switchover
    for (int alpha = SDL_ALPHA_OPAQUE; alpha > SDL_ALPHA_TRANSPARENT; alpha-=2) {
    	SDL_Delay(1);//usleep(10000);
        SDL_SetTextureAlphaMod(background, alpha);
        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, background, NULL, NULL);
        SDL_RenderPresent(m_renderer);
    }
    // change background color to blue
    SDL_SetRenderDrawColor(m_renderer, 10, 76, 124, 255);
}
int jumper::MainWindow::w()
{
    return m_width;
}

int jumper::MainWindow::h()
{
    return m_height;
}

} /* namespace jumper */
#include "MainWindow.hpp"


