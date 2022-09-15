#include "sheep.h"

Sheep::Sheep()
{
	initiative = 4;
	strength = 4;
	symbol = 'O';
	type = "Owca";
}

Sheep::Sheep(World* w)
{
	world = w;
	initiative = 4;
	strength = 4;
	symbol = 'O';
	type = "Owca";
}

Sheep::~Sheep()
{
}
