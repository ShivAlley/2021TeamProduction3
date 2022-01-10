#include "Game.h"

void Game::Cutting()
{
	if (MouseL.down())
	{
		cutBegin = Cursor::Pos();
		cutEnd.reset();
	}
	if (MouseL.up())
	{
		cutEnd = Cursor::Pos();
	}

	if (cutBegin and cutEnd)
	{
		cutLine = { cutBegin.value(),cutEnd.value() };
	}
	recievePoint = onion.intersectsAt(cutLine);
}


Game::Game(const InitData& init)
	:IScene(init)
{
	textPos     = { Scene::Width(),600 };
	textPosLong = { Scene::Width(),700 };
}

void Game::update()
{
	ClearPrint();
	Print << stopWatch.sF();
	Print << textPos.x;
	Print << textPosLong.x;
	Print << -Scene::Width() - flowTextLong.size() * font.fontSize() / 5;
	Print << flowTextLong.size();
	/*if (4s > stopWatch)
	{
		return;
	}*/

	Cutting();
	textPos.x -= (Scene::Width() + flowTextGrass.size() * font.fontSize()) / 5 * Scene::DeltaTime();
	textPosLong.x -= (Scene::Width() + flowTextLong.size() * font.fontSize()) / 5 * Scene::DeltaTime();
	
	
	
	

	
}

void Game::draw()const
{
	constexpr int32 textMergin = 40;
	Rect(Arg::topCenter(Scene::Center().x,TILECHIP * 2), TILECHIP*10,TILECHIP * 7).draw();
	Rect(Arg::topLeft(0,0), TILECHIP*2).draw();
	Rect(Arg::topRight(Scene::Width(),0), TILECHIP*2).draw();
	Rect(Arg::bottomLeft(0,Scene::Height()), TILECHIP*2).draw();
	Rect(Arg::bottomRight(Scene::Width(),Scene::Height()), TILECHIP*2).draw();
	Circle(Scene::Center().x,Scene::Center().y + TILECHIP / 2 , TILECHIP * 4).draw(Palette::Grey);
	Rect(20, 20, 400, 300).draw();
	font(text).draw(Rect(20, 20, 50*5, 140),ColorF(Palette::Orange,0.6f));
	font(flowTextGrass).draw(textPos);
	font(flowTextLong).draw(textPosLong);
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
		Line(x * TILECHIP, 0, x * TILECHIP, Scene::Height()).draw(1,Palette::Cyan);
	}
	for (int32 y = 0; y < 50; ++y)
	{
		Line(0,y*TILECHIP,Scene::Width(),y*TILECHIP).draw(1, Palette::Cyan);
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
