#include "Game.h"
#define DezSamp 96

CookingProgress::CookingProgress()
{
	
	
}

CookingProgress::~CookingProgress()
{
}



void CookingProgress::Cutting()
{
	if (MouseL.down())
	{
		cutBegin = Cursor::Pos();
	}
	if (MouseL.up())
	{
		cutEnd = Cursor::Pos();
	}

	if (cutEnd)
	{
		cutLine = { cutBegin.value(),cutEnd.value() };
	}
	recievePoint = onion.intersectsAt(cutLine);

}

Game::Game(const InitData& init)
	:IScene(init)
{
	//HACK:cookingProgress = new CookingProgress();
}
void Game::update()
{
	ClearPrint();
	Print << stopWatch.sF();
	/*if (4s > stopWatch)
	{
		return;
	}*/
	
	//HACK:cookingProgress->Cutting();
	
	
	
	
	
	

	
}

void Game::draw()const
{
	Rect(Arg::topCenter(Scene::Center().x,DezSamp * 2), DezSamp*10,DezSamp * 7).draw();
	Rect(Arg::topLeft(0,0), DezSamp*2).draw();
	Rect(Arg::topRight(Scene::Width(),0), DezSamp*2).draw();
	Rect(Arg::bottomLeft(0,Scene::Height()), DezSamp*2).draw();
	Rect(Arg::bottomRight(Scene::Width(),Scene::Height()), DezSamp*2).draw();
	Circle(Scene::Center().x,Scene::Center().y + DezSamp / 2 , DezSamp * 4).draw(Palette::Grey);
	onion.draw(Palette::Pink);
	onion2.draw(Palette::Pink);
	if (recievePoint)
		for (auto& a : *recievePoint)
		{
			Circle(a, 4).draw(Palette::Red);
		}
	
	cutLine.draw(Palette::Orange);

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
