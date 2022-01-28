#include "Game2.h"
Game2::Game2(const InitData& init)
	:IScene(init)
{
	AudioAsset(U"game").play();
	getData().scoreRomi = 0;
	for (auto p : step(static_cast<int32>(State::NUM)))
	{
		State st = static_cast<State>(p);
		StateMachine.push(st);
	}
	{
		Task tempTask{ U"",0 };
		cookingTask.push(tempTask);
		tempTask = Task{ U"食材をメッタ切りにする",1 };
		cookingTask.push(tempTask);
		tempTask = Task{ U"ソースをつくる",2 };
		cookingTask.push(tempTask);
		tempTask = Task{ U"袋に詰める",3 };
		cookingTask.push(tempTask);
		tempTask = Task{ U"連打で揉み込む",4 };
		cookingTask.push(tempTask);
		tempTask = Task{ U"連打で揉み込む",5 };
		cookingTask.push(tempTask);
		tempTask = Task{ U"これはエラーです\nnoexpect",6 };
		cookingTask.push(tempTask);
	}
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
	//ここから気合ゾーン_当たり判定設定
	//cucumb768x192
	Rect tempRefColliderCucum = Rect{ Arg::center(Scene::Center().x - TILECHIP * 4 + QUARTER_TILECHIP,Scene::Center().y) ,192 / 4,192 };
	int32 tempIntervalCucum = tempRefColliderCucum.pos.x;
	for (auto p : step(16))
	{
		//キュウリは16回切る
		tempRefColliderCucum.pos.x = p * HALF_TILECHIP + tempIntervalCucum;
		cutFoods << tempRefColliderCucum;
	}
	Rect tempRefColliderSalmon = Rect{ Arg::center(Scene::Center().x - TILECHIP * 4 + QUARTER_TILECHIP,Scene::Center().y) ,192 / 4,288 };
	int32 tempIntervalSalmon = tempRefColliderSalmon.pos.x;
	for (auto p : step(16))
	{
		//サーモンは16回切る
		tempRefColliderSalmon.pos.x = p * HALF_TILECHIP + tempIntervalSalmon;
		cutFoods << tempRefColliderSalmon;
	}
	//tomato384x384 and onion384x384
	Rect tempRefColliderRectangle = Rect{ Arg::center(Scene::Center().x - TILECHIP * 2 + QUARTER_TILECHIP,Scene::Center().y) ,192 / 4,384 };
	int32 tempIntervalRectangle = tempRefColliderRectangle.pos.x;
	Rect tempRefColliderRectangleY = Rect{ Arg::center(Scene::Center().x,Scene::Center().y - TILECHIP * 2 + HALF_TILECHIP) ,384,192 / 4 };
	int32 tempIntervalRectangleY = tempRefColliderRectangleY.pos.y;
	for (auto p : step(8))
	{
		//トマトは縦と横で8回ずつ切る
		tempRefColliderRectangle.pos.x = p * HALF_TILECHIP + tempIntervalRectangle;
		cutFoods << tempRefColliderRectangle;
	}
	for (auto p : step(8))
	{
		tempRefColliderRectangleY.pos.y = p * HALF_TILECHIP + tempIntervalRectangleY;
		cutFoods << tempRefColliderRectangleY;
	}
	for (auto p : step(8))
	{
		//トマトは縦と横で8回ずつ切る//玉ねぎも同様
		tempRefColliderRectangle.pos.x = p * HALF_TILECHIP + tempIntervalRectangle;
		cutFoods << tempRefColliderRectangle;
	}
	for (auto p : step(8))
	{
		tempRefColliderRectangleY.pos.y = p * HALF_TILECHIP + tempIntervalRectangleY;
		cutFoods << tempRefColliderRectangleY;
	}
	//気合ゾーンここまで
	for (auto p : step(static_cast<int32>(State::NUM)))
	{
		State st = static_cast<State>(p);
		StateMachine.push(st);
	}
}

void Game2::FlowComments()
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

void Game2::FlowCommentsDraw()const
{
	for (const auto& it : neutralComments)
	{
		if (it.textPos.has_value())
			font(it.text).draw(it.textPos.value().x, it.textPos.value().y);
	}
	for (const auto& it : happyComments)
	{
		if (it.textPos.has_value())
			font(it.text).draw(it.textPos.value().x, it.textPos.value().y, Palette::Yellow);
	}
	for (const auto& it : angryComments)
	{
		if (it.textPos.has_value())
			font(it.text).draw(it.textPos.value().x, it.textPos.value().y, Palette::Red);
	}
}

void Game2::update()
{

	ClearPrint();
	FlowComments();
	//Print << static_cast<int>(StateMachine.front());
	switch (StateMachine.front())
	{
	case State::intro:
		intro();
		break;
	case State::cutting:
		cutting();
		break;
	case State::cutToEntryInterval:
	{

		auto func = [&]() {stopWatchEase.start(); };
		static bool callOnce = [&]() { func(); return true; }();
		trans = Min(stopWatchEase.sF(), 1.0);
		ease = EaseInOutBack(trans);
		EasePos = fromWood.lerp(toWood, ease);

		easeBag = EaseInOutBack(trans);
		easePosBag = fromBag.lerp(toBag, easeBag);

		easePosliquidLemon = fromliquidLemon.lerp(toliquidLemon, ease);
		
		easePossoySource = fromsoySource.lerp(tosoySource, ease);

		easePosoliveOil = fromoliveOil.lerp(tooliveOil, ease);

		easePossalt = fromsalt.lerp(tosalt, ease);


		if (stopWatchEase.sF() > 2.0)
		{

			StateMachine.pop();
		}
	}
		break;
	case State::entryBowl:
		entryBowl();
		break;
	case State::EntryToPoke:
	{
		auto func = [&]() {stopWatchEase.restart(); };
		static bool callOnce = [&]() { func(); return true; }();
		trans = Min(stopWatchEase.sF(), 1.0);

		easeBag = EaseInOutBack(trans);
		easePosBag = fromBag.lerp(toBag, easeBag);

		easeCutCucumber = EaseInOutBack(trans);
		easePosCutCucumber = fromCutCucumber.lerp(toCutCucumber, easeBag);

		easeslicedOnion = EaseInOutBack(trans);
		easePosslicedOnion = fromslicedOnion.lerp(toslicedOnion, easeBag);

		easeslicedSalmon = EaseInOutBack(trans);
		easePosslicedSalmon = fromslicedSalmon.lerp(toslicedSalmon, easeBag);

		easecutTomato = EaseInOutBack(trans);
		easePoscutTomato = fromcutTomato.lerp(tocutTomato, easeBag);

		if (stopWatchEase.sF() > 2.0)
		{
			StateMachine.pop();
		}
	}
		break;
	case State::intoBag:
		intoBag();
		break;
	case State::poke:
		poke();
		break;
	default:
		break;
	}

}

void Game2::draw() const
{
	

	switch (StateMachine.front())
	{
	case State::intro:
		introDraw();
		break;
	case State::cutting:
		cuttingDraw();
		break;
	case State::cutToEntryInterval:
	{
		TextureAsset(U"backWood").drawAt(Scene::Center());
		TextureAsset(U"woodBoard").drawAt(EasePos);

		TextureAsset(U"liquidLemon").drawAt(easePosliquidLemon);
		TextureAsset(U"soySource").drawAt(easePossoySource);
		TextureAsset(U"oliveOil").drawAt(easePosoliveOil);
		TextureAsset(U"salt").drawAt(easePossalt);
		TextureAsset(U"dish").scaled(3,3).drawAt(easePosBag);

	}
		break;
	case State::entryBowl:
		entryBowlDraw();
		break;
	case State::EntryToPoke:
	{
		TextureAsset(U"backWood").drawAt(Scene::Center());
		TextureAsset(U"emptyBag").drawAt(easePosBag);
		TextureAsset(U"cutCucumber").drawAt(easePosCutCucumber);
		TextureAsset(U"cutTomato").drawAt(easePoscutTomato);
		TextureAsset(U"slicedSalmon").drawAt(easePosslicedSalmon);
		TextureAsset(U"slicedOnion").drawAt(easePosslicedOnion);

	}
		break;
	case State::intoBag:
		intoBagDraw();
		break;
	case State::poke:
		pokeDraw();
		break;
	default:
		break;
	}

	if (cookingTask.front().Progress not_eq 0)
		Rect(20, 20, 400, 300).draw(HSV(142, 0.42, 0.28, 0.6));//taskViewRect//黒板色


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
	
	FlowCommentsDraw();

	
}

void Game2::intro()
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

void Game2::introDraw()const
{
	double t = transition.value();

	TextureAsset(U"romiromiSalmon").draw();
	font(U"きょうの料理").drawAt(Scene::Center().x, Scene::Center().y - TILECHIP * 5);
	font(U"ロミロミサーモン").drawAt(Scene::Center().x, Scene::Center().y - TILECHIP * 3 - TILECHIP / 2);
	Circle{ Scene::Center(), (t * 1200) }.draw(Palette::Black);

}
 
void Game2::cutting()
{

	

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
		++cutCount;
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

void Game2::cuttingDraw()const
{
	TextureAsset(U"backWood").drawAt(Scene::Center());
	TextureAsset(U"woodBoard").drawAt(Scene::Center());
	if (cutCount <= 16) {
		TextureAsset(U"cucumber").drawAt(Scene::Center());
	}
	else if (cutCount <= 32) {
		TextureAsset(U"rawSalmon").drawAt(Scene::Center());
	}
	else if (cutCount <= 48) {
		TextureAsset(U"tomato").drawAt(Scene::Center());
	}
	else if (cutCount <= 64) {
		TextureAsset(U"onion").drawAt(Scene::Center());
	}
	//TextureAsset(U"garlic").drawAt(garlic.collision.center());
	
	nowCutting.draw(ColorF(Palette::Deepskyblue, 0.5));
	if (recievePoint)
	{
		for (auto& a : *recievePoint)
		{
			Circle(a, 4).draw(Palette::Red);
		}
	}
	cutLine.draw(Palette::Orange);
}

void Game2::intoBag()
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
		if (map.second.collision.intersects(bag.collision) and not map.second.isGrab)
		{
			map.second.isinto = true;
			map.second.collision.setPos(-1000, -1000);
			AudioAsset(U"powder").playOneShot();
		}
	}
	isAllin = std::ranges::all_of(foodManager, [](std::pair<String, Foods> food) {return food.second.isinto; });
	isAnyin = std::ranges::any_of(foodManager, [](std::pair<String, Foods> food) {return food.second.isinto; });
	if (isAllin)
	{
		StateMachine.pop();
		cookingTask.pop();
		return;
	}
}

void Game2::intoBagDraw() const
{
	TextureAsset(U"backWood").drawAt(Scene::Center());
	if (isAnyin)
	{
		TextureAsset(U"allInBag").drawAt(Scene::Center());
	}
	else
	{
		TextureAsset(U"emptyBag").drawAt(Scene::Center());
	}
	if (not foodManager.at(U"cutCucumber").isinto) {
		TextureAsset(U"cutCucumber").drawAt(foodManager.at(U"cutCucumber").collision.center());
	}
	if (not foodManager.at(U"cutTomato").isinto) {
		TextureAsset(U"cutTomato").drawAt(foodManager.at(U"cutTomato").collision.center());
	}
	if (not foodManager.at(U"slicedOnion").isinto) {
		TextureAsset(U"slicedOnion").drawAt(foodManager.at(U"slicedOnion").collision.center());
	}
	if (not foodManager.at(U"slicedSalmon").isinto) {
		TextureAsset(U"slicedSalmon").drawAt(foodManager.at(U"slicedSalmon").collision.center());
	}

}

void Game2::poke()
{
	auto func = [&]() {pokeWatch.start(); };
	static bool callOnce = [&]() { func(); return true; }();
	
	
	
	
	linework
		.setThickness(minThickness, maxThickness)
		.setLineCount(static_cast<size_t>(lineCount))
		.setOffsetRange(offsetRange);
		target.setCenter(Cursor::Pos());
		linework.setTargetShape(target);

	if (MouseL.down())
	{
	}

	


	if (BagCollider.leftClicked())
	{
		++PokeRating;
		AudioAsset(U"hit").playOneShot();
	}



	if (BagCollider.leftClicked() or BagCollider.leftPressed())
	{
		
		isPoking = true;
	}
	else
	{
		isPoking = false;
	}
	if (MouseL.up())
	{
		isPoking = false;
	}
	if (pokeWatch.s() > 20)
	{
		if (cutRating.sum() < 2000)
			getData().scoreRomi++;
		if (PokeRating > 40)
			getData().scoreRomi++;
		if (PokeRating > 60)
			getData().scoreRomi++;
		AudioAsset(U"game").stop();
		changeScene(GameState::Result2);
	}
}

void Game2::pokeDraw() const
{
	TextureAsset(U"backWood").drawAt(Scene::Center());
	if (isPoking)
	{
		linework.setTargetShape(target);
		linework.draw(Palette::Grey);
		TextureAsset(U"pokedBag").drawAt(Scene::Center());
	}
	else
	{
		//linework.draw(Palette::Grey);
		TextureAsset(U"allInBag").drawAt(Scene::Center());
	}

}

void Game2::entryBowl()
{
	for (auto& map : foodManager2)//個々に対しての処理
	{
		if (map.second.collision.leftClicked())
		{
			map.second.isGrab = true;
		}
	}
	if (MouseL.up())//全体に対しての処理
	{
		for (auto& it : foodManager2)
		{
			it.second.isGrab = false;
		}
	}
	for (auto& map : foodManager2)//個々に対して
	{
		if (map.second.isGrab)
		{
			map.second.collision.moveBy(Cursor::Delta());
		}
		if (map.second.collision.intersects(bag.collision) and not map.second.isGrab)
		{
			map.second.isinto = true;
			map.second.collision.setPos(-1000, -1000);
			AudioAsset(U"powder").playOneShot();
		}
	}
	isAllin2 = std::ranges::all_of(foodManager2, [](std::pair<String, Foods> food) {return food.second.isinto; });
	isAnyin2 = std::ranges::any_of(foodManager2, [](std::pair<String, Foods> food) {return food.second.isinto; });
	if (isAllin2)
	{
		StateMachine.pop();
		cookingTask.pop();
		
		return;
	}
}

void Game2::entryBowlDraw() const
{
	TextureAsset(U"backWood").drawAt(Scene::Center());
	if (isAnyin2)
	{
		TextureAsset(U"allInDish").drawAt(Scene::Center());
	}
	else
	{
		TextureAsset(U"dish").scaled(3,3).drawAt(Scene::Center());
	}
	if (not foodManager2.at(U"liquidLemon").isinto) {
		TextureAsset(U"liquidLemon").drawAt(foodManager2.at(U"liquidLemon").collision.center());
	}
	if (not foodManager2.at(U"soySource").isinto) {
		TextureAsset(U"soySource").drawAt(foodManager2.at(U"soySource").collision.center());
	}
	if (not foodManager2.at(U"oliveOil").isinto) {
		TextureAsset(U"oliveOil").drawAt(foodManager2.at(U"oliveOil").collision.center());
	}
	if (not foodManager2.at(U"salt").isinto) {
		TextureAsset(U"salt").drawAt(foodManager2.at(U"salt").collision.center());
	}
}
