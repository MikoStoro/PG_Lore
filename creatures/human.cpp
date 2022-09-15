#include "human.h"
#include <conio.h>
#include <iostream>

#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_SPACE 32

void Human::specialSkill(){
    //Eliksir
    if (skillCooldown <= 0) {
        skillCooldown = maxCooldown;
        strength += (elixirPower - bonusStrength);
        bonusStrength = elixirPower;
        world->addToJournal("Czlowiek uzywa umiejetnosci Magiczny Eliksir! Aktualna sila: " + std::to_string(strength));
    }
    else { world->addToJournal(("Czlowiek probuje uzyæ umiejetnosci Magiczny Eliksir, ale jest ona nieneladowana: " + std::to_string(skillCooldown))); }
}

Human::Human()
{
    strength = 5;
    initiative = 4;
    pos = NULL;
    symbol = 'C';
    type = "Czlowiek";
    skillCooldown = 0;
    bonusStrength = 0;
}

Human::Human(World* w)
{
    strength = 5;
    initiative = 4;
    pos = NULL;
    world = w;
    symbol = 'C';
    type = "Czlowiek";
    skillCooldown = 0;
    bonusStrength = 0;
}

Human::~Human(){}

Field* Human::getField(bool empty)
{
    int code = world->getHumanInput();
    Field** board = world->getBoard();


    do
    {
        if (_kbhit())
        {
            code = _getch();

        }
        switch (code)
            {
            case KB_LEFT:
                if (world->moveLegal(getX()-1, getY())) { return &board[getX()-1][getY()]; }
                break;

            case KB_RIGHT:
                if (world->moveLegal(getX()+1, getY())) { return &board[getX()+1][getY()]; }
                break;

            case KB_UP:
                if (world->moveLegal(getX(), getY()-1)) { return &board[getX()][getY()-1]; }
                break;

            case KB_DOWN:
                if (world->moveLegal(getX(), getY()+1)) { return &board[getX()][getY()+1]; }
                break;
            case KB_SPACE:
                specialSkill();
                break;
            }
    } while (code != KB_DOWN && code != KB_LEFT && code != KB_RIGHT && code != KB_UP && code != KB_SPACE);
    return NULL;
}

void Human::action(){
    age++;
    if (skillCooldown > 0) { skillCooldown -= 1; }
    if (bonusStrength > 0) {
        bonusStrength -= 1; strength -= 1;
        world->addToJournal("Moc Eliksiru slabnie! Aktualna sila: " + std::to_string(strength));
    }
    move();
}

void Human::destroy()
{
    world->addToJournal("Stworzenie " + getName() + " zostalo zniszczone");
    world->removeCreature(this);
}

std::string Human::getData()
{
    return (getType() + " " + std::to_string(getX()) + " " + std::to_string(getY()) + " " + std::to_string(strength) + " " + std::to_string(bonusStrength) + " " + std::to_string(skillCooldown));
}

void Human::setCooldown(int x)
{
    skillCooldown = x;
}

void Human::setBonusStrength(int x)
{
    bonusStrength = x;
}



