#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <fstream>

#include "GameEngine.h"

class Menu {
    public:
        //Display the menu
        void displayMenu();
        void newGame(int numbPlayers, bool colours);
        void loadGame();
        void credits();

        bool checkFileExists(std::string filePath);

        ~Menu();

        void printMenu(bool colour);
        void help();

    private:
        std::shared_ptr<GameEngine> gameEngine;

};

#endif // MENU_H