#pragma once
#include "common.h"
class Game2 : public App::Scene
{
public:
	Game2(const InitData& init);
	void update()override;
	void draw() const override;
private:

	void cutting();

};

