#pragma once
#include "animal.h"

class Wolf : public Animal {
public:
	Wolf();
	Wolf(World* w);
	~Wolf();
};