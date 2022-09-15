#pragma once
#include "creature.h"
enum actionResults;

class Animal : public Creature {
public:
	virtual void action() override;
	char collision(Creature* creature) override;
	virtual void move(bool MustBeEmpty = false);
	bool isAnimal() override;
	Animal();
	Animal(World* w);
	virtual ~Animal();
};