#pragma once
constexpr int32 TILECHIP = 96;
constexpr int32 HALF_TILECHIP = 96 / 2;
constexpr int32 QUARTER_TILECHIP = 96 / 4;

enum class GameState
{
	Title,
	Game,
	Game2,
	Menu,
	Result,
	Result2,
};

struct ShareGameData
{
	char dummy;
	int32 scorePepe;
	int32 scoreRomi;
};

using App = SceneManager<GameState,ShareGameData>;
