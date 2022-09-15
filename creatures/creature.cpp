#include "creature.h"
#include <stdio.h>
#include <random>

Creature::Creature() {
	age = 0;

	strength = 1;
	initiative = 1;
	pos = NULL;
	world = NULL;
	symbol = ' ';
	type = "creature";
}

Creature::Creature(World* w) {
	age = 0;
	strength = 1;
	initiative = 1;
	pos = NULL;
	world = w;
	symbol = ' ';
	type = "creature";
}

std::string Creature::getType() {
	return type;
}

Field* Creature::getPos() const{
  return this->pos;
}

int Creature::getInitiative() const{
  return this->initiative;
}

void Creature::draw() const{
  putchar(int(this->symbol));
}


World* Creature::getWorld() const{
  return this->world;
}

void Creature::destroy(){
	world->addToJournal("Stworzenie " + getName() + " zostalo zniszczone");
	world->removeCreature(this);
}

char Creature::defenseSkill(Creature* creature)
{
	return NULL;
}

int Creature::getAge() {
	return age;
}

int Creature::getStrength() {
	return strength;
}

void Creature::modifyStrength(char sign, int value){
	std::string message = "";
	message += ("Sila stworzenia: " + getName());
	if (sign == '+') {
		strength += value;
		message += " podniesiona o ";
	}
	else if (sign == '-') {
		strength -= value;
		message += " obnizona o ";
	}
	message += std::to_string(value);
	world->addToJournal(message);
}

char Creature::attackSkill(Creature* creature){
	return NULL;
}

int Creature::getX() {
	return this->pos->x;
}


int Creature::getY() {
	return this->pos->y;
}

void Creature::setPos(Field* field) {
	this->pos = field;
}


Field* Creature::getField(bool empty) {
	return getAdjacentField(empty);
}

bool Creature::isAnimal()
{
	return false;
}

std::string Creature::getName()
{
	return (type + "(" + std::to_string(getX()) + "," + std::to_string(getY()) + ")");
}

std::string Creature::getData()
{
	return (getType() + " " + std::to_string(getX()) + " " + std::to_string(getY()) + " " + std::to_string(getStrength()));
}

void Creature::setStrength(int x)
{
	strength = x;
}


Field* Creature::getAdjacentField(bool empty) {
	Field* fields[4];
	int index = 0;
	Field** board = world->getBoard();
	int x = getX();
	int y = getY();

	if (world->moveLegal(x, y+1, empty)) {
		fields[index] = &board[x][y+1];
		index++;
	}
	if (world->moveLegal(x, y-1, empty)) {
		fields[index] = &board[x][y-1];
		index++;
	}
	if (world->moveLegal(x+1, y, empty)) {
		fields[index] = &board[x+1][y];
		index++;
	}
	if (world->moveLegal(x-1, y, empty)) {
		fields[index] = &board[x-1][y];
		index++;
	}

	if (index == 0) {
		return NULL;
	}

	std::random_device device;
	std::mt19937 gen(device());
	return fields[gen() % index];
}

Field* Creature::getDirectionField(char direction)
{
	Field** board = world->getBoard();
	switch (direction) {
	case 'N': return(&board[pos->x][pos->y - 1]); break;
	case 'S': return(&board[pos->x][pos->y + 1]); break;
	case 'W': return(&board[pos->x-1][pos->y]); break;
	case 'E': return(&board[pos->x+1][pos->y]); break;
	default:return nullptr;
	}

}

void Creature::reproduce() {
	Field* field = getAdjacentField(true);
	if (field != NULL) {
		world->addToJournal(("Stworzenie: " + type + " rozmnozylo sie"));
		world->addCreature(this->type, field);
	}
}

Creature::~Creature(){}
