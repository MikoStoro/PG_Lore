#pragma once
#include "barszczSosnowskiego.h"
#include "animal.h"

BarszczSosnowskiego::BarszczSosnowskiego()
{
	strength = 10;
	initiative = 0;
	symbol = 'B';
	type = "BarszczSosnowskiego";
	reproductionRate = 10;
}

BarszczSosnowskiego::BarszczSosnowskiego(World* w)
{
	world = w;
	strength = 10;
	initiative = 0;
	symbol = 'B';
	type = "BarszczSosnowskiego";
	reproductionRate = 10;
}

BarszczSosnowskiego::~BarszczSosnowskiego(){}

char BarszczSosnowskiego::defenseSkill(Creature* creature){
	return 'D';
}

void BarszczSosnowskiego::killAnimal(Field* field){
	Creature* creature = field->creature;
	if (creature->isAnimal()) {
		creature->destroy();
	}
}

void BarszczSosnowskiego::killAdjacentAnimals()
{
	Field* field = getDirectionField('N');
	killAnimal(field);
	field = getDirectionField('S');
	killAnimal(field);
	field = getDirectionField('W');
	killAnimal(field);
	field = getDirectionField('E');
	killAnimal(field);
}
