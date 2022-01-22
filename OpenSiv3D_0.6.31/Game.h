#pragma once
#include "common.h"
class Game :public App::Scene
{
public:
	Game(const InitData& init);
	void update()override;
	void draw() const override;
private:
	void Cutting();
	Rect cutSample{ Arg::center(Scene::Center()),TILECHIP };
	Line cutLine{};
	Optional<Vec2> cutBegin;
	Optional<Vec2> cutEnd;
	Optional<Array<Vec2>> recievePoint;
	Circle spoon{ TILECHIP / 2,TILECHIP / 2,TILECHIP };
	Array<Rect> cutFoods;
	bool cutNow = false;
	Rect nowCutting{};
	/// @brief 値が小さいほど高評価（真っ直ぐ切れている）
	Array<int32> cutRating;
	Array<Rect> intoFoods;
	Rect oniRect{ Arg::center = Scene::Center(), 192, 288 };
	Circle frypanCollider{ Scene::Center().x, Scene::Center().y + TILECHIP / 2, TILECHIP * 4 };
	Circle cookPotCollider{ Scene::Center().x, Scene::Center().y + TILECHIP / 2, TILECHIP * 3 };
	Polygon rotatePotCollider =
		cookPotCollider.arcAsPolygon(0_deg, 360_deg, 10, 0);
	double ang = 0;
	/// @brief 得点が高いほど高評価
	double rotateRating = 0;
	Texture texOnion{ U"Image/onion.png" };

	int32 patterns[4] = { 0, 1, 2, 3 };

	struct Foods
	{
		Rect collision;
		bool isinto = false;
		bool isGrab = false;
	};
	Foods onion =
	{
		{ Arg::center = Scene::Center(), 192, 288 },
	};
	Foods pasta =
	{
		{Arg::center(Scene::Center().x + TILECHIP * 7,Scene::Center().y + TILECHIP / 2), TILECHIP * 2, TILECHIP * 8 },
	};
	Foods salt =
	{
		{Arg::center(Scene::Center().x - TILECHIP * 6,Scene::Center().y + TILECHIP / 2 - TILECHIP * 3), TILECHIP * 2 },
	};

	HashTable<String, Foods> foodManager =
	{
		{U"pasta",pasta},
		{U"salt",salt},
	};
	Stopwatch stopWatch{ StartImmediately::Yes };
	Stopwatch commentsWatch{ StartImmediately::Yes };
	struct Task
	{
		String cookingProcess;
		int32 Progress;
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
	Array<Comments> neutralComments;

	

	
	

#ifdef _DEBUG
	Polygon testpoly{};
	Texture texCookingPot{ U"Image/768x768.png" };
	Texture testCookPot{ U"Image/testPot.png" };
	TextureRegion test = texCookingPot(250, 100, 200, 150);
	const Font font{ 50 };
	String text = U"０１２３４\n５６７８９";
	String flowTextGrass = U"草";
	String flowTextLong = U"０１２３４５６７８９";
	Vec2 textVelocity = {};
	Vec2 textPos = {};
	Vec2 textPosLong = {};
	Rect onion2{ Arg::center = Scene::Center() - Point(300,0), 192, 288 };
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
