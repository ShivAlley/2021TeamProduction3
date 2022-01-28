#include "Title.h"
Title::Title(const InitData& init)
	:IScene(init)
{
	AudioAsset(U"title").play();
}

void Title::update()
{
	//野蛮なコード
	if (selectStage.mouseOver())
	{
		onTheselect = true;
	}
	else
	{
		onTheselect = false;
	}
	if (exitGame.mouseOver())
	{
		onTheExit = true;
	}
	else
	{
		onTheExit = false;
	}
	if (selectStage.leftClicked())
	{
		AudioAsset(U"title").stop();
		changeScene(GameState::Menu);
	}
	if (exitGame.leftClicked())
	{
		System::Exit();
	}
}

void Title::draw() const
{
	TextureAsset(U"title").drawAt(Scene::Center());
	selectStage.draw(Palette::Blueviolet);
	if (onTheselect)
	{
		selectStage.drawFrame(10, Palette::Orange);
	}
	exitGame.draw(Palette::Blueviolet);
	if (onTheExit)
	{
		exitGame.drawFrame(10, Palette::Orange);
	}
	font(U"ステージ選択").drawAt(selectStage.center());
	font(U"ゲーム終了").drawAt(exitGame.center());
}
