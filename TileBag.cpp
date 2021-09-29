#include <iostream>
#include <random>
#include <utility>
#include "TileBag.h"


TileBag::TileBag()
{
    tileBag = new LinkedList();
}

// Shuffle algorithm is very inefficient because the random indexes may 
// be equal to an index that already has a tile
// Is there a better way to do this...? That's a problem for the individual component lmao
void TileBag::shuffleTileBag()
{

    for (int k = 0; k < TILE_SET; ++k)
    {
        for (int i = 0; i < MAX_COLOURS; ++i)
        {
            for (int j = 0; j < MAX_SHAPES; ++j)
            {
                Tile* newTile = new Tile(colours[i], shapes[j]);
                addTile(newTile);
            }
        }
    }
    
    LinkedList* newTileBag = new LinkedList();
    std::random_device randomSeed;
    std::uniform_int_distribution<int> uniform_dist(0, tileBag->getLength());
    int count = 0;
    while (count < (MAX_TILES * TILE_SET))
    {
        int index = uniform_dist(randomSeed);
        if (tileBag->get(index) != nullptr)
        {
            Tile* newTileNewBag = new Tile(*(tileBag->get(index)));
            tileBag->deleteAt(index);
            newTileBag->addBack(newTileNewBag);
            ++count;
        }
    }

    delete tileBag;
    tileBag = newTileBag;
    newTileBag = nullptr;


}

TileBag::~TileBag()
{
    if (tileBag != nullptr)
    {
        delete tileBag;
        tileBag = nullptr;
    }
}

// MOVE CONSTRUCTOR
TileBag::TileBag(Tile&& other) 
{ 
    // TODO: ?
}

void TileBag::addTile(Tile* tile)
{
    tileBag->addBack(tile);
}

Tile* TileBag::drawTile()
{
    Tile* returnTile = nullptr;
    if (tileBag->getLength() > 0)
    {
        returnTile = new Tile(*tileBag->get(0));
        tileBag->deleteFront();
    }
    return returnTile;
}

void TileBag::printBag(bool colours)
{
    tileBag->print(colours);
}

bool TileBag::isEmpty()
{
    bool flag = true;
    if (tileBag->getLength() != 0)
    {
        flag = false;
    }
    return flag;
}

int TileBag::getLength()
{
    return tileBag->getLength();
}

std::string TileBag::getBagContents() 
{ 
    return tileBag->getContents();
}

void TileBag::placeTilesFromSave(std::string tileBagString)
{
    std::istringstream tileBagStream(tileBagString);
    std::string temp;
    while(std::getline(tileBagStream, temp, ',')) 
    {
        Tile* tile = new Tile(temp);
        this->addTile(tile);
    }
}