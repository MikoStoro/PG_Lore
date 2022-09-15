#include "turtle.h"
#include <random>

Turtle::Turtle()
{
	strength = 2;
	initiative = 1;
	symbol = 'Z';
	type = "Zolw";
}

Turtle::Turtle(World* w)
{
	world = w;
	strength = 2;
	initiative = 1;
	symbol = 'Z';
	type = "Zolw";
}

char Turtle::defenseSkill(Creature* creature)
{
	if (creature->getStrength() < 5) {
		world->addToJournal(("Stworzenie " + getName() + "odparlo atak"));
		return 'R';
	}
	else {
		return NULL;
	}
}

void Turtle::action()
{
	age++;
	std::random_device device;
	std::mt19937 gen(device());
	if (gen() % 3 == 0) {
		move();
	}

}

Turtle::~Turtle()
{
}
