#pragma once
#include "creature.h"
#include "world.h"
#include <string>
class World;
struct Field;

class Creature {
protected:
	int strength;
	int initiative;
	int age;
	Field* pos;
	World *world;
	char symbol;
	std::string type;

	enum directions {
		N = 1,
		E = 2,
		S = 3,
		W = 4
	};

public:

	virtual void action() = 0;
	virtual char collision(Creature* creature) = 0;
	void draw() const;
	virtual void destroy();
	virtual char defenseSkill(Creature* creature);
	virtual char attackSkill(Creature* creature);
	Creature(World* w);
	Creature();
	Field* getPos() const;
	void setPos(Field* field);
	int getX();
	int getY();
	int getInitiative() const;
	std::string getType();
	int getAge();
	int getStrength();
	void modifyStrength(char sign, int value);
	World* getWorld() const;
	void reproduce();
	Field* getAdjacentField(bool empty = false);
	Field* getDirectionField(char direction);
	virtual Field* getField(bool empty = false);
	virtual bool isAnimal();
	std::string getName();
	virtual std::string getData();
	void setStrength(int x);
	virtual ~Creature();

};
