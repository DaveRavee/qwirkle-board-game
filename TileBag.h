#ifndef TILE_BAG_H
#define TILE_BAG_H

#include "LinkedList.h"
#include <sstream>

class TileBag
{
    public:
        TileBag();
        ~TileBag();
        TileBag(Tile&& other);

        std::string getBagContents();
        void addTile(Tile* tile);
        void printBag(bool colours);
        bool isEmpty();
        void shuffleTileBag();
        Tile* drawTile();
        Tile* swapTile(Tile* tile);
    
        int getLength();

        void placeTilesFromSave(std::string tileBagString);

    private:
        LinkedList* tileBag;

};

#endif // TILE_BAG_H