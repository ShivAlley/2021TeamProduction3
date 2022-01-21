#pragma once
#include "common.h"
class Game :public App::Scene
{
public:
	Game(const InitData& init);
	void update()override;
	void draw() const override;
	void Cutting();
private:
	Line cutLine{};
	Line cutLine2{};
	Optional<Vec2> cutBegin;
	Optional<Vec2> cutEnd;
	Optional<Array<Vec2>> recievePoint;
	Optional<Array<Vec2>> recievePoint2;
	Circle spoon{ TILECHIP / 2,TILECHIP / 2,TILECHIP };

	Rect cutSample{ Arg::center(TILECHIP/2,TILECHIP/2),TILECHIP };
	Array<Rect> intoFoods;
	Array<Polygon> cutFoods;
	Array<Vec2> foodvertices;

	Rect oniRect{ Arg::center = Scene::Center(), 192, 288 };
	Circle cookingPot{ Scene::Center().x, Scene::Center().y + TILECHIP / 2, TILECHIP * 4 };
	Texture texOnion{ U"Image/onion.png" };
	
	struct Foods
	{
		Rect collision;
		Polygon collider;
		bool isinto;
	};
	Foods onion =
	{
		{ Arg::center = Scene::Center(), 192, 288 },
		{},
		false,
	};
	Foods pasta =
	{
		{Arg::center(Scene::Center().x + TILECHIP * 6,Scene::Center().y + TILECHIP / 2), TILECHIP * 2, TILECHIP * 8 },
		{},
		false,
	};
	Foods salt =
	{
		{Arg::center(Scene::Center().x - TILECHIP * 6,Scene::Center().y + TILECHIP / 2 - TILECHIP * 3), TILECHIP * 2 },
		{},
		false,
	};

	HashTable<String, bool> isgrab =
	{
		{U"pasta",false},
		{U"salt",false},
	};
	Stopwatch stopWatch{ StartImmediately::Yes };
	Stopwatch commentsWatch{ StartImmediately::Yes };
	struct Task
	{
		String cookingProcess;
		int32 priority;
	};
	Array<Task> cookingTask;
	struct Comments
	{
		const Font font{ 50 };
		String text;
		Optional<Vec2> textPos = none;
		bool isFlow = false;
	};
	Array<Comments> flowtingComments;
	Array<Comments> stuckingComments;

#ifdef _DEBUG
	Polygon testpoly{};
	Texture texCookingPot{ U"Image/768x768.png" };
	Texture testCookPot{ U"Image/testPot.png" };
	TextureRegion test = texCookingPot(250, 100, 200, 150);
	const Font font{ 50 };
	String text                  = U"０１２３４\n５６７８９";
	String flowTextGrass         = U"草";
	String flowTextLong          = U"０１２３４５６７８９";
	Vec2 textVelocity            = {};
	Vec2 textPos                 = {};
	Vec2 textPosLong             = {};
	Rect onion2{ Arg::center     = Scene::Center() - Point(300,0), 192, 288 };
	Image image{ U"Image/testPot.png" };
#endif // _DEBUG
};

//class CookingProgress :public Game
//{
//public:
//	CookingProgress();
//	~CookingProgress();
//	void Cutting() override;
//private:
//
//};


