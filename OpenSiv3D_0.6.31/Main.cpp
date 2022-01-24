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
	//Window::SetFullscreen(true,0);
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
	TextureAsset::Register(U"onion",U"Image/onion.png");
	TextureAsset::Register(U"animatedCookPot",U"Image/cookPot.png");
	TextureAsset::Register(U"woodBoard",U"Image/woodBoard.png");
	TextureAsset::Register(U"rawPastaInCookPot",U"Image/RawPastaInCookPot.png");
	TextureAsset::Register(U"pastaInCookPot",U"Image/pastaInCookPot.png");
	TextureAsset::Register(U"rawPasta",U"Image/Rawpasta.png");
	TextureAsset::Register(U"salt",U"Image/salt.png");
	TextureAsset::Register(U"dish",U"Image/dish.png");
	TextureAsset::Register(U"emptyFrypan",U"Image/emptyFrypan.png");
	TextureAsset::Register(U"onFoodFrypan",U"Image/onFoodFrypan.png");
	TextureAsset::Register(U"frypanHand",U"Image/frypanHand.png");
	
	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}

		


	}

}
