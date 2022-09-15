#include "plant.h"
#include <random>

void Plant::action()
{
	tryToReproduce();
}

char Plant::collision(Creature* creature)
{
	return NULL;
}

Plant::Plant()
{
	strength = 0;
	initiative = 0;
	world = NULL;
	symbol = 'p';
	type = "plant";
	reproductionRate = 100;
}

Plant::Plant(World* w)
{
	strength = 0;
	initiative = 0;
	world = w;
	symbol = 'p';
	type = "plant";
	reproductionRate = 100;
}

bool Plant::isAnimal()
{
	return false;
}

Plant::~Plant(){}

void Plant::tryToReproduce(){
	std::random_device device;
	std::mt19937 gen(device());

	if ((gen() % 100) <= reproductionRate) {
		this->reproduce();
	}
}
