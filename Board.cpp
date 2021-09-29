#include "Board.h"
#include <iostream>
#include <stdio.h>
#include <algorithm>

using std::vector;
using std::shared_ptr;

// BASE GAMEPLAY WILL USE A FIXED SIZE BOARD

Board::Board()
{

}

void Board::setBoardDimensions(int rows, int cols)
{
    this->max_rows = rows;
    this->max_cols = cols;
    vector<vector<Tile*>> tiles (max_rows, vector<Tile*> (max_cols, nullptr));
    this->tiles = tiles;
    this->numberOfTilesOnBoard = 0;
       
    for (int i = 0; i < max_rows; ++i)
    {
        for (int j = 0; j < max_cols; ++j)
        {
            tiles[i][j] = nullptr;
        }
    }
}

std::string Board::getBoardState()
{
    std::string boardTilePositions = "";
    int count = 0;
    for (int i = 0; i < max_rows; ++i)
    {
        for (int j = 0; j < max_cols; ++j)
        {
            if (numberOfTilesOnBoard == 0)
            {
                boardTilePositions = "";
            }

            if (tiles[i][j] != nullptr && numberOfTilesOnBoard != 0)
            {
                ++count;
                if (count < numberOfTilesOnBoard)
                {
                    boardTilePositions += tiles[i][j]->getColour() + 
                    std::to_string(tiles[i][j]->getShape()) + AT + 
                    ((char)(i+ASCII)) + std::to_string(j) + ", ";
                }
                else if (count == numberOfTilesOnBoard)
                {
                    boardTilePositions += tiles[i][j]->getColour() + 
                    std::to_string(tiles[i][j]->getShape()) + AT + 
                    (char)(i+ASCII) + std::to_string(j);
                }
            }
        }
    } 

    return boardTilePositions;
}


Board::~Board() 
{ 
    for (int i = 0; i < max_rows; ++i)
    {
        for (int j = 0; j < max_cols; ++j)
        {
            if (tiles[i][j] != nullptr) 
            { 
                delete tiles[i][j];
                tiles[i][j] = nullptr;
            }
        }
    } 
}

void Board::printBoard(bool colours)
{
    char row = 'A';
    // Printing header of numbers
    std::cout << " ";
    for (int i = 0; i < max_cols; ++i)
    {
        printf("%4d", i);
    }
    std::cout << std::endl;
    
    std::cout << "  ";
    if (max_cols < 10)
    {
        for (int i = 0; i < max_cols; ++i)
        {
        std::cout << "---"; 
        }  
        std::cout << std::endl;
    }
    else 
    {
        for (int i = 0; i < max_cols; ++i)
        {
            std::cout << "----"; 
        }
        std::cout << std::endl;
    }

    // Now we print the rows
    for (int i = 0; i < max_rows; ++i)
    {
        std::cout << row << SMALL_WALL;
        for (int j = 0; j < max_cols; ++j)
        {
            if (tiles[i][j] == nullptr)
            {
                std::cout << BIG_WALL;
            }

            else
            {
                if (colours == true)
                {
                    printColour(tiles[i][j]);
                }
                else
                {
                    std::cout<< tiles[i][j]->getColour() 
                             << tiles[i][j]->getShape() 
                             << SMALL_WALL;
                }
            }
        }
        std::cout << std::endl;
        ++row;
    }
}

bool Board::isEmpty(int row, int col)
{
    bool isEmpty = true;
    if (row < max_rows - max_rows || 
        row > max_rows - 1 || 
        col < max_cols - max_cols || 
        col > max_cols - 1)
    {
        isEmpty = false;
    }
    
    else if (tiles[row][col] != nullptr)
    {
        isEmpty = false;
    }
    return isEmpty;
}

Tile* Board::getTile(int row, int col)
{
    Tile* returnTile = nullptr;
    if (tiles[row][col] != nullptr)
    {
        returnTile = tiles[row][col];
    }
    return returnTile;
}

void Board::placeTile(Tile* tile, int x, int y) 
{
    tiles[x][y] = tile;
    ++numberOfTilesOnBoard;
}   

void Board::removeTile(int x, int y)
{
    if(tiles[x][y] != nullptr)
    {
        delete tiles[x][y];
        tiles[x][y] = nullptr;
        --numberOfTilesOnBoard;
    }
}

//26,26
//B4@B2, B6@B3, B5@B4, R4@C2, G5@C4, Y1@D1, Y4@D2, Y2@D3, P4@E2
//R3,P1,R2,Y6,O1,G1,R5

void Board::setBoardFromSave(std::string boardString)
{
    std::istringstream boardStream(boardString);
    std::string temp;
    while(std::getline(boardStream, temp, ',')) 
    {
        int stringStart = temp.size() - temp.size();
        // Removing the white space in line
        if ((int)temp.find(" ") == stringStart)
        {
            temp.erase(remove_if(temp.begin(), temp.end(), isspace),
                                temp.end());
        }

        std::string delimiter = AT;
        
        // Obtaining the string of the user's tile
        std::string tileToPlace = temp.substr(stringStart, 
                                    temp.find(delimiter));
        
        // Obtaining the string of the position to place onto the board
        const char* tileChar = tileToPlace.c_str();
        std::string position = temp.substr(temp.find(delimiter) + 1, 
                                temp.length());

        Tile* tile = new Tile(tileChar);

        int rowPosition = (position[0] - ASCII);
        int colPosition = stoi(position.substr(1));
       
        placeTile(tile, rowPosition, colPosition);
        
    }    
}

void Board::createBoardFromSave(std::string boardString)
{
    std::istringstream boardStream(boardString);
    std::string temp; 
    std::getline(boardStream >> std::ws, temp, ',');   
    int rows = std::stoi(temp);
    std::getline(boardStream >> std::ws, temp, ',');   
    int cols = std::stoi(temp);
    setBoardDimensions(rows, cols);
}

int Board::getMaxRow()
{
    return max_rows;
}
int Board::getMaxCol()
{
    return max_cols;
}

int Board::getNumberOfTilesOnBoard()
{
    return numberOfTilesOnBoard;
}

void Board::printColour(Tile* tile)
{
    if (tile->getColour() == RED)
    {
        std::cout << REDCODE << tile->getColour() 
                             << tile->getShape() << RESET << SMALL_WALL;
    }
    else if (tile->getColour() == ORANGE) { 
        std::cout << ORANGECODE << tile->getColour() 
                                << tile->getShape() << RESET << SMALL_WALL;
    }
    else if (tile->getColour() == YELLOW) { 
        std::cout << YELLOWCODE << tile->getColour() 
                                << tile->getShape() << RESET << SMALL_WALL;
    }
    else if (tile->getColour() == GREEN) { 
        std::cout << GREENCODE << tile->getColour() 
                               << tile->getShape() << RESET << SMALL_WALL;
    }
    else if (tile->getColour() == BLUE) { 
        std::cout << BLUECODE << tile->getColour() 
                              << tile->getShape() << RESET << SMALL_WALL;
    }
    else if (tile->getColour() == PURPLE) { 
        std::cout << PURPLECODE << tile->getColour() 
                                << tile->getShape() << RESET << SMALL_WALL;
    }

}