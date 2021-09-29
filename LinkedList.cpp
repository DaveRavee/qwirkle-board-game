#include "LinkedList.h"
#include <iostream>

LinkedList::LinkedList()
{
    this->head = nullptr;
    this->tail = nullptr; 
    this->length = 0;
}

LinkedList::~LinkedList()
{
    clear();
}

int LinkedList::getLength()
{
    return length;
}

std::string LinkedList::getContents()
{
    std::string linkedListContents = "";
    Node* current = head;
    if (current != nullptr && current->next != nullptr)
    {
        while (current->next!= nullptr)
        {
            linkedListContents += current->tile->getColour() + 
                                  std::to_string(current->tile->getShape()) + 
                                  ",";

            current = current->next;
        }
        linkedListContents += current->tile->getColour() + 
                              std::to_string(current->tile->getShape());
    }

    else if (current != nullptr)
    {
        linkedListContents += current->tile->getColour() + 
                              std::to_string(current->tile->getShape());
    }

    return linkedListContents;
}

void LinkedList::print(bool colours)
{
   Node* current = head;
   if (current != nullptr && current->next != nullptr)
   {
    while (current->next!= nullptr)
    {
        if (colours == true)
        {
            printColour(current, false);
            current = current->next;
        }
        else {
            std::cout << current->tile->getColour() 
                      << current->tile->getShape() 
                      << ",";
            current = current->next;
        }
    }
    if (colours == true)
    {
        printColour(current, true);
    }
    else
    {
        std::cout << current->tile->getColour() 
                  << current->tile->getShape() 
                  << std::endl;
    }
   }

   else if (current != nullptr)
   {
       if (colours == true)
       {
        printColour(current, true);
       }
       else
       {
           std::cout << current->tile->getColour() 
                     << current->tile->getShape() 
                     << std::endl;
       }
   }
}

void LinkedList::clear()
{
    while (head != nullptr)
    {
        deleteFront();
    }
}

Tile* LinkedList::get(int i)
{
    Node* currentNode = head;
    int x = 0;
    Tile* returnValue = nullptr;

    while (currentNode != nullptr)
    {
        if (x == i)
        {
            returnValue = currentNode->tile;
        }
        currentNode = currentNode->next;
        ++x;
    }

    return returnValue;
}

void LinkedList::addFront(Tile* tile)
{
    Node* newNode = new Node(tile, nullptr);
    newNode->next = head;
    if (head != nullptr)
    {
        head->prev = newNode;
    }

    else
    {
        tail = newNode;
    }

    head = newNode;
    ++length;
}

void LinkedList::addBack(Tile* tile)
{
    Node* newNode = new Node(tile, nullptr);
    newNode->prev = tail;

    if (tail != nullptr)
    {
        tail->next = newNode;
    }

    else
    {
        head = newNode;
    }

    tail = newNode;
    ++length;
}

void LinkedList::deleteBack()
{
    if (tail != nullptr)
    {
        Node* deleteNode = tail;
        tail = tail->prev;

        if (tail != nullptr)
        {
            tail->next = nullptr;
        }

        else
        {
            head = nullptr;
        }
        --length;
        delete deleteNode;
    }
}

void LinkedList::addAt(Tile* tile, int index){

    Node* newNode = new Node(tile, nullptr);

    Node* currentNode = head;
    int count = 0;

    if (head == nullptr && index == 0)
    {
        addFront(tile);
    }

    else if (index <= length && index >= 0 && head != nullptr)
    {
        if (index == 0)
        {
            addFront(tile);
        }

        else if (index == length)
        {
            addBack(tile);
        }
        else 
        {
            while (count < index-1)
            {
                currentNode = currentNode->next;
                ++count;
            }

            if (currentNode != nullptr)
            {
                if (currentNode->next != nullptr)
                {
                    Node* nodeAfter = currentNode->next;
                    currentNode->next = newNode;
                    newNode->next = nodeAfter;
                    nodeAfter->prev = newNode;
                    newNode->prev = currentNode;
                    ++length;
                }
            }
        }

    }

    else if (head == nullptr)
    {
        std::cout << "No contents available" << std::endl;
    }

    else if (index > length || index < 0)
    {
        std::cout << "Invalid index" << std::endl;
    }
}

void LinkedList::deleteAt(int i)
{
    if (i >= 0 && i < length)
    {
        if (i == 0)
        {
            deleteFront();
        }

        else if (i == length)
        {
            deleteBack();
        }

        else{
            Node* currentNode = head;
            if (head != nullptr)
            {
                int count = 0;
                while (count < i)
                {
                    currentNode = currentNode->next;
                    ++count;
                }

                if (currentNode->next == nullptr)
                {
                    tail = currentNode->prev;
                }

                else
                {
                    currentNode->next->prev = currentNode->prev;
                }

                if (currentNode->prev == nullptr)
                {
                    head = currentNode->next;
                }

                else
                {
                    currentNode->prev->next = currentNode->next;
                }

                delete currentNode;
                --length;
            }
        }
    }
    
}

void LinkedList::deleteFront()
{
    if (head != nullptr)
    {
        Node* deleteNode = head;
        head = head->next;

        if (head != nullptr)
        {
            head->prev = nullptr;

        }

        else
        {
            tail = nullptr;
        }

        delete deleteNode;
        --length;
    }
    
}

/* 
 * A helper method that determines the colour of the tile being placed 
 * on the board by using ANSI escape characters
 */
void LinkedList::printColour(Node* currentNode, bool end) 
{
    if (end == false) 
    { 
        if (currentNode->tile->getColour() == RED)
        {
            std::cout << REDCODE << currentNode->tile->getColour()
                      << currentNode->tile->getShape() << RESET << ",";
        }
        else if (currentNode->tile->getColour() == ORANGE)
        {
            std::cout << ORANGECODE << currentNode->tile->getColour()
                      << currentNode->tile->getShape() << RESET << ",";
        }
        else if (currentNode->tile->getColour() == YELLOW)
        {
            std::cout << YELLOWCODE << currentNode->tile->getColour()
                      << currentNode->tile->getShape() << RESET << ",";
        }
        else if (currentNode->tile->getColour() == GREEN)
        {
            std::cout << GREENCODE << currentNode->tile->getColour()
                      << currentNode->tile->getShape() << RESET << ",";
        }
        else if (currentNode->tile->getColour() == BLUE)
        {
            std::cout << BLUECODE << currentNode->tile->getColour()
                      << currentNode->tile->getShape() << RESET << ",";
        }
        else if (currentNode->tile->getColour() == PURPLE)
        {
            std::cout << PURPLECODE << currentNode->tile->getColour()
                      << currentNode->tile->getShape() << RESET << ",";
        }
    }
    else
    {
        if (currentNode->tile->getColour() == RED)
        {
            std::cout << REDCODE << currentNode->tile->getColour()
                      << currentNode->tile->getShape() << RESET;
        }
        else if (currentNode->tile->getColour() == ORANGE)
        {
            std::cout << ORANGECODE << currentNode->tile->getColour()
                      << currentNode->tile->getShape() << RESET;
        }
        else if (currentNode->tile->getColour() == YELLOW)
        {
            std::cout << YELLOWCODE << currentNode->tile->getColour()
                      << currentNode->tile->getShape() << RESET;
        }
        else if (currentNode->tile->getColour() == GREEN)
        {
            std::cout << GREENCODE << currentNode->tile->getColour()
                      << currentNode->tile->getShape() << RESET;
        }
        else if (currentNode->tile->getColour() == BLUE)
        {
            std::cout << BLUECODE << currentNode->tile->getColour()
                      << currentNode->tile->getShape() << RESET;
        }
        else if (currentNode->tile->getColour() == PURPLE)
        {
            std::cout << PURPLECODE << currentNode->tile->getColour()
                      << currentNode->tile->getShape() << RESET;
        }
    }
}