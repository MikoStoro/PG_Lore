#include "grass.h"

Grass::Grass(){
	strength = 0;
	initiative = 0;
	symbol = 'T';
	type = "Trawa";
	reproductionRate = 30;
}

Grass::Grass(World* w){
	world = w;
	strength = 0;
	initiative = 0;
	symbol = 'T';
	type = "Trawa";
	reproductionRate = 10;
}

Grass::~Grass(){}
