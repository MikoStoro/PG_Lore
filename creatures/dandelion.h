#pragma once
#include "plant.h"

class Dandelion : public Plant {
public:
	Dandelion();
	Dandelion(World* w);
	~Dandelion();
	void action() override;
};