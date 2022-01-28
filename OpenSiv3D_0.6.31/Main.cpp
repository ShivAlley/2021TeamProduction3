# include <Siv3D.hpp>
#include "common.h"
#include "Title.h"
#include "Game.h"
#include "Game2.h"
#include "Menu.h"
#include "Result.h"
#include "Result2.h"

void Main()
{
#ifdef _DEBUG
	const Font font{ 50 };
#endif // _DEBUG
	
#ifdef _DEBUG
	//Window::Resize(1280,720);
	//Window::SetFullscreen(true,0);
	//Window::SetStyle(WindowStyle::Frameless);
	//Window::Resize(1920, 1080);
#endif // DEBUG

#ifdef NDEBUG
	Window::SetTitle(U"Funny Live Cooking!!");
	Window::SetFullscreen(true);
	Window::Resize(1920,96*11);
	System::SetTerminationTriggers(UserAction::Default);
	Scene::SetResizeMode(ResizeMode::Keep);
#endif

	App manager;
	manager.add<Title>(GameState::Title);
	manager.add<Game>(GameState::Game);
	manager.add<Menu>(GameState::Menu);
	manager.add<Result>(GameState::Result);
	manager.add<Result2>(GameState::Result2);
	manager.add<Game2>(GameState::Game2);
	//manager.init(State::Menu);
	//manager.init(State::Game2);
	//manager.init(GameState::Result);
	TextureAsset::Register(U"woodBoard",Resource(U"Image/woodBoard.png"));
	TextureAsset::Register(U"garlic",			Resource(U"Image/garlic.png"));
	TextureAsset::Register(U"animatedCookPot",	Resource(U"Image/cookPot.png"));
	TextureAsset::Register(U"rawPastaInCookPot",Resource(U"Image/RawPastaInCookPot.png"));
	TextureAsset::Register(U"pastaInCookPot",	Resource(U"Image/pastaInCookPot.png"));
	TextureAsset::Register(U"rawPasta",			Resource(U"Image/Rawpasta.png"));
	TextureAsset::Register(U"salt",				Resource(U"Image/salt.png"));
	TextureAsset::Register(U"dish",				Resource(U"Image/dish.png"));
	TextureAsset::Register(U"emptyFrypan",		Resource(U"Image/emptyFrypan.png"));
	TextureAsset::Register(U"onFoodFrypan",		Resource(U"Image/onFoodFrypan.png"));
	TextureAsset::Register(U"frypanHand",		Resource(U"Image/frypanHand.png"));
	TextureAsset::Register(U"cucumber",			Resource(U"Image/cucumber.png"));
	TextureAsset::Register(U"onion",			Resource(U"Image/onion.png"));
	TextureAsset::Register(U"rawSalmon",		Resource(U"Image/rawSalmon.png"));
	TextureAsset::Register(U"tomato",			Resource(U"Image/tomato.png"));
	TextureAsset::Register(U"allInBag",			Resource(U"Image/allInBag.png"));
	TextureAsset::Register(U"pokedBag",			Resource(U"Image/pokedBag.png"));
	TextureAsset::Register(U"peperoncino",		Resource(U"Image/peperoncino.png"));
	TextureAsset::Register(U"cutCucumber",		Resource(U"Image/cutCucumber.png"));
	TextureAsset::Register(U"cutTomato",		Resource(U"Image/cutTomato.png"));
	TextureAsset::Register(U"slicedOnion",		Resource(U"Image/slicedonion.png"));
	TextureAsset::Register(U"slicedSalmon",		Resource(U"Image/slicedSalmon.png"));
	TextureAsset::Register(U"emptyBag",			Resource(U"Image/emptyBag.png"));
	TextureAsset::Register(U"soySource",		Resource(	U"Image/soySource.png"));
	TextureAsset::Register(U"oliveOil",			Resource(U"Image/oliveOil.png"));
	TextureAsset::Register(U"liquidLemon",		Resource(U"Image/liquidLemon.png"));
	TextureAsset::Register(U"allInDish",		Resource(U"Image/allInDish.png"));
	TextureAsset::Register(U"title",			Resource(U"Image/title.png"));
	TextureAsset::Register(U"romiromiSalmon",	Resource(U"Image/romiromiSalmon.png"));
	TextureAsset::Register(U"backWood",			Resource(U"Image/BackWood.png"));
	TextureAsset::Register(U"IH",				Resource(U"Image/IH.png"));
	TextureAsset::Register(U"pepeIcon",			Resource(U"Image/pepeIcon.png"));
	TextureAsset::Register(U"romiIcon",			Resource(U"Image/romiIcon.png"));
	TextureAsset::Register(U"star",				Resource(U"Image/star.png"));
	TextureAsset::Register(U"result",			Resource(U"Image/result.png"));
	TextureAsset::Register(U"resultPepe",		Resource(U"Image/resultPepe.png"));
	TextureAsset::Register(U"resultRomi",		Resource(U"Image/resultRomi.png"));
	AudioAsset::Register(U"hit",				Resource(U"Sound/hit.wav"));
	AudioAsset::Register(U"boil",				Resource(U"Sound/boil.wav"),Loop::Yes);
	AudioAsset::Register(U"powder",				Resource(U"Sound/powder.wav"));
	AudioAsset::Register(U"cut",				Resource(U"Sound/cut.wav"));
	AudioAsset::Register(U"stir",				Resource(U"Sound/stir.wav"));
	AudioAsset::Register(U"title",				Resource(U"Sound/タイトル.wav"),Loop::Yes);
	AudioAsset::Register(U"stageSelect",		Resource(U"Sound/レシピ選択.wav"),Loop::Yes);
	AudioAsset::Register(U"game",				Resource(U"Sound/調理画面.wav"),Loop::Yes);
	AudioAsset::Register(U"result",				Resource(U"Sound/評価効果音1.wav"));
	
	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}

	


	}

}
