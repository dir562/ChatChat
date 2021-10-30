#pragma once
#include "game_system.h"
#include "Renderer.h"
#include "DynamicObj.h"

class Game
{
public:
	static Game& instance()
	{
		static Game _instance;
		return _instance;
	}

public:
	Renderer& renderer = Renderer::instance();
	GAME_SYSTEM& game_system = GAME_SYSTEM::instance();

	Player0Ptr player;

public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

private:
	Game() 
	{
		init();
	};

	void init()
	{
		player = renderer.get_player();
	}
};

