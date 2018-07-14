/*
 * Menu.hpp
 *
 *  Created on: Feb 29, 2016
 *      Author: ledelkott
 *      Author: tstahn
 */

#ifndef JUMPER_MENU_HPP
#define JUMPER_MENU_HPP

#include <string>
#include <SDL2/SDL.h>
#include "Game.hpp"
#include "MainWindow.hpp"

namespace jumper {

using namespace std;

/**
 * @brief A class responsible for Menues in the MainWindow
 */
class Menu {
    public:
        /**
         * Constructor
         *
         * @param window    Window on which the menu should be displayed
         */
        Menu(MainWindow* window);

        /**
         * Destructor
         */
        ~Menu();

        /**
         * Shows given strings as manupoints and offers the opportunity to choose one of them
         * the position can be stated and whether the background is black or unchanged
         *
         * @param pLabels       Vector of strings which should be displayed
         * @param position      Position of the shown menupoints
         *                          0 - top, 1 - center, anything else - bottom
         * @param emptyBackground   decided whether the background is black or the previous one
         * @return position of selected menupoint
         */
        int showMenu(vector<string> &pLabels, int position, bool emptyBackground);

        /**
         * Reads the highscores out of a xml file and displays them on an black background
         * until keyboard input (return, enter, esc)
         */
        void showHighScore();

        /**
         * Reads the credits out of a xml file and renders it to the screen, not used in order to make the program look better
         */
        void showCredits();

    private:
        MainWindow* m_Window;
        SDL_Surface* screen;
    };
}


#endif //JUMPER_MENU_HPP
