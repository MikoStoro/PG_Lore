#pragma once
#include <vector>
#include <map>
#include <string>
#include "creature.h"

class Creature;


struct Field {
	int x, y;
	Creature* creature;
};

class World {
private:
	int boardWidth, boardHeight;
	std::vector<Creature*> creatures;
	std::vector<Creature*> newlyAdded;
	std::string journal;
	Field** board;
	bool creaturesChanged;
	bool finished = false;
	int humanInput = NULL;
public:
	World(int w, int h);
	void nextTurn();
	void draw();
	Creature* createCreature(std::string t);
	Creature* getCreature(int x, int y);
	Field** getBoard();
	void removeCreature(Creature* creature);
	void removeCreature(int x, int y);
	void addCreature(std::string t, Field* field);
	void addCreature(std::string t, int x, int y);
	void loadCreatures();
	bool moveLegal(int x, int y, bool mustBeEmpty = false);
	int getWidth();
	int getHeight();
	void addToJournal(std::string entry);
	void resetJournal();
	bool isFinished();
	void finish();
	void writeToFile();
	void getInput();
	int getHumanInput();
	void extendedWrite();
	void extendedRead(std::string n = "extConfig.txt");
	void clear();
	~World();
};
