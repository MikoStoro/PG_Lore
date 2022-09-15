#pragma once
#include "plant.h"

class Guarana : public Plant {
public:
	Guarana();
	Guarana(World* w);
	~Guarana();
	char defenseSkill(Creature* creature) override;
};