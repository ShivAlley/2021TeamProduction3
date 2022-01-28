#pragma once
#include "common.h"
#include <variant>
class Game :public App::Scene
{
public:
	//Game() {}
	Game(const InitData& init);
	void update()override;
	void draw() const override;
private:
	enum class State : int32
	{
		intro,
		cutting,
		cutToEntryInterval,
		entryCookingPot,
		entryToStirInterval,
		stir,
		BUFFER,//もし最後のステートでpopしてしまった場合に例外を送出しないためのバッファ
		NUM,//Stateベースで範囲を回すときの回数
	};
	//std::unordered_map<State, std::function<void(void)>> switchFunction;
	//std::unordered_map<State, std::function<void*(void*)>> switchFunction2;
	//std::unordered_map<State, std::function<void(void)>> switchFunction;
	//std::unordered_map<State, std::function<void(void)>> switchFunction2;
	
	std::queue<State> StateMachine;
	void intro();
	void Cutting();
	void FlowComments();
	void entryCookingPot();
	void stir();
	void introDraw()const;
	void CuttingDraw() const;
	void FlowCommentsDraw() const;
	void entryCookingPotDraw() const;
	void stirDraw()const;
	//Rect cutSample{ Arg::center(Scene::Center()),TILECHIP / 2 };
	Transition transition{ 2.0s, 0.5s };
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
	/// @brief 値が小さいほど高評価
	Array<int32> burnRating;
	//std::pair<RectF, double> intoFood;
	//Array<std::pair<RectF, double>> intoFoods;
	struct inFood
	{
		RectF collider;
		double brightness;
	};
	Array<inFood> intoFoods;
	Rect oniRect{ Arg::center = Scene::Center(), 192, 288 };
	Circle frypanCollider{ Scene::Center().x, Scene::Center().y + TILECHIP / 2, TILECHIP * 4 };
	Circle cookPotCollider{ Scene::Center().x, Scene::Center().y + TILECHIP / 2, TILECHIP * 3 };
	Polygon rotatePotCollider =
		cookPotCollider.arcAsPolygon(0_deg, 360_deg, 10, 0);
	double ang = 0;
	/// @brief 得点が高いほど高評価
	double rotateRating = 0;
	Texture texgarlic{ U"Image/garlic.png" };

	int32 patterns[4] = { 0, 1, 2, 3 };
	bool isAllin = false;

	struct Foods
	{
		Rect collision;
		bool isinto = false;
		bool isGrab = false;
	};
	Foods garlic =
	{
		{ Arg::center = Scene::Center(), 192, 288 },
	};
	Foods pasta =
	{
		{Arg::center(Scene::Center().x + TILECHIP * 7,Scene::Center().y + TILECHIP / 2), TILECHIP * 2, TILECHIP * 8 },
	};
	Foods salt =
	{
		{Arg::center(Scene::Center().x - TILECHIP * 6,Scene::Center().y - TILECHIP / 2 + TILECHIP * 3), TILECHIP * 2 },
	};

	HashTable<String, Foods> foodManager =
	{
		{U"pasta",pasta},
		{U"salt",salt},
	};
	const Font font{ 50, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"UDDigiKyokashoN-R.ttc" };
	struct Task
	{
		Task(String s,int32 p)
			:cookingProcess(s)
			,Progress(p)
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
		//const Font font{ 50, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"UDDigiKyokashoN-R.ttc" };
		String text;
		Optional<Vec2> textPos = none;
		bool isFlow = false;
	};
	Array<Comments> neutralComments;
	Array<Comments> angryComments;
	Array<Comments> happyComments;

	int32 Score = 0;
	
	//Array<Comments> neutralComments;
	Stopwatch commentsWatch{ StartImmediately::Yes };
	Stopwatch commentsWatchangry{ StartImmediately::Yes };
	Stopwatch commentsWatchhappy{ StartImmediately::Yes };

	Stopwatch stopWatch{ StartImmediately::Yes };
	Stopwatch stopWatchRotate{ StartImmediately::Yes };
	//イージング//今後はちゃんと構造体に入れよう！
	Stopwatch stopWatchEase{ StartImmediately::No };
	Vec2 fromWood{ Scene::Center() };
	Vec2 toWood{ Scene::Center() - Point(Scene::Width(),0) };
	double trans;
	double ease;
	Vec2 EasePos;
	//鍋
	Vec2 fromCookPot{ Scene::Center() + Point(Scene::Width(),0) };
	Vec2 toCookPot{ Scene::Center() };
	double easeCookPot;
	Vec2 EasePosCookPot;
	//塩
	Vec2 fromSalt{ Scene::Center() + Point(Scene::Width(),0) };
	Vec2 toSalt{ salt.collision.center() };
	double easeSalt;
	Vec2 EasePosSalt;
	//パスタ
	Vec2 fromPasta{ Scene::Center() + Point(Scene::Width(),0) };
	Vec2 toPasta{ pasta.collision.center() };
	double easePasta;
	Vec2 EasePosPasta;
	//フェードアウトする皿
	Vec2 fromDish{ Scene::Center().x - TILECHIP * 6,Scene::Center().y - TILECHIP / 2 + TILECHIP * 3 };
	Vec2 toDish{ Scene::Center().x - TILECHIP * 6 - Scene::Width(),Scene::Center().y - TILECHIP / 2 + TILECHIP * 3 };
	double easeDish;
	Vec2 EasePosDish;
	//フェードアウトする鍋
	Vec2 fromFadeoutCookPot{ Scene::Center() };
	Vec2 toFadeoutCookPot{ Scene::Center().x - Scene::Width(),Scene::Center().y};
	double easeFadeoutCookPot;
	Vec2 EaseFadeoutPosCookPot;

	Vec2 fromFrypan{ Scene::Center() + Point(Scene::Width(),0) };
	Vec2 toFrypan{ Scene::Center() };
	double easeFrypan;
	Vec2 EasePosFrypan;


	


};



