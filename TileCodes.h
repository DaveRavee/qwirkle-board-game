#include <iostream>

#ifndef ASSIGN1_TILECODES_H
#define ASSIGN1_TILECODES_H

//Used by Game Engine
#define INVALID_NUM -1

// Tilebag
#define TILE_SET 2
#define MAX_COLOURS 6
#define MAX_SHAPES 6
#define MAX_TILES (MAX_COLOURS * MAX_SHAPES)

// Colours
#define RED    'R'
#define ORANGE 'O'
#define YELLOW 'Y'
#define GREEN  'G'
#define BLUE   'B'
#define PURPLE 'P'

// Shapes
#define CIRCLE    1
#define STAR_4    2
#define DIAMOND   3
#define SQUARE    4
#define STAR_6    5
#define CLOVER    6

// Colour Codes 
#define REDCODE "\033[31m"
#define ORANGECODE "\033[38;2;255;165;0m"
#define YELLOWCODE "\033[38;2;255;255;0m"
#define GREENCODE "\033[38;2;67;196;78m"
#define BLUECODE "\033[0;34m"
#define PURPLECODE "\033[38;2;158;79;227m"
#define RESET "\033[0m"

const char colours[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};

const int shapes[] = {CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};

#endif // ASSIGN1_TILECODES_H

