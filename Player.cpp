#include "Player.h"

Player::Player(std::string name)
{
    this->name = name;
    this->score = 0;
    this->hand = new Hand();
    this->state = false;
}

Player::~Player()
{
    delete hand;
}

void Player::setName(string name){
    this->name = name; 
}
    
string Player::getName(){
    return name;
}

void Player::setScore(int score){
    this->score = score;
}

int Player::getScore(){
    return score;
}

Hand* Player::getHand() { 
    return hand;
}

void Player::incrementScore(int score)
{
    this->score += score;
}

bool Player::getState() 
{
    return state;
}

void Player::setState(bool state)
{
    this->state = state;
}