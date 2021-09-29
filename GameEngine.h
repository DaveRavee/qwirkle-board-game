#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#define PLACE "place"
#define REPLACE "replace"
#define SAVE "save"
#define HELP "help"
#define QUIT "quit"
#define TILE_ON_HAND "tileOnHand"
#define TILE_ON_BOARD "boardTilePlacement"
#define SAVE_EXTENSION ".save"
#define ASCII 65

#define MIN_COL 0 
#define MAX_HAND 6
#define MAX_TILE_LINE 6 
// Defining locations of string indexes for the player commands
#define TOTAL_COMMAND_LENGTH 15
#define TOTAL_COMMAND_LENGTH_CASE_1 14
#define TOTAL_COMMAND_LENGTH_CASE_2 15
#define HAND_TILE_INDEX 6
#define TILE_STRING_INDEX 12
#define TILE_STRING_LENGTH_SINGLE 2 
#define TILE_STRING_LENGTH_DOUBLE 3 
#define COMMAND_REPLACE 10 

#define UP 'U'
#define DOWN 'D'
#define LEFT 'L'
#define RIGHT 'R'

#define COLOUR "colour"
#define SHAPE "shape"

#define PLACEHOLDER "PLACEHOLD"
#define TWO_PLAYERS 2 
#define THREE_PLAYERS 3
#define FOUR_PLAYERS 4
#define TWO_PLAYER_LINE_NUMB 11
#define THREE_PLAYER_LINE_NUMB 14
#define FOUR_PLAYER_LINE_NUMB 17

#define FIRST_PLAYER 0
#define SECOND_PLAYER 1
#define THIRD_PLAYER 2
#define FOURTH_PLAYER 3

#define REPLACE_TILE_INDEX 8
#define TILE_INDEX 6
#define TILE_LENGTH 2 

#define PLACE_COMMAND_SINGLE_DIGIT 14
#define PLACE_COMMAND_DOUBLE_DIGIT 15
#define BOARD_INDEX 12 
#define BOARD_POS_SINGLE_DIGIT 2 
#define BOARD_POS_DOUBLE_DIGIT 2 

#include <iostream>
#include <memory>
#include <fstream>
#include <ostream>
#include <utility>
#include <queue>
#include "Player.h"
#include "TileBag.h"
#include "Board.h"

using std::cout;
using std::cin;
using std::endl;
//"using std::endl;" here but not on this file

class GameEngine
{
    public:
        GameEngine();
        ~GameEngine();

        void newGame(int numbPlayers, bool colours);
        void playTheGame();
        void saveTheGame(std::string fileName);
        
        void loadTheGame(std::string fileName);
        void loadPlayers(std::istream& inStream, string input);
        void loadBoard(std::istream& inStream, std::string input);
        void loadTileBag(std::istream& inStream, std::string input);
        void loadCurrentPlayer(std::istream& inStream, std::string input);


        void placeOnBoard(Tile* tile, int row, int col);
        bool actionPlace(string command, std::shared_ptr<Player> player, bool* firstTurn);
        bool actionReplace(string command, std::shared_ptr<Player> player);
        void playRound(std::shared_ptr<Player> player, bool* gameOver, bool* firstTurn, bool* quit, bool* firstEmptyHand);
        void replenishHandTiles(shared_ptr<Player> player);

        bool hasSurroundingTiles(int row, int col);
        bool edgeCase(int row, int col);
        bool hasSameColourOrShapeEdgeCases(Tile* tile, int row, int col);
        bool validTilePlacement(Tile* tile, int x, int y, shared_ptr<Player> player);
        bool isSameColourOrShape(Tile* tile, int row, int col);
        void updatePoints(shared_ptr<Player> player);
        void updatePoints(shared_ptr<Player> player, int incrementPoints);
        int countPointsInLine(int row, int col, char* direction);
        bool sixTileLineCheck(int row, int col, char direction, Tile* tile, string type);

        bool checkLeftIsValid(Tile* tile, int row, int col);
        bool checkRightIsValid(Tile* tile, int row, int col);
        bool checkUpIsValid(Tile* tile, int row, int col);
        bool checkDownIsValid(Tile* tile, int row, int col);

    

        // Tile* getTileFromStr(std::shared_ptr<Player> player, string tileString);
        int getBoardPlacementRow(string boardPlacementString);
        int getBoardPlacementCol(string boardPlacementString);

        void printRound(std::shared_ptr<Player> player);
        void gameOverPrint();
        void giveHand(std::shared_ptr<Player> player); 
        bool playersHaveEnoughTiles();
        bool playerNoLongerHasTiles();
        void setOtherPlayerTurn();
        bool playerHasTile(string tileString, shared_ptr<Player> player);
        bool tileIsValid(string tileString, string type);
        
        
        

    private:
        std::shared_ptr<Player> player1;
        std::shared_ptr<Player> player2;
        bool playerNameValid(char* name);
        std::shared_ptr<TileBag> tileBag;
        std::shared_ptr<Board> board;
        bool player1Turn;
        int current_round_score;
        int numbPlayers;
        std::vector<std::shared_ptr<Player>> playerList;
        bool colours;
        bool legacy;
};

#endif // GAMEENGINE_H
