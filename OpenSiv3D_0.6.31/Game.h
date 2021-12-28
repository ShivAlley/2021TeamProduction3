#pragma once
#include "common.h"
class Game :public App::Scene
{
public:
	Game(const InitData& init);
	void update()override;
	void draw() const override;
private:
#ifdef _DEBUG
	const Font font{ 50 };
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

