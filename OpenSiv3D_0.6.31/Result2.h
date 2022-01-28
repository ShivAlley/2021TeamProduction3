#pragma once
#include "common.h"
class Result2 : public App::Scene
{
public:
	Result2(const InitData& init);
	void update()override;
	void draw()const override;
private:
	Stopwatch stopwatch{ StartImmediately::Yes };
};

