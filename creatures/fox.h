#pragma once
#include "animal.h"

class Dandelion : public Animal {
public:
	Dandelion();
	Dandelion(World* w);
	char attackSkill(Creature* creature) override;
	~Dandelion();
};