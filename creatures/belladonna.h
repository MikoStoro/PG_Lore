#pragma once
#include "plant.h"

class Belladonna : public Plant {
public:
	Belladonna();
	Belladonna(World* w);
	char defenseSkill();
	~Belladonna();
};