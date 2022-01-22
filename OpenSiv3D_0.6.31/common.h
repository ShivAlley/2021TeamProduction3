#pragma once
#include <Siv3D.hpp>
constexpr int32 TILECHIP = 96;
constexpr int32 HALF_TILECHIP = 96 / 2;
constexpr int32 QUARTER_TILECHIP = 96 / 4;

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
