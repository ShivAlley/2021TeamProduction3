#include "Game.h"

void Game::intro()
{
	if (5.0 < stopWatch.s())
	{
		transition.update(true);
		if (8.0 < stopWatch.s())
		{
			StateMachine.pop();
			cookingTask.pop();
			return;
		}
	}

}

void Game::Cutting()
{
	
	for (auto& p : cutRating)
	{
		//s3d::Print << p;
	}

	if (MouseL.down())
	{
		cutBegin = Cursor::Pos();
		cutEnd.reset();
	}
	if (MouseL.up())
	{
		cutEnd = Cursor::Pos();
		AudioAsset(U"cut").playOneShot();
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
			cookingTask.pop();
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
	AudioAsset(U"boil").play();
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
			map.second.collision.setPos(-1000, -1000);
			AudioAsset(U"powder").playOneShot();

			
		}
	}

	spoon.center = Cursor::Pos();
	//食材が全て鍋に入っていればTrue
	//bool isAllin = std::all_of(foodManager.begin(), foodManager.end(), [](std::pair<String, Foods> food) {return food.second.isinto; });

	isAllin = std::ranges::all_of(foodManager, [](std::pair<String, Foods> food) {return food.second.isinto; });
	
	if (isAllin)
	{
		auto func = [&]() {cookingTask.pop(); };
		static bool callOnce = [&]() { func(); return true; }();
		auto func2 = [&]() {stopWatchRotate.restart(); };
		static bool callOnce2 = [&]() { func2(); return true; }();
		if (rotatePotCollider.intersects(spoon) and MouseL.pressed())
		{
			ang += Abs(Cursor::DeltaF().x + Cursor::DeltaF().y);
			rotateRating += Abs(Cursor::DeltaF().x + Cursor::DeltaF().y);
			if (rotateRating > 50000 or stopWatchRotate.s() > 10)
			{
				StateMachine.pop();
				cookingTask.pop();
				AudioAsset(U"boil").stop();
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
			AudioAsset(U"stir").play();
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
		if (cutRating.sum() < 1000)
			getData().scorePepe++;
		if (rotateRating > 20000)
			getData().scorePepe++;
		if (burnRating.sum() < 300 * 50)
			getData().scorePepe++;
		AudioAsset(U"game").stop();
		changeScene(GameState::Result);
		return;
	}
}


void Game::FlowComments()
{
	for (auto& p : neutralComments)
	{
		if (p.textPos.has_value())
		{
			if (p.textPos.value().x < -static_cast<double>(font.fontSize() * p.text.size()))
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
			it.textPos->x -= (Scene::Width() - it.text.size() * font.fontSize()) / 5 * Scene::DeltaTime();
			RectF rect = font(it.text).region(it.textPos.value().x, it.textPos.value().y);
			rect.stretched(it.text.size() * font.fontSize());
			if (font(it.text).region(it.textPos.value().x, it.textPos.value().y).leftClicked()
				and font(it.text).region(it.textPos.value().x, it.textPos.value().y).rightPressed())
			{
				{
					it.isFlow = false;
					it.textPos.reset();
				}
			}
		}
	}

	for (auto& p : angryComments)
	{
		if (p.textPos.has_value())
		{
			if (p.textPos.value().x < -static_cast<double>(font.fontSize() * p.text.size()))
			{
				p.textPos.reset();
				p.isFlow = false;
			}
		}
	}
	if (commentsWatchangry.s() == 20)
	{
		
		commentsWatchangry.restart();
		if (not angryComments.isEmpty())
		{
			//ラムダ式の参照渡しでコメントコンテナの要素を変更してその返り値で
			//無限ループにならないようにしたWhile文を回し、かならず1秒に一回コメントが流れるようにしている
			std::function<bool()> PostComment = [&]() -> bool
			{
				if (auto& post = angryComments.choice(); post.isFlow == false)
				{
					post.isFlow = true;
					post.textPos = { Scene::Width(), Random<int32>(0, Scene::Height() - 90) };
					return false;
				}
				return true;
			};

			while (PostComment())
			{
				if (not angryComments.any([](Comments comm) {return comm.isFlow == false; }))
				{
					break;
				}
			}
		}
	}
	for (auto& it : angryComments)
	{
		if (it.textPos.has_value())
		{
			it.textPos->x -= (Scene::Width() - it.text.size() * font.fontSize()) / 5 * Scene::DeltaTime();
			RectF rect = font(it.text).region(it.textPos.value().x, it.textPos.value().y);
			rect.stretched(it.text.size() * font.fontSize());
			if (font(it.text).region(it.textPos.value().x, it.textPos.value().y).leftClicked()
				and font(it.text).region(it.textPos.value().x, it.textPos.value().y).rightPressed())
			{
				{
					it.isFlow = false;
					it.textPos.reset();
				}
			}
		}
	}

	for (auto& p : neutralComments)
	{
		if (p.textPos.has_value())
		{
			if (p.textPos.value().x < -static_cast<double>(font.fontSize() * p.text.size()))
			{
				p.textPos.reset();
				p.isFlow = false;
			}
		}
	}
	if (commentsWatchhappy.s() == 10)
	{
		commentsWatchhappy.restart();
		if (not happyComments.isEmpty())
		{
			//ラムダ式の参照渡しでコメントコンテナの要素を変更してその返り値で
			//無限ループにならないようにしたWhile文を回し、かならず1秒に一回コメントが流れるようにしている
			std::function<bool()> PostComment = [&]() -> bool
			{
				if (auto& post = happyComments.choice(); post.isFlow == false)
				{
					post.isFlow = true;
					post.textPos = { Scene::Width(), Random<int32>(0, Scene::Height() - 90) };
					return false;
				}
				return true;
			};

			while (PostComment())
			{
				if (not happyComments.any([](Comments comm) {return comm.isFlow == false; }))
				{
					break;
				}
			}
		}
	}
	for (auto& it : happyComments)
	{
		if (it.textPos.has_value())
		{
			it.textPos->x -= (Scene::Width() - it.text.size() * font.fontSize()) / 2 * Scene::DeltaTime();
			RectF rect = font(it.text).region(it.textPos.value().x, it.textPos.value().y);
			rect.stretched(it.text.size() * font.fontSize());
			if (font(it.text).region(it.textPos.value().x, it.textPos.value().y).leftClicked()
				and font(it.text).region(it.textPos.value().x, it.textPos.value().y).rightPressed())
			{
				{
					it.isFlow = false;
					it.textPos.reset();
				}
			}
		}
	}
}

void Game::introDraw() const
{
	double t = transition.value();
	
	TextureAsset(U"peperoncino").draw();
	font(U"きょうの料理").drawAt(Scene::Center().x, Scene::Center().y - TILECHIP * 5);
	font(U"ペペロンチーノ").drawAt(Scene::Center().x, Scene::Center().y - TILECHIP * 3 - TILECHIP / 2);
	Circle{ Scene::Center(), (t * 1200) }.draw(Palette::Black);
}

void Game::CuttingDraw()const
{
	TextureAsset(U"backWood").drawAt(Scene::Center());
	TextureAsset(U"woodBoard").drawAt(Scene::Center());
	TextureAsset(U"garlic").drawAt(garlic.collision.center());
	
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
	for (const auto& it : neutralComments)
	{
		if (it.textPos.has_value())
			font(it.text).draw(it.textPos.value().x, it.textPos.value().y);
	}
	for (const auto& it : happyComments)
	{
		if (it.textPos.has_value())
			font(it.text).draw(it.textPos.value().x, it.textPos.value().y,Palette::Yellow);
	}
	for (const auto& it : angryComments)
	{
		if (it.textPos.has_value())
			font(it.text).draw(it.textPos.value().x, it.textPos.value().y,Palette::Red);
	}
}

void Game::entryCookingPotDraw()const
{
	TextureAsset(U"IH").drawAt(Scene::Center());
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
	//rotatePotCollider.draw();
}

void Game::stirDraw()const
{
	TextureAsset(U"IH").drawAt(Scene::Center());
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
	AudioAsset(U"game").play();
	getData().scorePepe = 0;
	//auto f = [this]() {intro(); this->ease; };
	//switchFunction2[State::intro] = f;
	
	{
		TextReader reader{ Resource(U"comments/neutralComments.txt") };
		Comments temptxt;
		while (reader.readLine(temptxt.text))
		{
			neutralComments << temptxt;
		}
	}
	{
		TextReader reader{ Resource(U"comments/happyComments.txt") };
		Comments temptxt;
		while (reader.readLine(temptxt.text))
		{
			happyComments << temptxt;
		}
	}
	{
		TextReader reader{ Resource(U"comments/angryComments.txt") };
		Comments temptxt;
		while (reader.readLine(temptxt.text))
		{
			angryComments << temptxt;
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
	{
		Task tempTask{ U"",0 };
		cookingTask.push(tempTask);
		tempTask = Task{ U"にんにくを\nみじん切りにする",1 };
		cookingTask.push(tempTask);
		tempTask = Task{ U"岩塩とパスタを\n鍋へブチ込む",2 };
		cookingTask.push(tempTask);
		tempTask = Task{ U"鍋ごとブン回して\nかき混ぜる",3 };
		cookingTask.push(tempTask);
		tempTask = Task{ U"全部に火が通るように炒める",4 };
		cookingTask.push(tempTask);
		tempTask = Task{ U"これはエラーです\nnoexpect",5 };
		cookingTask.push(tempTask);
	}

	
	
	//今後の研究
	//いけるswitchFunction[State::cutting] = std::bind(&Game::Cutting, this);
	//むりswitchFunction2[State::cutting] = std::bind(&Game::Cutting, this);
}

void Game::update()
{

	ClearPrint();

	//Print << static_cast<int32>(StateMachine.front());
	switch (StateMachine.front())
	{
	case State::intro:
		intro();
		break;
	case State::cutting:
		Cutting();
		break;
	case State::cutToEntryInterval:
	{
		auto func = [&]() {stopWatchEase.start(); };
		static bool callOnce = [&]() { func(); return true; }();
		trans = Min(stopWatchEase.sF(), 1.0);
		ease = EaseInOutBack(trans);
		EasePos = fromWood.lerp(toWood, ease);

		easeCookPot = EaseInOutBack(trans);
		EasePosCookPot = fromCookPot.lerp(toCookPot, easeCookPot);

		easeSalt = EaseInOutBack(trans);
		EasePosSalt = fromSalt.lerp(toSalt, easeSalt);

		easePasta = EaseInOutBack(trans);
		EasePosPasta = fromPasta.lerp(toPasta, easePasta);

		if (stopWatchEase.sF() > 2.0)
		{
			StateMachine.pop();
		}
		break;
	}
	/*TextureAsset(U"woodBoard").drawAt(Scene::Center());
	TextureAsset(U"garlic").drawAt(garlic.collision.center());*/
	case State::entryCookingPot:
		entryCookingPot();
		break;
	case State::entryToStirInterval:
	{
		auto func = [&]() {stopWatchEase.restart(); };
		static bool callOnce = [&]() { func(); return true; }();
		trans = Min(stopWatchEase.sF(), 1.0);
		easeFrypan = EaseInOutBack(trans);
		EasePosFrypan = fromFrypan.lerp(toFrypan, easeFrypan);

		easeDish = EaseInOutBack(trans);
		EasePosDish = fromDish.lerp(toDish, easeDish);

		easeFadeoutCookPot = EaseInOutBack(trans);
		EaseFadeoutPosCookPot = fromFadeoutCookPot.lerp(toFadeoutCookPot, easeFadeoutCookPot);


		if (stopWatchEase.sF() > 2.0)
		{
			StateMachine.pop();
			break;
		}
		break;
	}
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


	switch (StateMachine.front())
	{
	case State::intro:
		introDraw();
		break;
	case State::cutting:
		CuttingDraw();
		break;
	case State::cutToEntryInterval:
	{
		TextureAsset(U"backWood").drawAt(EasePos);
		TextureAsset(U"woodBoard").drawAt(EasePos);
		TextureAsset(U"emptyFrypan").drawAt(EasePosCookPot);
		TextureAsset(U"IH").drawAt(EasePosCookPot);
		TextureAsset(U"salt").drawAt(EasePosSalt);
		TextureAsset(U"rawPasta").drawAt(EasePosPasta);
		//TextureAsset(U"garlic").drawAt(garlic.collision.center());
		break;
	}
	case State::entryCookingPot:
		entryCookingPotDraw();
		break;
	case State::entryToStirInterval:
	{
		TextureAsset(U"IH").drawAt(EasePosFrypan);
		TextureAsset(U"frypanHand").drawAt(EasePosFrypan + Point(0,TILECHIP * 4));
		TextureAsset(U"onFoodFrypan").drawAt(EasePosFrypan);
		TextureAsset(U"pastaInCookPot").scaled(1.25,1.0).drawAt(EaseFadeoutPosCookPot);
		TextureAsset(U"dish").drawAt(EasePosDish);

		break;
	}
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
	if(cookingTask.front().Progress not_eq 0)
		Rect(20, 20, 400, 300).draw(HSV(142, 0.42, 0.28,0.6));//taskViewRect//黒板色
	

	switch (cookingTask.front().Progress)
	{
	case 0:
		break;
	case 1:
		font(cookingTask.front().cookingProcess)
			.draw(cookingTask.front().taskView, HSV(140, 0.03, 0.85));//ColorIron
		break;
	case 2:
		font(cookingTask.front().cookingProcess)
			.draw(cookingTask.front().taskView, HSV(140, 0.03, 0.85));
		break;
	case 3:
		font(cookingTask.front().cookingProcess)
			.draw(cookingTask.front().taskView, HSV(140, 0.03, 0.85));
		break;
	case 4:
		font(cookingTask.front().cookingProcess)
			.draw(cookingTask.front().taskView, HSV(140, 0.03, 0.85));
		break;
	default:
		break;
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
