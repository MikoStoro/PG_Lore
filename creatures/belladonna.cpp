#include "belladonna.h"

Belladonna::Belladonna()
{
	strength = 99;
	initiative = 0;
	symbol = 'J';
	type = "WilczaJagoda";
	reproductionRate = 10;
}

Belladonna::Belladonna(World* w)
{
	world = w;
	strength = 99;
	initiative = 0;
	symbol = 'J';
	type = "WilczaJagoda";
	reproductionRate = 10;
}

char Belladonna::defenseSkill()
{
	return 'D';
}

Belladonna::~Belladonna(){}
