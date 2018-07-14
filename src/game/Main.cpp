#include "MainWindow.hpp"
#include "TextureFactory.hpp"
#include "TexturedLayer.hpp"
#include "ScoreBoard.hpp"
#include "Item.hpp"
//#include "MovingBot.hpp"
#include "Game.hpp"
#include "Menu.hpp"
#include <iostream>
#include <unistd.h>

using namespace jumper;
using std::string;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	// Run on double tap:
	//chdir(".\\bin");

    MainWindow window("Jumper", 800, 600);

    vector<string> vec = {"Start","Settings", "Highscores", "End"};
    int selected = 0;
    bool quit = false;
    do {
        // opens the menu and stay in it if settings or highscores are selected
        selected = window.getMenu()->showMenu(vec, 1, true);

        if (selected==1)
            window.openSettingsMenu();
        if (selected==2)
                window.getMenu()->showHighScore();
        if (selected == 0)
        {
            Game game(&window);

            game.setupGame("..\\res\\lvl1.xml"); //game.setupGame(argv[1]);
            window.setGame(&game);
            window.openWaitingScreen("..\\res\\nextLevel1_font.png");
            game.start();
            quit = window.run();
            window.getMenu()->showHighScore();
        }
    } while (selected != 3 && selected != -1 && !quit);
    // Free textures
    TextureFactory::instance(window.getRenderer()).deleteAll();
}
