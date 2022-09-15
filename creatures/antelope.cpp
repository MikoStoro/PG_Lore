#pragma once
#include "antelope.h"
#include <random>

char Antelope::flee(){
	std::random_device device;
	std::mt19937 gen(device());
	if (gen() % 2 == 0) {
		Field* newField = getAdjacentField(true);
		if (newField != NULL) {
			world->addToJournal(("Stworzenie: " + getName() + " ucieklo od walki"));
			pos->creature = NULL;
			pos = newField;
			return 'M';
		}
	}
	return NULL;
}

Antelope::Antelope()
{
	strength = 4;
	initiative = 4;
	symbol = 'A';
	type = "Antylopa";
}

Antelope::Antelope(World* w)
{
	world = w;
	strength = 4;
	initiative = 4;
	symbol = 'A';
	type = "Antylopa";
}

char Antelope::attackSkill(Creature* creature){
	if (creature->getStrength() >= strength) { return flee(); }
	else { return NULL; }
}

char Antelope::defenseSkill(Creature* creature) {
	if (creature->getStrength() >= strength) {return flee(); }
	else { return NULL; }
}

void Antelope::action()
{
	age++;
	move(true);
	move();
}

Antelope::~Antelope(){}
