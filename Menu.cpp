#include "Menu.h"
#include "GameEngine.h"
#include <memory>

Menu::~Menu()
{
    
}
void Menu::displayMenu(){

    bool menuDisplay = true;
    std::shared_ptr<bool> colours = std::make_shared<bool>(false);
    std::shared_ptr<bool> firstDisplay = std::make_shared<bool>(true);

    while (menuDisplay && !std::cin.eof())
    {
        if (*firstDisplay == true) { 
            printMenu(*colours);
        }

        int selection = 0;
        std::cin >> selection;
        std::cout << std::endl;
        if (selection == 1)
        {
            int players = 0;
            std::cout << "How many players? (2 - 4 players)" << std::endl;
            std::cout << "\n";
            std::cout << "> ";
            std::cin >> players;
            if (players == 2)
            {
                newGame(2, *colours);
            }
            else if(players == 3)
            {
                newGame(3, *colours);
            }
            else if (players == 4) 
            {
                newGame(4, *colours);
            } 
            else {
                std::cout << "Please enter a valid number from 2 - 4" << std::endl;
            }
        }

        else if (selection == 2)
        {
            loadGame();

        }

        else if (selection == 3)
        {
            if (*firstDisplay == false)
            {
                credits();
                printMenu(!(*colours));
            }
            else
            {
                credits();
            }
        }
        
        else if (selection == 4 || !cin.good())
        {
            menuDisplay = false;
            // delete colours;
            // delete firstDisplay;
            std::cout << "\nGoodbye\n" << std::endl;
        }

        else if (selection == 5)
        {
            if (*colours == false)
            {
                *colours = true;
                *firstDisplay = false;
                printMenu(true);
            }
            else if (*colours == true) 
            {
                *colours = false;
                printMenu(false);
            }
        }

        else if (selection == 6) 
        {
            if (*firstDisplay == false) { 
                help();
                printMenu(!(*colours));
            }
            else
            {
                help();
            }
        }

        else 
        {
            std::cout << "\nInvalid selection, please try again\n" << std::endl;
        }

    }
}

// CHANGE AMOUNT OF PLAYERRS RIGHT NOW
void Menu::newGame(int numbPlayers, bool colours)
{
    gameEngine = std::make_shared<GameEngine>();
    gameEngine->newGame(numbPlayers, colours);
}

void Menu::credits()
{    
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Name: Abdul " << std::endl;
    std::cout << "Student ID: s3542549" << std::endl;
    std::cout << "Email: s3542549@student.rmit.edu.au" << std::endl;
    std::cout << std::endl;

    std::cout << "Name: Ary Vinicio Pietroski Ramires" << std::endl;
    std::cout << "Student ID: s3857679" << std::endl;
    std::cout << "Email: s3857679@student.rmit.edu.au" << std::endl;
    std::cout << std::endl;

    std::cout << "Name: David Li Hong Xu" << std::endl;
    std::cout << "Student ID: s3844051" << std::endl;
    std::cout << "Email: s3844051@student.rmit.edu.au" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Name: Hugh Trung-Hieu Phung" << std::endl;
    std::cout << "Student ID: 3842508" << std::endl;
    std::cout << "Email: s3842508@student.rmit.edu.au" << std::endl;

    std::cout << "-----------------------------------" << std::endl;

}

void Menu::loadGame()
{
    bool loop = true;
    std::string filePath;
    std::cout << "\nEnter the filename from which load a game" << std::endl;
    while (loop && !std::cin.eof())
    {
        if (!cin.eof())
        {
            std::cout << "> " ;
            std::cin >> filePath;

            if (filePath == "help")
            {
                std::cout << "\nplease enter the relative path of the save file you want to load\n" 
                        << std::endl;
            }

            else if (checkFileExists(filePath))
            {
                //call game engine loadTheGame
                gameEngine = std::make_shared<GameEngine>();
                gameEngine->loadTheGame(filePath);
                loop = false;
            }

            else
            {
                loop = false;
            }
        }

        if (cin.eof()) 
        {
            std::cout << "\nGoodbye\n" << std::endl;
        }   
    }
}

bool Menu::checkFileExists(std::string filePath) 
{ 
    bool exists = false;
    std::ifstream saveFile(filePath);
    if (saveFile.fail()) 
    { 
        exists = false;
    }
    else 
    {
        exists = true;
    }
    saveFile.close();
    return exists;
}

/* 
 * A helper method that prints out the menu depending on the state 
 * (if there is color or no colour)
 */
void Menu::printMenu(bool colour)
{
    if (colour == false)
    {
        std::cout << "Menu" << std::endl;
        std::cout << "----" << std::endl;
        std::cout << "1. New Game" << std::endl;
        std::cout << "2. Load Game" << std::endl;
        std::cout << "3. Credits" << std::endl;
        std::cout << "4. Quit" << std::endl;
        std::cout << "5. Toggle Colours On" << std::endl;
        std::cout << "6. Help" << std::endl;
        std::cout << "\n";
        std::cout << "> "; 
    }
    else
    {
        std::cout << "Menu" << std::endl;
        std::cout << "----" << std::endl;
        std::cout << "1. New Game" << std::endl;
        std::cout << "2. Load Game" << std::endl;
        std::cout << "3. Credits" << std::endl;
        std::cout << "4. Quit" << std::endl;
        std::cout << "5. Toggle Colours Off" << std::endl;
        std::cout << "6. Help" << std::endl;
        std::cout << "\n";
        std::cout << "> "; 
    }
}

// A helper method that prints out the available commands 
void Menu::help()
{
    std::cout << "Please enter a number for 1 to 6" << std::endl;
    std::cout << "Input '1' to start a new game" << std::endl;
    std::cout << "Input '2' to load a game" << std::endl;
    std::cout << "Input '3' to show the students that worked on the game" 
              << std::endl;
    std::cout << "Input '4' to quit the game" << std::endl;
    std::cout << "Input '5' to toggle tile colours on/off" << std::endl;
    std::cout << "Input '6' to see this message again\n" << std::endl;
}