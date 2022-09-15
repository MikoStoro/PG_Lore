#include "animal.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

enum actionResults {
	STAY = 'S',
	DIE = 'D',
	ESCAPE = 'E',
	MOVE = 'M',
	NONE = NULL
};

char Animal::collision(Creature* creature) {
	//Kody wyniku:
	//S - zostañ
	//D - zgiñ
	//E - uciekaj
	//M - przemieœæ siê
	if (this->type != creature->getType()) {
		char s =  creature->defenseSkill(this);
		if (s == NONE) {
			s = this->attackSkill(creature);
		}

		if (s != NONE) {
			return s;
		}

		if ((this->strength >= creature->getStrength())) {
			if (creature->isAnimal()) {world->addToJournal("Stworzenie: " + getName() + " walczy ze stworzem " + creature->getName());}
			else { world->addToJournal(("Stworzenie: " + getName() + " zjada: " + creature->getName())); }
			creature->destroy();
			return MOVE;
		}
		else{
			return DIE;
		}
	}else {
		this->reproduce();
		return STAY;
	}

}

void Animal::move(bool mustBeEmpty) {

	Field* oldField = this->pos;
	Field* targetField = getField(mustBeEmpty);
	Field** board = world->getBoard();
	if (targetField != NULL) {
		if (targetField->creature == NULL) {
			oldField->creature = NULL;
			targetField->creature = this;
			this->setPos(targetField);
		}
		else {
			char result = collision(targetField->creature);

			switch (result) {
			case MOVE:
				oldField->creature = NULL;
				targetField->creature = this;
				this->setPos(targetField);
				break;
			case DIE:
				this->destroy();
				break;
			case ESCAPE:
				this->move(true);
				break;
			case STAY: break;
			}
		}
	}
}

bool Animal::isAnimal()
{
	return true;
}




void Animal::action() {
	age++;
	move();
}

Animal::Animal() {
	symbol = 'a';
	type = "animal";
}

Animal::Animal(World* w) {
	world = w;
	symbol = 'a';
	type = "animal";
}

Animal::~Animal() {

}
