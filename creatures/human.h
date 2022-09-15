#pragma once
#include "animal.h"

class Human : public Animal {
private:
	const int elixirPower = 5;
	const int maxCooldown = 5;
	void specialSkill();
	int skillCooldown;
	int bonusStrength;
public:
	Human();
	Human(World* w);
	~Human();
	Field* getField(bool empty = false) override;
	void action() override;
	void destroy() override;
	std::string getData() override;
	void setCooldown(int x);
	void setBonusStrength(int x);
};