#pragma once
#include "common.h"
class Game :public App::Scene
{
public:

	Game() {}
	Game(const InitData& init);
	void update()override;
	void draw() const override;
protected:
	Line cutLine{};
	Optional<Vec2> cutBegin;
	Optional<Vec2> cutEnd;
	
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
	virtual void Cutting() {}
	Game* cookingProgress;
};

class CookingProgress :public Game
{
public:
	CookingProgress();
	~CookingProgress();
	void Cutting() override;
private:

};


