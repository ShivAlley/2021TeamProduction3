#include "stdafx.h"
#include "Result2.h"

Result2::Result2(const InitData& init)
	:IScene(init)
{
	AudioAsset(U"result").playOneShot();
}

void Result2::update()
{
	if (stopwatch.s() > 5)
		changeScene(GameState::Title);
}

void Result2::draw() const
{
	TextureAsset(U"result").drawAt(Scene::Center());
	TextureAsset(U"resultRomi")
		.scaled(0.5, 0.5).drawAt(Scene::Center().x, Scene::Center().y + TILECHIP * 3);

	if (getData().scoreRomi == 0)
	{

	}
	else if (getData().scoreRomi == 1)
	{
		TextureAsset(U"star").drawAt(Scene::Center().x - TILECHIP * 2, Scene::Center().y - TILECHIP * 3);
	}
	else if (getData().scoreRomi == 2)
	{
		TextureAsset(U"star").drawAt(Scene::Center().x - TILECHIP * 2, Scene::Center().y - TILECHIP * 3);
		TextureAsset(U"star").drawAt(Scene::Center().x, Scene::Center().y - TILECHIP * 3);

	}
	else if (getData().scoreRomi == 3)
	{
		TextureAsset(U"star").drawAt(Scene::Center().x - TILECHIP * 2, Scene::Center().y - TILECHIP * 3);
		TextureAsset(U"star").drawAt(Scene::Center().x, Scene::Center().y - TILECHIP * 3);
		TextureAsset(U"star").drawAt(Scene::Center().x + TILECHIP * 2, Scene::Center().y - TILECHIP * 3);

	}
}
