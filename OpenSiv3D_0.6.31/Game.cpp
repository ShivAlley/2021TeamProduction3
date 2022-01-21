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
		cutLine2 = cutLine.movedBy(10, 20);
	}
	Print << cutFoods.isEmpty();
	for (auto& pr : foodvertices)
		Print << pr.xy();
	if (recievePoint)
		Print << recievePoint->size();
	if (cutFoods.any([&](Polygon poly) {return poly.intersects(cutLine); }) and cutLine.hasLength()
		and cutFoods.any([&](Polygon poly) {return poly.intersects(cutLine2); }) and cutLine2.hasLength())
	{
		for (auto& ref : cutFoods)
		{

			recievePoint = ref.intersectsAt(cutLine);
			recievePoint2 = ref.intersectsAt(cutLine2);
			Array<Vec2> tempArr;
			
			for (auto& vert : ref.vertices())
			{
				//Float2を受け取りVec2にキャストしてからそれを寿命の短い配列に渡しカプセル内のメンバー変数配列に代入する
				Float2 temp;
				temp = vert.xy();
				static_cast<Vec2>(temp);
				Print << temp;
				tempArr << temp;

			}
				//tempArr << cutLine.center().xy();
				foodvertices = tempArr;
				tempArr.clear();
		}
		if ((recievePoint->size() > 1) and (recievePoint2->size() > 1))//要素数が2個以上の時//限界クソコード
		{
			foodvertices.append(recievePoint.value());
			foodvertices.append(recievePoint2.value());
			
			cutFoods = Polygon::Correct(foodvertices, {}/*not Holes*/);

			//cutFoods.each([](Polygon poly) {poly.scale(0.9, 0.9); });
			
		}
		//recievePoint->append(cutFoods.each([=](Polygon food) {return food.intersects(cutLine); }));
	}
	else
	{
		recievePoint.reset();
		recievePoint2.reset();
	}
	//recievePoint = onion.collision.intersectsAt(cutLine);
	//recievePoint = oniRect.intersectsAt(cutLine);
}


Game::Game(const InitData& init)
	:IScene(init)
{
	{ 
		TextReader reader{ U"comments/normalComments.txt" };
		Comments temptxt;
		while (reader.readLine(temptxt.text))
		{

			flowtingComments << temptxt;
		}
	}

	
	textPos     = { Scene::Width(),600 };
	textPosLong = { Scene::Width(),700 };
	cutSample.setPos(Scene::Center().x + TILECHIP, Scene::Center().y + TILECHIP);
	cutFoods << onion.collision.asPolygon();
	testpoly = onion.collision.asPolygon();
	//comments.choice().textPos = { Scene::Width(), Random<int32>(0, Scene::Height() - 90) };
	
}


void Game::update()
{
	ClearPrint();
	for (auto p : flowtingComments)
	{
		Print << p.font.isEmpty();
		Print << p.text;
		Print << p.textPos;
	}
	for (auto& p : flowtingComments)
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
		if (not flowtingComments.isEmpty())
		{
			//ラムダ式の参照渡しでコメントコンテナの要素を変更してその返り値で
			//無限ループにならないようにしたWhile文を回し、かならず1秒に一回コメントが流れるようにしている
			std::function<bool()> PostComment = [&]() -> bool
			{
				if (auto& post = flowtingComments.choice(); post.isFlow == false)
				{
					post.isFlow = true;
					post.textPos = { Scene::Width(), Random<int32>(0, Scene::Height() - 90) };
					return false;
				}
				return true;
			};

			while (PostComment())
			{
				if (not flowtingComments.any([](Comments comm) {return comm.isFlow == false; }))
				{
					break;
				}
			}
		}
	
	}
	for (auto& ref : flowtingComments)
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
	Rect(Arg::topCenter(Scene::Center().x,TILECHIP * 2), TILECHIP*10,TILECHIP * 7).draw();
	Rect(Arg::topLeft(0,0), TILECHIP*2).draw();
	Rect(Arg::topRight(Scene::Width(),0), TILECHIP*2).draw();
	Rect(Arg::bottomLeft(0,Scene::Height()), TILECHIP*2).draw();
	Rect(Arg::bottomRight(Scene::Width(),Scene::Height()), TILECHIP*2).draw();
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
	if (recievePoint2)
	{
		for (auto& a : *recievePoint2)
		{
			Circle(a, 4).draw(Palette::Red);
		}
	}
	
	cutLine.draw(Palette::Orange);
	cutLine2.draw(Palette::Orange);
	for (auto& ref : flowtingComments)
	{
		if(ref.textPos.has_value())
			ref.font(ref.text).draw(ref.textPos.value().x,ref.textPos.value().y);
		
	}

	for (const auto& foodv : foodvertices)
		Circle(foodv.xy(), 4).draw(Palette::Magenta);

	for (int32 x = 0; x < 50; ++x)
	{
		Line(x * TILECHIP, 0, x * TILECHIP, Scene::Height()).draw(1,Palette::Cyan);
	}
	for (int32 y = 0; y < 50; ++y)
	{
		Line(0,y*TILECHIP,Scene::Width(),y*TILECHIP).draw(1, Palette::Cyan);
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
