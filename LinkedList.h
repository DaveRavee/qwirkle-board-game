#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

class LinkedList {
public:

   LinkedList();
   ~LinkedList();

   void print(bool colours);
   int getLength();
   
   Tile* get(int i);

   void addFront(Tile* tile);
   void addBack(Tile* tile);
   void addAt(Tile* tile, int index);

   void deleteFront();
   void deleteBack();
   void deleteAt(int i);

   std::string getContents();
   
   void clear();

   void printColour(Node* currentNode, bool end);

private:
   Node* head;
   Node* tail;
   int length; 
};

#endif // ASSIGN2_LINKEDLIST_H
