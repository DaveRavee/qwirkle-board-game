#ifndef BOARD_H
#define BOARD_H
#define MAX_ROW 26
#define MAX_COL 26
#define ASCII 65
#define AT "@"
#define SMALL_WALL " |"
#define BIG_WALL "   |"

#include "Tile.h"
#include <vector>
#include <memory>
#include <string>
#include <sstream>

using std::vector;
using std::shared_ptr; 

class Board{
    public:
        Board();
        ~Board();

        void printBoard(bool colours);
        void placeTile(Tile* tile, int x, int y);
        bool isEmpty(int row, int col);
        Tile* getTile(int row, int col);
        void removeTile(int x, int y);
        std::string getBoardState();
        void setBoardFromSave(std::string boardString);
        void setBoardDimensions(int rows, int cols);
        void createBoardFromSave(std::string boardString);

        int getMaxRow();
        int getMaxCol();
        int getNumberOfTilesOnBoard();

        void printColour(Tile* tile);

    private:
        vector<vector<Tile*>> tiles;
        int numberOfTilesOnBoard;
        int max_rows;
        int max_cols;
};
    

#endif // BOARD_H