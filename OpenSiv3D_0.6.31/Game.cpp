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
}

Game::Game(const InitData& init)
	:IScene(init)
{
	{
		TextReader reader{ U"comments/normalComments.txt" };
		Comments temptxt;
		while (reader.readLine(temptxt.text))
		{
			neutralComments << temptxt;
		}
	}

	cutFoods << onion.collision;
}

void Game::update()
{
	ClearPrint();
	for (auto p : neutralComments)
	{
		Print << p.text;
		Print << p.textPos;
	}
	for (auto& p : neutralComments)
	{
		if (p.textPos.has_value())
		{
			if (p.textPos.value().x < -static_cast<double>(p.font.fontSize() * p.text.size()))
			{
				p.textPos.reset();
				p.isFlow = false;
			}
		}
	}
	if (commentsWatch.s() == 1)
	{
		commentsWatch.restart();
		if (not neutralComments.isEmpty())
		{
			//ラムダ式の参照渡しでコメントコンテナの要素を変更してその返り値で
			//無限ループにならないようにしたWhile文を回し、かならず1秒に一回コメントが流れるようにしている
			std::function<bool()> PostComment = [&]() -> bool
			{
				if (auto& post = neutralComments.choice(); post.isFlow == false)
				{
					post.isFlow = true;
					post.textPos = { Scene::Width(), Random<int32>(0, Scene::Height() - 90) };
					return false;
				}
				return true;
			};

			while (PostComment())
			{
				if (not neutralComments.any([](Comments comm) {return comm.isFlow == false; }))
				{
					break;
				}
			}
		}
	}
	for (auto& ref : neutralComments)
	{
		if (ref.textPos.has_value())
		{
			ref.textPos->x -= (Scene::Width() - ref.text.size() * ref.font.fontSize()) / 5 * Scene::DeltaTime();
			RectF rect = ref.font(ref.text).region(ref.textPos.value().x, ref.textPos.value().y);
			rect.stretched(ref.text.size() * ref.font.fontSize());
			if (ref.font(ref.text).region(ref.textPos.value().x, ref.textPos.value().y).leftClicked())
			{
				ref.isFlow = false;
				ref.textPos.reset();
			}
		}
	}

#if 0
	Print << stopWatch.sF();
	Print << textPos.x;
	Print << textPosLong.x;
	Print << -Scene::Width() - flowTextLong.size() * font.fontSize() / 5;
	Print << flowTextLong.size();
	/*if (4s > stopWatch)
	{
		return;
	}*/

	textPos.x -= (Scene::Width() + flowTextGrass.size() * font.fontSize()) / 5 * Scene::DeltaTime();
	textPosLong.x -= (Scene::Width() + flowTextLong.size() * font.fontSize()) / 5 * Scene::DeltaTime();
#endif

	//spoon.center = Cursor::Pos();

	Cutting();

	if (MouseL.pressed() and spoon.intersects(cutSample) and cookingPot.contains(cutSample))
	{
		cutSample.moveBy(Cursor::Delta());
	}
	if (not cookingPot.contains(cutSample))
	{
		cutSample.moveBy(-Cursor::Delta());
	}
}

void Game::draw()const
{
	constexpr int32 textMergin = 40;
	Rect(Arg::topCenter(Scene::Center().x, TILECHIP * 2), TILECHIP * 10, TILECHIP * 7).draw();
	Rect(Arg::topLeft(0, 0), TILECHIP * 2).draw();
	Rect(Arg::topRight(Scene::Width(), 0), TILECHIP * 2).draw();
	Rect(Arg::bottomLeft(0, Scene::Height()), TILECHIP * 2).draw();
	Rect(Arg::bottomRight(Scene::Width(), Scene::Height()), TILECHIP * 2).draw();
	Rect(20, 20, 400, 300).draw();

	//cookingPot.draw(Palette::Brown);
	//testCookPot.draw();
	//testCookPot.scaled(1.25,1.0).drawAt(Scene::Center().x,Scene::Center().y + TILECHIP / 2);
	//cutSample.draw(Palette::Coral);
	//spoon.draw(ColorF(Palette::Red,0.5f));
	//onion2.draw(Palette::Pink);
	//oniRect.draw(Palette::Pink);

	onion.collision.draw(Palette::Pink);
	texOnion.drawAt(onion.collision.center());
	for (const auto& ref : cutFoods)
	{
		ref.draw(Palette::Black);
	}
	if (recievePoint)
	{
		for (auto& a : *recievePoint)
		{
			Circle(a, 4).draw(Palette::Red);
		}
	}

	cutLine.draw(Palette::Orange);
	for (auto& ref : neutralComments)
	{
		if (ref.textPos.has_value())
			ref.font(ref.text).draw(ref.textPos.value().x, ref.textPos.value().y);
	}

	for (int32 x = 0; x < 50; ++x)
	{
		Line(x * TILECHIP, 0, x * TILECHIP, Scene::Height()).draw(1, Palette::Cyan);
	}
	for (int32 y = 0; y < 50; ++y)
	{
		Line(0, y * TILECHIP, Scene::Width(), y * TILECHIP).draw(1, Palette::Cyan);
	}
}
//const Point pos = Cursor::Pos();
//if (InRange(pos.x, 0, image.width() - 1)
//		&& InRange(pos.y, 0, image.height() - 1))
//{
//	// マウスカーソルの位置にあるピクセルの色を取得
//	const Color pixelColor = image[pos];
//

//	Rect{ 500, 20, 40 }.draw(pixelColor);

//	PutText(U"{}"_fmt(pixelColor), Arg::topLeft(560, 20));
//}
