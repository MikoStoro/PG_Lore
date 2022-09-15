#pragma once
#include "animal.h"

class Turtle : public Animal {
public:
	Turtle();
	Turtle(World* w);
	char defenseSkill(Creature* creature) override;
	void action() override;
	~Turtle();
};