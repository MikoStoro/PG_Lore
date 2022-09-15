#pragma once
#include "world.h"
#include "animal.h"
#include "plant.h"

#include "wolf.h"
#include "sheep.h"
#include "fox.h"
#include "antelope.h"
#include "turtle.h"
#include "antelope.h"

#include "barszczSosnowskiego.h"
#include "dandelion.h"
#include "grass.h"
#include "guarana.h"
#include "belladonna.h"

#include "human.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <conio.h>

#define NOCREATURE ' '
#define ENTER 13
#define S 115
#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_SPACE 32
#define Q 113
#define R 114
#define T 116


World::World(int w, int h) {
	boardWidth = w;
	boardHeight = h;
	board = new Field* [w];
	creaturesChanged = false;
	for (int i = 0; i < h; i++) {
		board[i] = new Field[h];
	}

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			board[i][j].creature = NULL;
			board[i][j].x = i;
			board[i][j].y = j;
		}
	}
}

void World::removeCreature(Creature* creature) {
	for (int i = 0; i < creatures.size(); i++) {
		if (creature == creatures[i]) {
			board[creature->getX()][creature->getY()].creature = NULL;
			delete creatures[i];
			creatures.erase(creatures.begin()+i);
		}
	}
}

void World::removeCreature(int x, int y) {
	Field* field = &board[x][y];
	Creature* creature = field->creature;

	for (int i = 0; i < creatures.size(); i++) {
		if (creature == creatures[i]) {
			field->creature = NULL;
			delete creatures[i];
			creatures.erase(creatures.begin() + i);
		}
	}
}

int World::getHeight() {
	return boardHeight;
}

void World::addToJournal(std::string entry){
	journal += entry;
	journal += "\n";
}

void World::resetJournal()
{
	journal.clear();
}

bool World::isFinished()
{
	return finished;
}

void World::finish()
{
	finished = true;
	writeToFile();
}

void World::writeToFile()
{
	std::ofstream file;
	file.open("output.txt");

	for (int x = 0; x < boardWidth; x++) {
		for (int y = 0; y < boardHeight; y++) {
			Field* field = &board[x][y];
			if (field->creature != NULL) {
				Creature* c = field->creature;
				file << c->getType() << " " << c->getX() << " " << c->getY() << '\n';
			}
		}
	}

	file.close();
}


void World::getInput()
{
	int code = 0;
	while (code != ENTER && code != S && code!=KB_DOWN && code!=KB_LEFT&&code!=KB_RIGHT&&code!=KB_UP&&code!=KB_SPACE && code != Q){
		if (_kbhit()) {
			if (_kbhit()){
				code = _getch();
			}

			switch (code) {
			case ENTER:
				nextTurn();
				draw();
				break;
			case S:
				extendedWrite();
				break;
			case Q:
				finish();
			default:
				if (code == KB_DOWN || code == KB_LEFT || code == KB_RIGHT || code == KB_UP || code == KB_SPACE) {
					humanInput = code;
					nextTurn();
					draw();
					break;
				}
			}
		}
	}
}

int World::getHumanInput()
{
	return humanInput;
}

void World::extendedWrite()
{
	std::ofstream file;
	file.open("extConfig.txt");

	for (int x = 0; x < boardWidth; x++) {
		for (int y = 0; y < boardHeight; y++) {
			Field* field = &board[x][y];
			if (field->creature != NULL) {
				Creature* c = field->creature;
				file << c->getData() << '\n';
			}
		}
	}

	file.close();
}



void World::extendedRead(std::string n)
{
	std::ifstream file;
	file.open(n);
	std::string name;
	int x, y, strength, bonusStrength, cooldown;
	if (file.is_open()) {
		while (file.good()) {
			file >> name;
			file >> x;
			file >> y;
			file >> strength;
			addCreature(name, x, y);
			Creature* c = getCreature(x, y);
			c->setStrength(strength);
			if (name == "Czlowiek") {
				Human* cz = (Human*)c;
				file >> bonusStrength;
				file >> cooldown;
				cz->setBonusStrength(bonusStrength);
				cz->setCooldown(cooldown);
			}
		}
	}
	file.close();
}

void World::clear()
{
	for (int x = 0; x < boardWidth; x++) {
		for (int y = 0; y < boardHeight; y++) {
			delete board[x][y].creature;
			board[x][y].creature = NULL;
		}
	}
	creatures.clear();
	creaturesChanged = true;
}

int World::getWidth() {
	return boardWidth;
}

Field** World::getBoard(){
	return board;
}

Creature* World::createCreature(std::string t) {
	Creature* creature;
	if (t == "Wilk") { creature = new Wolf(this); }
	else if (t == "Owca") { creature = new Sheep(this); }
	else if (t == "Lis") { creature = new Dandelion(this); }
	else if (t == "Antylopa"){ creature = new Antelope(this); }
	else if (t == "Zolw") { creature = new Turtle(this); }
	else if (t == "Trawa") { creature = new Grass(this); }
	else if (t == "BarszczSosnowskiego") { creature = new BarszczSosnowskiego(this); }
	else if (t == "WilczaJagoda") { creature = new Belladonna(this); }
	else if (t == "Guarana") { creature = new Guarana(this); }
	else if (t == "Czlowiek") { creature = new Human(this); }
	else if (t == "Mlecz") { creature = new Dandelion(this); }
	else { std::cout << "Nieznane stworzenie"; creature = NULL; }
	return creature;
}

void World::addCreature(std::string t, Field* field) {
	Creature* creature = createCreature(t);
	if (creature != NULL) {
		creature->setPos(field);
		newlyAdded.push_back(creature);
		field->creature = creature;
		creaturesChanged = true;
	}
}



void World::addCreature(std::string t, int x, int y) {
	if (moveLegal(x,y)) {
		Creature* creature = createCreature(t);
		if (creature != NULL) {
			creature->setPos(&board[x][y]);
			newlyAdded.push_back(creature);
			Field* field = &board[x][y];
			field->creature = creature;
			creaturesChanged = true;
		}
	}
}

void World::loadCreatures(){
	std::ifstream file;
	file.open("startingConfig.txt");
	std::string input;
	int x, y;

	if (file.is_open()) {
		while (file.good()) {
			file >> input;
			file >> x;
			file >> y;
			addCreature(input, x, y);
		}
	}
	file.close();
}

void World::nextTurn() {
	resetJournal();

	if (creaturesChanged) {
		for (int i = 0; i < newlyAdded.size(); i++) {
			creatures.push_back(newlyAdded[i]);
		}
		newlyAdded.clear();
		sort(creatures.begin(), creatures.end(),
			[](Creature* a, Creature* b) {
				if (a->getInitiative() != b->getInitiative()) { return(a->getInitiative() > b->getInitiative()); }
				else { return(a->getAge() > b->getAge()); }
			});
		creaturesChanged = false;
	}

	for (int i = 0; i < creatures.size(); i++) {
		creatures[i]->action();
	}

	humanInput = NULL;
}

bool World::moveLegal(int x, int y, bool mustBeEmpty) {
	if (x >= 0 && x < boardWidth && y >= 0 && y < boardHeight) {
		if (mustBeEmpty && board[x][y].creature != NULL) {
			return false;
		}
		return true;
	}
	return false;
}

void World::draw() {
	system("cls");

	for (int x = 0; x < boardWidth+2; x++) {
		putchar('-');
	}
	putchar('\n');
	std::cout << journal << std::endl;
	for (int x = 0; x < boardWidth + 2; x++) {
		putchar('-');
	}

	putchar('\n');
	for (int x = 0; x < boardWidth; x++){
		putchar('|');
		for (int y = 0; y < boardHeight; y++) {
			Field* field = &board[y][x];
			if (field->creature != NULL) {
				field->creature->draw();
			}
			else { putchar(NOCREATURE); }
		}
		putchar('|');
		putchar('\n');
	}
	for (int x = 0; x < boardWidth + 2; x++) {
		putchar('-');
	}
}

Creature* World::getCreature(int x, int y) {
	return board[x][y].creature;
}

World::~World() {}