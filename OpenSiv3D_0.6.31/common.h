#pragma once
#include <Siv3D.hpp>
#define TILECHIP 96
enum class State
{
	Title,
	Game,
	Game2,
	Menu,
	Result,
};

struct ShareGameData
{
	char dummy;
};

using App = SceneManager<State,ShareGameData>;
