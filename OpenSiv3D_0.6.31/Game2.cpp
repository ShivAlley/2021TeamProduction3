#include "Game2.h"
Game2::Game2(const InitData& init)
	:IScene(init)
{

}

void Game2::update()
{

	ClearPrint();
	
	
}

void Game2::draw() const
{
	
	


	for (int32 x = 0; x < 50; ++x)
	{
		Line(x * TILECHIP, 0, x * TILECHIP, Scene::Height()).draw(1, Palette::Cyan);
	}
	for (int32 y = 0; y < 50; ++y)
	{
		Line(0, y * TILECHIP, Scene::Width(), y * TILECHIP).draw(1, Palette::Cyan);
	}
}

void Game2::cutting()
{

	for (auto& p : cutRating)
	{
		s3d::Print << p;
	}

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
	if (not cutNow)
	{
		if (cutFoods.isEmpty())
		{
			StateMachine.pop();
			return;
		}
		nowCutting = cutFoods.front();
		cutNow = true;
	}
	if (nowCutting.intersectsAt(cutLine))
	{
		recievePoint = nowCutting.intersectsAt(cutLine);
		bool isVertical = nowCutting.w < nowCutting.h;
		if (isVertical)
		{
			if (recievePoint.has_value() and recievePoint->size() > 1)
			{
				cutRating << nowCutting.h - AbsDiff(recievePoint.value().at(0).y,
													recievePoint.value().at(1).y);
			}
			else
			{
				cutRating << 200;
			}
		}
		else
		{
			if (recievePoint.has_value() and recievePoint->size() > 1)
			{
				cutRating << nowCutting.w - AbsDiff(recievePoint.value().at(0).x,
													recievePoint.value().at(1).x);
			}
			else
			{
				cutRating << 200;
			}
		}
		cutNow = false;
		cutFoods.pop_front();
		cutLine = {};
		//recievePoint.reset();
		cutEnd.reset();
	}

}
