// creatures.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include <iostream>
#include "world.h"
#include "human.h"

int main()
{
	World* world = new World(20, 20);
	//world->loadCreatures();
	world->extendedRead();
	world->draw();

	while (!world->isFinished()) {
		world->getInput();
		/*world->nextTurn();
		world->draw();*/
	}
	world->clear();
	return 0;
}