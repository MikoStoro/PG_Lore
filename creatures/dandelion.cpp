#include "dandelion.h"

Dandelion::Dandelion()
{
	strength = 0;
	initiative = 0;
	symbol = 'M';
	type = "Mlecz";
	reproductionRate = 15;
}

Dandelion::Dandelion(World* w)
{
	world = w;
	strength = 0;
	initiative = 0;
	symbol = 'M';
	type = "Mlecz";
	reproductionRate = 15;
}

Dandelion::~Dandelion()
{
}

void Dandelion::action()
{
	tryToReproduce();
	tryToReproduce();
	tryToReproduce();
}
