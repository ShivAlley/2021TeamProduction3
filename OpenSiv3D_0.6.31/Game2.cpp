#include "Game2.h"
Game2::Game2(const InitData& init)
	:IScene(init)
{

}

void Game2::update()
{

	ClearPrint();
	if (pasta.collision.leftClicked())
	{
		isgrab[U"pasta"] = true;
	}
	if (salt.collision.leftClicked())
	{
		isgrab[U"salt"] = true;
	}
	if (MouseL.up())
	{
		for (auto it = isgrab.begin(); it != isgrab.end(); ++it)
		{
			it->second = false;
		}
	}
	if (isgrab[U"pasta"])
	{
		pasta.collision.moveBy(Cursor::Delta());
	}
	
	if (isgrab[U"salt"])
	{
		salt.collision.moveBy(Cursor::Delta());
	}
	if (pasta.collision.intersects(cookingPot))
	{
		pasta.isinto = true;
	}
	if (salt.collision.intersects(cookingPot))
	{
		salt.isinto = true;
	}
	
}

void Game2::draw() const
{
	
	cookingPot.draw(Palette::Grey);
	pasta.collision.draw(Palette::Coral);
	salt.collision.draw(Palette::Darkgreen);
	//cookingPot.drawAt(Scene::Center().x, Scene::Center().y + DezSamp / 2);
	


	for (int32 x = 0; x < 50; ++x)
	{
		Line(x * TILECHIP, 0, x * TILECHIP, Scene::Height()).draw(1, Palette::Cyan);
	}
	for (int32 y = 0; y < 50; ++y)
	{
		Line(0, y * TILECHIP, Scene::Width(), y * TILECHIP).draw(1, Palette::Cyan);
	}
}
