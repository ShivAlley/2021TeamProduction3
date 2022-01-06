#pragma once
#include "common.h"
class Game :public App::Scene
{
public:
	Game(const InitData& init);
	void update()override;
	void draw() const override;
private:
	Line cutLine{};
	Vec2 cutBegin = { NONE,NONE };
	Vec2 cutEnd = { NONE,NONE };
	Rect onion{ Arg::center = Scene::Center(), 192, 288 };
	Optional<Array<Vec2>> recievePoint;
#ifdef _DEBUG
	const Font font{ 50 };
	Rect onion2{ Arg::center = Scene::Center() - Point(300,0), 192, 288 };
#endif // _DEBUG
	Stopwatch stopWatch{ StartImmediately::Yes };
	struct Task
	{
		String cookingProcess;
		int32 priority;
		bool isDone;
	};
	Array<Task> cookingTask;

};

