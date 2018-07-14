/*
 * TexturedLayer.cpp
 *
 *  Created on: Feb 29, 2016
 *      Author: ledelkott
 *      Author: tstahn
 */

#include "Menu.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include "Vector.hpp"
#include "Renderable.hpp"
#include <sstream>
#include <fstream>

namespace jumper
{

Menu::Menu(MainWindow* window)
{
    m_Window = window;
    screen = NULL;
}
Menu::~Menu()
{

}

int Menu::showMenu(vector<string> &labels, int position, bool emptyBackground)
{
    // initialise TTF_Init, necessary to use ttf_font functions
    if (TTF_Init() == -1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    SDL_Surface* screen = NULL;

    // creating a blank surface, which will be the background
    if (emptyBackground)
        screen = SDL_CreateRGBSurface(0, m_Window->w(), m_Window->h(), 32, 0, 0, 0, 0);

    TTF_Font *font = TTF_OpenFont("..\\res\\HotSweat.ttf", 25);
    const int NUMMENU = labels.size();
    SDL_Surface* menus[NUMMENU];
    // the integer which will be updated and returned
    int selected = 0;
    SDL_Color color[2] = {{255, 0, 0},          // Red
                          {255, 255, 255}};     // White
    SDL_Rect pos[NUMMENU];

    // calculate the centered positions of every font_surface
    for (int i = 0; i < NUMMENU; i++)
    {
        if (i == 0)
        {
            menus[0] = TTF_RenderText_Solid(font, labels[0].c_str(), color[0]);
        }
        else
        {
            menus[i] = TTF_RenderText_Solid(font, labels[i].c_str(), color[1]);
        }

        pos[i].x = m_Window->w()/2 - menus[i]->clip_rect.w/2;

        //set the heigth of the target rectangle according the the position
        //top position
        if (position == 0)
        {
            pos[i].y = 30.0*i;
        }
        // centered position
        else
        {
            if (position == 1)
            {
                if (NUMMENU % 2 != 0)
                {
                    pos[i].y = (m_Window->h() / 2) - (NUMMENU / 2) * 30.0f + i * 30 - 5;
                }
                else
                {
                    pos[i].y = ((float) m_Window->h() - (30.0f) * (float) NUMMENU) / 2.0f + 30 * i + 5;
                }
            }
            //bottom position
            else
            {
                pos[i].y = (m_Window->h() - NUMMENU * 30) + i * 30;
            }
        }
        // sets the SDL_Rect width and length of every surface
        pos[i].h = menus[i]->clip_rect.h;
        pos[i].w = menus[i]->clip_rect.w;
    }

    SDL_Event event;
    SDL_Renderer *renderer = m_Window->getRenderer();

    while (true)
    {
        // add the font_surfaces to the backgroundSurface and render it
        for (int i = 0; i < NUMMENU; i ++)
        {
            menus[i] = TTF_RenderText_Solid(font, labels[i].c_str(), color[(selected == i) ? 0 : 1]);

            if (emptyBackground)
            {
                // write the font_surfaces to one single scree_surface if the background should be cleared
                SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
            }
            else
            {
                // create Textures out of the fontsurfaces and immediately copy it to the renderer, keeps the background
                SDL_Texture *bTexture = SDL_CreateTextureFromSurface(renderer, menus[i]);
                SDL_RenderCopy(renderer, bTexture, NULL, &pos[i]);
            }
        }

        if (emptyBackground)
        {
            // clears the backgruond and copies the screensurface to the renderer
            SDL_RenderClear(renderer);
            SDL_Texture *bTexture = SDL_CreateTextureFromSurface(renderer, screen);
            SDL_RenderCopy(renderer, bTexture, NULL, NULL);
        }

        SDL_RenderPresent(renderer);

        // wait for input from user
        if (SDL_WaitEvent(&event))
        {
            // free surfaces
            for (int i = 0; i < NUMMENU; i++)
                SDL_FreeSurface(menus[i]);

            switch (event.type)
            {
                // in case the window should be closed, return -1
                case SDL_QUIT:
                    SDL_FreeSurface(screen);
                    return -1;
                case SDL_KEYDOWN:
                    // if key_down is pressed update selected
                    if (event.key.keysym.sym == SDLK_DOWN)
                    {
                        selected = selected +1;
                        // if last entry was selected and key down is pressed go to the first menu entry
                        if (selected >= NUMMENU)
                            selected = 0;
                    }
                    // if key_up is pressed update selected
                    if (event.key.keysym.sym == SDLK_UP)
                    {
                        selected = selected - 1;
                        // if first entry was selected and key up is pressed go to the last menu entry
                        if (selected < 0)
                            selected = NUMMENU -1;
                    }
                    if (event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)
                    {
                        SDL_FreeSurface(screen);
                        return selected;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void Menu::showHighScore()
{
    using boost::property_tree::ptree;

    SDL_Renderer* renderer = m_Window->getRenderer();
    // initialise the font which will be used and clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color color = {255, 255, 255};
    TTF_Font *font = TTF_OpenFont("..\\res\\HotSweat.ttf", 25);

    // try to open highscorefile
    try
    {
        // copy "Highscores!" to the screen and leave an offset afterwards
        int yPos = 20;
        int yOffset = (m_Window->h()-20) / 12;
        SDL_Surface* outputSurface = TTF_RenderText_Solid(font, "Highscores!", color);

        SDL_Rect outputPos;
        outputPos.x = m_Window->w() / 2 - outputSurface ->clip_rect.w / 2;
        outputPos.y = yPos;
        outputPos.h = outputSurface->clip_rect.h;
        outputPos.w = outputSurface->clip_rect.w;

        SDL_Texture* outputTexture = SDL_CreateTextureFromSurface(renderer, outputSurface);
        SDL_RenderCopy(renderer, outputTexture, NULL, &outputPos);
        SDL_FreeSurface(outputSurface);
        SDL_RenderPresent(renderer);

        yPos += yOffset;

        ptree pt;

        // read the xml file and create a texture out of every entry which will be copied to the renderer at the
        // according position
        read_xml(".\\highscore.xml", pt);

        BOOST_FOREACH (const ptree::value_type&  v, pt.get_child("highscores"))
        {
            if (v.first == "score")
            {
                string name = v.second.get("<xmlattr>.name", "");
                int score = v.second.get<int>("score", 0);

                SDL_Surface* outputSurface = TTF_RenderText_Solid(font, (name + ":  " + to_string(score)).c_str(), color);

                SDL_Rect outputPos;
                yPos += yOffset;
                outputPos.x = m_Window->w() / 2 - outputSurface ->clip_rect.w / 2;
                outputPos.y = yPos;
                outputPos.h = outputSurface->clip_rect.h;
                outputPos.w = outputSurface->clip_rect.w;

                SDL_Texture* outputTexture = SDL_CreateTextureFromSurface(renderer, outputSurface);
                SDL_RenderCopy(renderer, outputTexture, NULL, &outputPos);
                SDL_FreeSurface(outputSurface);
                SDL_RenderPresent(renderer);
            }
        }
    }
    catch (...)
    {
        // if there are no highscores yet, print this information
        SDL_Surface* outputSurface = TTF_RenderText_Solid(font, "Keine Highscores vorhanden!", color);

        SDL_Rect outputPos;
        outputPos.x = m_Window->w() / 2 - outputSurface ->clip_rect.w / 2;
        outputPos.y = 20;
        outputPos.h = outputSurface->clip_rect.h;
        outputPos.w = outputSurface->clip_rect.w;

        SDL_Texture* outputTexture = SDL_CreateTextureFromSurface(renderer, outputSurface);
        SDL_RenderCopy(renderer, outputTexture, NULL, &outputPos);
        SDL_FreeSurface(outputSurface);
        SDL_RenderPresent(renderer);
    }

    bool enter = false;
    SDL_Event event;
    // wait until one of the enter keys is pressed
    while (!enter)
    {
        if (SDL_WaitEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    enter = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE)
                        enter = true;
                    break;
                default:
                    break;
            }
        }
    }
}
void Menu::showCredits() {
    // reads the credits file and put it line by line in a vector
    string line;
    ifstream infile("..\\res\\credits.txt");
    if (infile) {
        vector<string> credits;
        while (std::getline(infile, line)) {
            credits.push_back(line);
        }
        infile.close();
        // initiliase the font
        TTF_Font *font = TTF_OpenFont("..\\res\\HotSweat.ttf", 25);
        SDL_Color color = {255, 255, 255};
        SDL_Renderer* renderer = m_Window->getRenderer();
        SDL_Surface* outputSurface[10];
        SDL_Rect outputPos[10];
        // go through the whole vector
        for (auto it = credits.begin(); it != credits.end(); it++){
            SDL_RenderClear(renderer);
            // set the position on the screen for the next 10 entries
            for (int i = 0; i<10; i++) {
                if ((it+i) != credits.end()) {
                    if ((it+i)->c_str() == "")
                        outputSurface[i] = TTF_RenderText_Solid(font, " ", color);
                    else
                        outputSurface[i] = TTF_RenderText_Solid(font, (it+i)->c_str(), color);
                    if (outputSurface[i]!=NULL) {
                        outputPos[i].x = m_Window->w() / 2 - outputSurface[i]->clip_rect.w / 2;
                        outputPos[i].y = 20 + (m_Window->h() / 10) * i;
                        outputPos[i].h = outputSurface[i]->clip_rect.h;
                        outputPos[i].w = outputSurface[i]->clip_rect.w;
                        SDL_Texture *outputTexture = SDL_CreateTextureFromSurface(renderer, outputSurface[i]);
                        SDL_RenderCopy(renderer, outputTexture, NULL, &outputPos[i]);
                        SDL_FreeSurface(outputSurface[i]);
                    }
                } else break;
            }
            // render the next 10 entries on the screen and wait until the next entry is selected
            SDL_RenderPresent(renderer);
            SDL_Delay(500);
        }
        return;
    }
    else  {
        cout << "couldn't find credits file" << endl;
        return;
    }
}

}
