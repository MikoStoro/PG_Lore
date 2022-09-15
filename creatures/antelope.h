#pragma once
#include "animal.h"

class Antelope : public Animal {
private:
	char flee();
public:
	Antelope();
	Antelope(World* w);
	char defenseSkill(Creature* creature) override;
	char attackSkill(Creature* creature) override;
	void action() override;
	~Antelope();
};