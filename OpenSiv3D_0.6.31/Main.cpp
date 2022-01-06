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
	
#ifdef _DEBUG
	Window::Resize(1920, 96*11);
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetFullscreen(true,0);
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

	manager.init(State::Game);
	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}

		


	}

}
