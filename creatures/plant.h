#pragma once
#include "creature.h"

class Plant : public Creature {
protected:
	void action() override;
	char collision(Creature* creature) override;
	int reproductionRate; //% value

public:
	void tryToReproduce();
	Plant();
	Plant(World* w);
	bool isAnimal();
	~Plant();
;
};