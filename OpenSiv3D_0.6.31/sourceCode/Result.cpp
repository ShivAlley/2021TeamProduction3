#include "Result.h"
Result::Result(const InitData& init)
	:IScene(init)
{
	AudioAsset(U"result").playOneShot();
}

void Result::update()
{
	if (stopwatch.s() > 5)
		changeScene(GameState::Title);
}

void Result::draw()const
{
	TextureAsset(U"result").drawAt(Scene::Center());
	TextureAsset(U"resultPepe")
		.scaled(0.5,0.5).drawAt(Scene::Center().x, Scene::Center().y + TILECHIP * 3);
	//TextureAsset(U"star").drawAt(Scene::Center().x - TILECHIP * 2, Scene::Center().y - TILECHIP * 3);
	//TextureAsset(U"star").drawAt(Scene::Center().x, Scene::Center().y - TILECHIP * 3);
	//TextureAsset(U"star").drawAt(Scene::Center().x + TILECHIP * 2, Scene::Center().y - TILECHIP * 3);
	if (getData().scorePepe == 0)
	{

	}
	else if (getData().scorePepe == 1)
	{
		TextureAsset(U"star").drawAt(Scene::Center().x - TILECHIP * 2, Scene::Center().y - TILECHIP * 3);
	}
	else if (getData().scorePepe == 2)
	{
		TextureAsset(U"star").drawAt(Scene::Center().x - TILECHIP * 2, Scene::Center().y - TILECHIP * 3);
		TextureAsset(U"star").drawAt(Scene::Center().x, Scene::Center().y - TILECHIP * 3);

	}
	else if (getData().scorePepe == 3)
	{
		TextureAsset(U"star").drawAt(Scene::Center().x - TILECHIP * 2, Scene::Center().y - TILECHIP * 3);
		TextureAsset(U"star").drawAt(Scene::Center().x, Scene::Center().y - TILECHIP * 3);
		TextureAsset(U"star").drawAt(Scene::Center().x + TILECHIP * 2, Scene::Center().y - TILECHIP * 3);

	}
	


}
