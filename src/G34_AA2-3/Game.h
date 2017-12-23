#pragma once
#include "Scene.h"
#include "Menu.h"
#include "Level.h"
#include "Ranking.h"

class Game
{
public:
	Game();
	~Game();

	enum class GameState
	{
		MENU,
		LEVEL1,
		LEVEL2,
		RANKING,
		EXIT
	};

	GameState getGameState();
	void run();

private:
	Scene *m_currentScene;
	GameState m_gameState;
};