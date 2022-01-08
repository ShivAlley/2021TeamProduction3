#pragma once
#include <Siv3D.hpp>
enum class State
{
	Title,
	Game,
	Menu,
	Result,
};

struct ShareGameData
{
	char dummy;
};

using App = SceneManager<State,ShareGameData>;
