#pragma once
#include "common.h"
class Game2 : public App::Scene
{
public:
	Game2(const InitData& init);
	void update()override;
	void draw() const override;
private:
	void intro();
	void introDraw()const;
	void cutting();
	void cuttingDraw()const;
	void intoBag();
	void intoBagDraw()const;
	void poke();
	void pokeDraw()const;
	void entryBowl();
	void entryBowlDraw()const;

	void FlowComments();
	void FlowCommentsDraw()const;
	enum class State : int32
	{
		intro,
		cutting,
		cutToEntryInterval,
		entryBowl,
		EntryToPoke,
		intoBag,
		poke,
		BUFFER,//もし最後のステートでpopしてしまった場合に例外を送出しないためのバッファ
		NUM,//Stateベースで範囲を回すときの回数
	};
	const Font font{ 50, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"UDDigiKyokashoN-R.ttc" };

	std::queue<State> StateMachine;
	Line cutLine{};
	Optional<Vec2> cutBegin;
	Optional<Vec2> cutEnd;
	Optional<Array<Vec2>> recievePoint;
	Array<Rect> cutFoods;
	bool cutNow = false;
	Rect nowCutting{};
	/// @brief 値が小さいほど高評価（真っ直ぐ切れている）
	Array<int32> cutRating;
	int32 cutCount = 0;
	bool isAllin = false;
	bool isAnyin = false;
	bool isAllin2 = false;
	bool isAnyin2 = false;

	struct Foods
	{
		Rect collision;
		bool isinto = false;
		bool isGrab = false;
	};

	Foods bag =
	{
		{ Arg::center = Scene::Center(), 576, 672 },
	};
	Foods cutCucumber =
	{
		{Arg::center(Scene::Center().x + TILECHIP * 7,Scene::Center().y + TILECHIP * 4), TILECHIP * 2 },
	};
	Foods salt =
	{
		{Arg::center(Scene::Center().x - TILECHIP * 6,Scene::Center().y - TILECHIP / 2 + TILECHIP * 3), TILECHIP * 2 },
	};
	Foods slicedOnion =
	{

		{Arg::center(Scene::Center().x - TILECHIP * 6,Scene::Center().y - TILECHIP / 2 + TILECHIP * 3), 384 },
	};
	Foods slicedSalmon =
	{
		{Arg::center(Scene::Center().x - TILECHIP * 6,Scene::Center().y - TILECHIP / 2 - TILECHIP * 3), 384 },
	};
	Foods cutTomato =
	{
		{Arg::center(Scene::Center().x + TILECHIP * 6,Scene::Center().y - TILECHIP / 2 - TILECHIP * 2), 384 },
	};
	Foods liquidLemon = cutCucumber;
	Foods soySource = slicedSalmon;
	Foods oliveOil = cutTomato;

	HashTable<String, Foods> foodManager =
	{
		{U"cutCucumber",cutCucumber},
		{U"slicedOnion",slicedOnion},
		{U"slicedSalmon",slicedSalmon},
		{U"cutTomato",cutTomato},

	};

	HashTable<String, Foods> foodManager2 =
	{
		{U"liquidLemon",liquidLemon},
		{U"soySource",soySource},
		{U"oliveOil",oliveOil},
		{U"salt",salt},
	};


	Rect BagCollider{ Arg::center(Scene::Center()),576,672 };
	Rect BowlCollider{ Arg::center(Scene::Center()),384,384 };
	bool isPoking = false;
	int32 PokeRating = 0;
	Ellipse target{ Scene::CenterF().x,Scene::CenterF().y, 180 * 2, 120 * 2 };
	Rect outer = Scene::Rect();
	double minThickness = 30.0, maxThickness = 100.0;
	double lineCount = 100;
	double offsetRange = 60.0;

	mutable SaturatedLinework<Ellipse> linework{ target, outer };
	
	
	struct Task
	{
		Task(String s, int32 p)
			:cookingProcess(s)
			, Progress(p)
		{}
		Task(const Task& obj)
		{
			cookingProcess = obj.cookingProcess;
			Progress = obj.Progress;
			taskView = obj.taskView;
		};
		String cookingProcess;
		int32 Progress;
		Rect taskView = Rect{ 20, 20, 400, 300 };
	};
	std::queue<Task> cookingTask;
	struct Comments
	{
		//const Font font{ 50 };
		String text;
		Optional<Vec2> textPos = none;
		bool isFlow = false;
	};
	Array<Comments> neutralComments;
	Array<Comments> angryComments;
	Array<Comments> happyComments;
	Stopwatch commentsWatch{ StartImmediately::Yes };
	Stopwatch commentsWatchangry{ StartImmediately::Yes };
	Stopwatch commentsWatchhappy{ StartImmediately::Yes };

	Stopwatch stopWatch{ StartImmediately::Yes };
	Stopwatch pokeWatch{ StartImmediately::No };
	Transition transition{ 2.0s, 0.5s };

	Stopwatch stopWatchEase{ StartImmediately::No };
	Vec2 fromWood{ Scene::Center() };
	Vec2 toWood{ Scene::Center() - Point(Scene::Width(),0) };
	double trans;
	double ease;
	Vec2 EasePos;

	Vec2 fromliquidLemon{ liquidLemon.collision.center() + Point(Scene::Width(),0) };
	Vec2 toliquidLemon{ liquidLemon.collision.center() };
	double easeliquidLemon;
	Vec2 easePosliquidLemon;

	Vec2 fromsoySource{ soySource.collision.center() + Point(Scene::Width(),0) };
	Vec2 tosoySource{ soySource.collision.center() };
	double easesoySource;
	Vec2 easePossoySource;

	Vec2 fromoliveOil{ oliveOil.collision.center() + Point(Scene::Width(),0) };
	Vec2 tooliveOil{ oliveOil.collision.center() };
	double easeoliveOil;
	Vec2 easePosoliveOil;

	Vec2 fromsalt{ salt.collision.center() + Point(Scene::Width(),0) };
	Vec2 tosalt{ salt.collision.center() };
	double easesalt;
	Vec2 easePossalt;



	Vec2 fromBag{ Scene::Center() + Point(Scene::Width(),0) };
	Vec2 toBag{ Scene::Center() };
	double easeBag;
	Vec2 easePosBag;

	Vec2 fromCutCucumber{ cutCucumber.collision.center() + Point(Scene::Width(),0) };
	Vec2 toCutCucumber{ cutCucumber.collision.center() };
	double easeCutCucumber;
	Vec2 easePosCutCucumber;

	Vec2 fromslicedOnion{ slicedOnion.collision.center() + Point(Scene::Width(),0) };
	Vec2 toslicedOnion{ slicedOnion.collision.center() };
	double easeslicedOnion;
	Vec2 easePosslicedOnion;

	Vec2 fromslicedSalmon{ slicedSalmon.collision.center() + Point(Scene::Width(),0) };
	Vec2 toslicedSalmon{ slicedSalmon.collision.center() };
	double easeslicedSalmon;
	Vec2 easePosslicedSalmon;

	Vec2 fromcutTomato{ cutTomato.collision.center() + Point(Scene::Width(),0) };
	Vec2 tocutTomato{ cutTomato.collision.center() };
	double easecutTomato;
	Vec2 easePoscutTomato;

};

