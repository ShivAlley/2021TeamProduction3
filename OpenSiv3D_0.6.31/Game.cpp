#include "Game.h"

Game::Game(const InitData& init)
	:IScene(init)
{

}
#define DezSamp 96
void Game::update()
{
	ClearPrint();
	Print << stopWatch.sF();
	if (4s > stopWatch)
	{
		return;
	}

	
	//Rect{ Arg::center(Cursor::Pos()), DezSamp }.draw(ColorF{ 1.0, 0.0, 0.0, 0.5 });
	//Circle(Cursor::Pos(), DezSamp).draw(Palette::Lightblue);
	

	
}

void Game::draw()const
{
	Rect(Arg::topCenter(Scene::Center().x,DezSamp * 2), 960,DezSamp * 6).draw();
	Rect(Arg::topLeft(0,0), DezSamp).draw();
	Rect(Arg::topRight(Scene::Width(),0), DezSamp).draw();
	Rect(Arg::bottomLeft(0,Scene::Height()), DezSamp).draw();
	Rect(Arg::bottomRight(Scene::Width(),Scene::Height()), DezSamp).draw();
	
	Circle(Scene::Center().x,Scene::Center().y + DezSamp / 2 , DezSamp * 3).draw(Palette::Grey);
	for (int32 x = 0; x < 50; ++x)
	{
		Line(x * DezSamp, 0, x * DezSamp, Scene::Height()).draw(1,Palette::Cyan);
	}
	for (int32 y = 0; y < 50; ++y)
	{
		Line(0,y*DezSamp,Scene::Width(),y*DezSamp).draw(1, Palette::Cyan);
	}
	//Circle(Scene::Center().x,Scene::Center().y , DezSamp * 4).draw(Palette::Gold);
	//AltCookCircle
}
	/*for (int32 y = 0; y < 50; ++y)
	{
		for (int32 x = 0; x < 50; ++x)
		{
			if ((x + y) % 2)
			{
				Rect{ x * 100, y * 100, 100 }.draw(ColorF{ 0.2, 0.3, 0.4 });
			}
		}
	}*/
