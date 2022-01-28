#pragma once
#include "common.h"
class Menu :public App::Scene
{
public:
	Menu(const InitData& init);
	void update()override;
	void draw() const override;
private:
	Rect pepe{ Arg::center(Scene::Center().x / 2,Scene::Center().y),400 };
	Rect romi{ Arg::center(Scene::Center().x + Scene::Center().x / 2,Scene::Center().y),400 };
	double scale = 0;
	double scale2 = 0;
};

