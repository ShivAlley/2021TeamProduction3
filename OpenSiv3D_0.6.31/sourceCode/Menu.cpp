#include "Menu.h"

Menu::Menu(const InitData& init)
	:IScene(init)
{
	AudioAsset(U"stageSelect").play();
}

void Menu::update()
{
	if (romi.leftClicked())
	{
		AudioAsset(U"stageSelect").stop();
		changeScene(GameState::Game2);
	}
	if (pepe.leftClicked())
	{
		AudioAsset(U"stageSelect").stop();
		changeScene(GameState::Game);
	}
	if (romi.mouseOver())
	{
		scale = 2.5;
	}
	else
	{
		scale = 2.0;
	}
	if (pepe.mouseOver())
	{
		scale2 = 2.5;
	}
	else
	{
		scale2 = 2.0;
	}
}

void Menu::draw() const
{
	TextureAsset(U"backWood").drawAt(Scene::Center());
	TextureAsset(U"romiIcon")
		.scaled(scale,scale).drawAt(romi.center());
	TextureAsset(U"pepeIcon")
		.scaled(scale2, scale2).drawAt(pepe.center());
}
