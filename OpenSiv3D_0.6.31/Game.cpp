#include "Game.h"

void Game::intro()
{

}

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

void Game::entryCookingPot()
{
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

	spoon.center = Cursor::Pos();
	//食材が全て鍋に入っていればTrue
	//bool isAllin = std::all_of(foodManager.begin(), foodManager.end(), [](std::pair<String, Foods> food) {return food.second.isinto; });
	isAllin = std::ranges::all_of(foodManager, [](std::pair<String, Foods> food) {return food.second.isinto; });
	if (isAllin)
	{
		if (rotatePotCollider.intersects(spoon) and MouseL.pressed())
		{
			ang += Abs(Cursor::DeltaF().x + Cursor::DeltaF().y) * Scene::DeltaTime();
			rotateRating += Abs(Cursor::DeltaF().x + Cursor::DeltaF().y) * Scene::DeltaTime();
			if (rotateRating > 1000)
			{
				StateMachine.pop();
				return;
			}
		}
	}
}

void Game::stir()
{
	spoon.center = Cursor::Pos();
	for (auto& it : intoFoods)
	{
		if (MouseL.pressed() and spoon.intersects(it.collider) and frypanCollider.contains(it.collider))
		{
			it.collider.moveBy(Cursor::Delta());
			it.brightness -= Abs(Cursor::DeltaF().x + Cursor::DeltaF().y) * Scene::DeltaTime() / 1000;
			//Print << it.moveLength;

			if (it.brightness < 0)
			{
				it.brightness = 0;
			}
			///あとでループの外で一度だけ範囲を回す
			//burnRating << AbsDiff(it.brightness, 0.40) * 1000;
		}
		if (not frypanCollider.contains(it.collider))
		{
			it.collider.moveBy(-Cursor::Delta());
		}
	}
	if (intoFoods.all([](inFood food) {return food.brightness < 0.40; }))
	{
		for (auto& it : intoFoods)
		{
			burnRating << AbsDiff(it.brightness, 0.40) * 1000;
		}
		StateMachine.pop();
		return;
	}
}

void Game::FlowComments()
{
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
	for (auto& it : neutralComments)
	{
		if (it.textPos.has_value())
		{
			it.textPos->x -= (Scene::Width() - it.text.size() * it.font.fontSize()) / 5 * Scene::DeltaTime();
			RectF rect = it.font(it.text).region(it.textPos.value().x, it.textPos.value().y);
			rect.stretched(it.text.size() * it.font.fontSize());
			if (it.font(it.text).region(it.textPos.value().x, it.textPos.value().y).leftClicked()
				and it.font(it.text).region(it.textPos.value().x, it.textPos.value().y).rightPressed())
			{
				{
					it.isFlow = false;
					it.textPos.reset();
				}
			}
		}
	}
}

void Game::CuttingDraw()const
{
	TextureAsset(U"woodBoard").drawAt(Scene::Center());
	TextureAsset(U"onion").drawAt(onion.collision.center());
	for (const auto& it : cutFoods)
	{
		it.draw(ColorF(Palette::Black, 0.5f));
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
}

void Game::FlowCommentsDraw()const
{
	for (auto& ref : neutralComments)
	{
		if (ref.textPos.has_value())
			ref.font(ref.text).draw(ref.textPos.value().x, ref.textPos.value().y);
	}
}

void Game::entryCookingPotDraw()const
{
	const uint64 t = Time::GetMillisec();
	const int32 n = (t / 250 % 4);
	if (not isAllin and foodManager.at(U"pasta").isinto)
	{
		TextureAsset(U"rawPastaInCookPot")
			.scaled(1.25, 1.0)
			.drawAt(Scene::Center());
	}
	if(not isAllin and not foodManager.at(U"pasta").isinto)
	{
		TextureAsset(U"animatedCookPot")((patterns[n]) * 768, 0, 768, 768)
			.scaled(1.25, 1.0)
			.drawAt(Scene::Center());
	}
	if (isAllin)
	{
		TextureAsset(U"pastaInCookPot")
			.scaled(1.25, 1.0)
			.rotated(ang)
			.drawAt(Scene::Center());
	}
	//if (not pasta.isinto)
	if (not foodManager.at(U"pasta").isinto)
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
		TextureAsset(U"dish").draw(Arg::center(Scene::Center().x - TILECHIP * 6,Scene::Center().y - TILECHIP / 2 + TILECHIP * 3));
	}
	rotatePotCollider.draw();
}

void Game::stirDraw()const
{
	TextureAsset(U"emptyFrypan").drawAt(frypanCollider.center);
	TextureAsset(U"onFoodFrypan").drawAt(cookPotCollider.center,ColorF(1,1,1,0.3));
	TextureAsset(U"frypanHand").drawAt(cookPotCollider.center + Point(0,TILECHIP * 6 - TILECHIP / 2));
	for (const auto& it : intoFoods)
	{
		it.collider.draw(HSV(0, 0.75, it.brightness));
	}
	spoon.draw(ColorF(Palette::Red, 0.5f));
	//frypanCollider.draw(ColorF(Palette::Brown,0.5f));
	//cookPotCollider.draw(ColorF(Palette::Navy, 0.5f));
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
	//ここから気合ゾーン---------------
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
	//気合ゾーンここまで---------------------
	for (auto p : step(50))
	{
		Circle tempCircle = frypanCollider;
		tempCircle.r -= TILECHIP;
		inFood temp = {
			{ Arg::center(RandomVec2(tempCircle)),TILECHIP },
			1.0,
		};
		intoFoods << temp;
	}
	for (auto p : step(static_cast<int32>(State::NUM)))
	{
		State st = static_cast<State>(p);
		StateMachine.push(st);
	}
	//今後の研究
	//switchFunction[State::cutting] = std::bind(&Game::Cutting, this);
}
void Game::update()
{
#if 0
	/*if (4s > stopWatch)
	{
		return;
	}*/

#endif
	ClearPrint();

	Print << static_cast<int32>(StateMachine.front());
	switch (StateMachine.front())
	{
	case State::intro:
		StateMachine.pop();
		break;
	case State::cutting:
		Cutting();
		break;
	case State::cutToEntryInterval:
		StateMachine.pop();
		break;
	case State::entryCookingPot:
		entryCookingPot();
		break;
	case State::EntryToStirInterval:
		StateMachine.pop();
		break;
	case State::stir:
		stir();
		break;
	default:
		break;
	}

	FlowComments();
	//
	//Cutting();
	//
	//entryCookingPot();
	//
	//stir();
}

void Game::draw()const
{
	constexpr int32 textMergin = 40;

	//Rect(20, 20, 400, 300).draw();//taskViewRect

	switch (StateMachine.front())
	{
	case State::intro:
		break;
	case State::cutting:
		CuttingDraw();
		break;
	case State::cutToEntryInterval:
		break;
	case State::entryCookingPot:
		entryCookingPotDraw();
		break;
	case State::EntryToStirInterval:
		break;
	case State::stir:
		stirDraw();
		break;
	default:
		break;
	}

	//CuttingDraw();

	//entryCookingPotDraw();

	//stirDraw();

	FlowCommentsDraw();

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
