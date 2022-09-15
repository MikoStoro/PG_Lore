#include "fox.h"

Dandelion::Dandelion()
{
	strength = 3;
	initiative = 7;
	symbol = 'L';
	type = "Lis";
}

Dandelion::Dandelion(World* w)
{
	world = w;
	strength = 3;
	initiative = 7;
	symbol = 'L';
	type = "Lis";
}

Dandelion::~Dandelion()
{
}

char Dandelion::attackSkill(Creature* creature)
{
	if (creature->getStrength() > strength) {
		world->addToJournal((getName() + " zdolal uniknac walki"));
		return 'S';
	}
	return 0;
}
