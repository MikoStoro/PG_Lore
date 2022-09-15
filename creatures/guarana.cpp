#include "guarana.h"

Guarana::Guarana()
{
	strength = 0;
	initiative = 0;
	symbol = 'G';
	type = "Guarana";
	reproductionRate = 30;
}

Guarana::Guarana(World* w)
{
	world = w;
	strength = 0;
	initiative = 0;
	symbol = 'G';
	type = "Guarana";
	reproductionRate = 30;
}

Guarana::~Guarana(){}

char Guarana::defenseSkill(Creature* creature){
	creature->modifyStrength('+', 3);
	return NULL;
}
