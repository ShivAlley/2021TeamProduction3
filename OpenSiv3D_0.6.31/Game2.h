#pragma once
#include "common.h"
class Game2 : public App::Scene
{
public:
	Game2(const InitData& init);
	void update()override;
	void draw() const override;
private:

	//HACK:Suggestion:Dezsamp*4to3
	Circle cookingPot{ Scene::Center().x, Scene::Center().y + TILECHIP / 2, TILECHIP * 4 };
	struct Foods
	{
		Rect collision;
		bool isinto;
	};
	Foods pasta =
	{
		{Arg::center(Scene::Center().x + TILECHIP * 6,Scene::Center().y + TILECHIP / 2), TILECHIP * 2, TILECHIP * 8 },
		false,
	};
	Foods salt =
	{
		{Arg::center(Scene::Center().x - TILECHIP * 6,Scene::Center().y + TILECHIP / 2 - TILECHIP * 3), TILECHIP * 2 },
		false,
	};

	HashTable<String, bool> isgrab =
	{
		{U"pasta",false},
		{U"salt",false},
	};
private:
	Texture texCookingPot{ U"Image/768x768.png" };
	TextureRegion test = texCookingPot(250, 100, 200, 150);

};

