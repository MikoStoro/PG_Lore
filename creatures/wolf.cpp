#include "wolf.h"

Wolf::Wolf()
{
	strength = 9;
	initiative = 5;
	type = "Wilk";
	symbol = 'W';
}

Wolf::Wolf(World* w)
{
	world = w;
	strength = 9;
	initiative = 5;
	type = "Wilk";
	symbol = 'W';
}

Wolf::~Wolf()
{
}
