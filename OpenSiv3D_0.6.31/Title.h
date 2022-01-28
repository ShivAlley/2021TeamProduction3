#pragma once
#include "common.h"
class Title :
	public App::Scene
{
public:
	Title(const InitData& init);
	void update()override;
	void draw() const override;
private:
	const Font font{ 50, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"UDDigiKyokashoN-R.ttc" };
	RectF selectStage{ Arg::center(Scene::Center().x + TILECHIP * 6,Scene::Center().y - TILECHIP),TILECHIP * 6,TILECHIP };
	RectF exitGame{ Arg::center(Scene::Center().x + TILECHIP * 6,Scene::Center().y + TILECHIP * 4),TILECHIP * 6,TILECHIP };
	bool onTheselect = false;
	bool onTheExit = false;
};

