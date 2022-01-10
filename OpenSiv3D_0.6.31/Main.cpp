# include <Siv3D.hpp>
#include "common.h"
#include "Title.h"
#include "Game.h"
#include "Game2.h"
#include "Menu.h"
#include "Result.h"

void Main()
{
#ifdef _DEBUG
	const Font font{ 50 };
#endif // _DEBUG
	
#ifdef _DEBUG
	Window::Resize(1920,96*11);
	//Window::Resize(1280,720);
	Scene::SetResizeMode(ResizeMode::Keep);
	//Window::SetFullscreen(true);
	//Window::SetStyle(WindowStyle::Frameless);
	//Window::Resize(1920, 1080);
#endif // DEBUG

#ifdef NDEBUG
	Window::SetFullscreen(true);
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);
#endif

	App manager;
	manager.add<Title>(State::Title);
	manager.add<Game>(State::Game);
	manager.add<Menu>(State::Menu);
	manager.add<Result>(State::Result);
	manager.add<Game2>(State::Game2);
	manager.init(State::Game);
	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}

		


	}

}
