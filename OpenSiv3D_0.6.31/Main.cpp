# include <Siv3D.hpp>
#include "common.h"
#include "Title.h"
#include "Game.h"
#include "Menu.h"
#include "Result.h"

void Main()
{
#ifdef _DEBUG
	const Font font{ 50 };
#endif // _DEBUG
	
#ifndef _DEBUG
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);
#endif // DEBUG

	Window::SetFullscreen(true);
	App manager;
	manager.add<Title>(State::Title);
	manager.add<Game>(State::Game);
	manager.add<Menu>(State::Menu);
	manager.add<Result>(State::Result);

	manager.init(State::Game);
	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}

		


	}

}
