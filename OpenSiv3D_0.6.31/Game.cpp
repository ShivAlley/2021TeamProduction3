#include "Game.h"

void Game::Cutting()
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
	if (cutNow == false)
	{
		nowCutting = cutFoods.choice();
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
		}
		else
		{
			if (recievePoint.has_value() and recievePoint->size() > 1)
			{
				cutRating << nowCutting.w - AbsDiff(recievePoint.value().at(0).x,
													recievePoint.value().at(1).x);
				
			}
		}
		cutNow = false;
		cutLine = {};
		//recievePoint.reset();
		cutEnd.reset();
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
	//気合ゾーン---------------
	//左から縦４つ
	cutFoods << Rect{ Arg::center(Scene::Center().x - TILECHIP + QUARTER_TILECHIP,Scene::Center().y + HALF_TILECHIP), 192 / 4, 192 };
	cutFoods << Rect{ Arg::center(Scene::Center().x - TILECHIP + QUARTER_TILECHIP + HALF_TILECHIP,Scene::Center().y + TILECHIP / 2), 192 / 4, 192 };
	cutFoods << Rect{ Arg::center(Scene::Center().x - TILECHIP + QUARTER_TILECHIP + TILECHIP,Scene::Center().y + HALF_TILECHIP), 192 / 4, 192 };
	cutFoods << Rect{ Arg::center(Scene::Center().x - TILECHIP + QUARTER_TILECHIP + TILECHIP + HALF_TILECHIP,Scene::Center().y + HALF_TILECHIP), 192 / 4, 192 };
	//上から横４つ
	cutFoods << Rect{ Arg::center(Scene::Center().x,Scene::Center().y - QUARTER_TILECHIP), 192, 192 / 4 };
	cutFoods << Rect{ Arg::center(Scene::Center().x,Scene::Center().y + QUARTER_TILECHIP), 192, 192 / 4 };
	cutFoods << Rect{ Arg::center(Scene::Center().x,Scene::Center().y + HALF_TILECHIP + QUARTER_TILECHIP), 192, 192 / 4 };
	cutFoods << Rect{ Arg::center(Scene::Center().x,Scene::Center().y + TILECHIP + QUARTER_TILECHIP), 192, 192 / 4 };
	//---------------------
}

void Game::update()
{
	ClearPrint();
	for (auto p : neutralComments)
	{
		s3d::Print << p.text;
		s3d::Print << p.textPos;
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
		if (ref.textPos.has_value())
		{
			ref.textPos->x -= (Scene::Width() - ref.text.size() * ref.font.fontSize()) / 5 * Scene::DeltaTime();
			RectF rect = ref.font(ref.text).region(ref.textPos.value().x, ref.textPos.value().y);
			rect.stretched(ref.text.size() * ref.font.fontSize());
			if (ref.font(ref.text).region(ref.textPos.value().x, ref.textPos.value().y).leftClicked())
			{
				{
					ref.isFlow = false;
					ref.textPos.reset();
				}
			}
		}

#if 0

	
	/*if (4s > stopWatch)
	{
		return;
	}*/
	
#endif

	

	//Cutting();


	
	for (auto& map : foodManager)//個々に対しての処理
	{
		if (map.second.collision.leftClicked())
		{
			map.second.isGrab = true;
		}
	}
	if (MouseL.up())//全体に対しての処理
	{
		for (auto& it : foodManager)
		{
			it.second.isGrab = false;
		}
	}
	for (auto& map : foodManager)//個々に対して
	{
		if (map.second.isGrab)
		{
			map.second.collision.moveBy(Cursor::Delta());
		}
		if (map.second.collision.intersects(cookPotCollider) and not map.second.isGrab)
		{
			map.second.isinto = true;
		}
	}

	//食材が全て鍋に入っていればTrue
	bool isAllin = std::all_of(foodManager.begin(), foodManager.end(), [](std::pair<String, Foods> food) {return food.second.isinto; });

	if (isAllin)
	{
		if (rotatePotCollider.intersects(spoon) and MouseL.pressed())
		{
			ang += Abs(Cursor::DeltaF().x + Cursor::DeltaF().y) * Scene::DeltaTime();
			rotateRating += Abs(Cursor::DeltaF().x + Cursor::DeltaF().y) * Scene::DeltaTime();
		}
	}
	
	spoon.center = Cursor::Pos();
	if (MouseL.pressed() and spoon.intersects(cutSample) and frypanCollider.contains(cutSample))
	{
		cutSample.moveBy(Cursor::Delta());
	}
	if (not frypanCollider.contains(cutSample))
	{
		cutSample.moveBy(-Cursor::Delta());
	}
}

void Game::draw()const
{
	constexpr int32 textMergin = 40;
	//Rect(Arg::topCenter(Scene::Center().x, TILECHIP * 2), TILECHIP * 10, TILECHIP * 7).draw();
	TextureAsset(U"woodBoard").drawAt(Scene::Center());
	//RectMerginSection
	//Rect(Arg::topLeft(0, 0), TILECHIP * 2).draw();
	//Rect(Arg::topRight(Scene::Width(), 0), TILECHIP * 2).draw();
	//Rect(Arg::bottomLeft(0, Scene::Height()), TILECHIP * 2).draw();
	//Rect(Arg::bottomRight(Scene::Width(), Scene::Height()), TILECHIP * 2).draw();

	//Rect(20, 20, 400, 300).draw();//taskViewRect

	//testCookPot.draw();
	const uint64 t = Time::GetMillisec();
	const int32 n = (t / 250 % 4);

	TextureAsset(U"animatedCookPot")((patterns[n]) * 768, 0, 768, 768)
		.scaled(1.25, 1.0)
		.rotated(ang)
		.drawAt(Scene::Center());
	//if (not pasta.isinto)
	if(not foodManager.at(U"pasta").isinto)
	{
		TextureAsset(U"rawPasta").drawAt(foodManager.at(U"pasta").collision.center());
	}
	//if (not salt.isinto)
	if (not foodManager.at(U"salt").isinto)
	{
		TextureAsset(U"salt").drawAt(foodManager.at(U"salt").collision.center());
	}
	else
	{
		TextureAsset(U"dish").draw(Arg::center(Scene::Center().x - TILECHIP * 6, Scene::Center().y + TILECHIP / 2 - TILECHIP * 3));
	}



	//HACK:円弧が重なってマウスが動いている時鍋が回転するところから



	frypanCollider.draw(ColorF(Palette::Brown,0.5f));
	cutSample.draw(Palette::Coral);
	spoon.draw(ColorF(Palette::Red,0.5f));
	cookPotCollider.draw(ColorF(Palette::Navy, 0.5f));
	rotatePotCollider.draw();
#if 0 //CuttingDraw


	onion.collision.draw(Palette::Pink);
	texOnion.drawAt(onion.collision.center());
	for (const auto& ref : cutFoods)
	{
		ref.draw(ColorF(Palette::Black,0.5f));
	}
	nowCutting.draw(ColorF(Palette::Deepskyblue, 0.5f));
	if (recievePoint)
	{
		for (auto& a : *recievePoint)
		{
			Circle(a, 4).draw(Palette::Red);
		}
	}
	cutLine.draw(Palette::Orange);
#endif // 0

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
