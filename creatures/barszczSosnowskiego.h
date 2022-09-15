#pragma once
#include "plant.h"

class BarszczSosnowskiego : public Plant {
public:
	BarszczSosnowskiego();
	BarszczSosnowskiego(World* w);
	~BarszczSosnowskiego();
	char defenseSkill(Creature* creature) override;
private:
	void killAnimal(Field* field);
	void killAdjacentAnimals();
};