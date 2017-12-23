#include "Game.h"


Game::Game()
{
	m_gameState = GameState::MENU;
	m_currentScene = new Menu();
}


Game::~Game()
{
	delete m_currentScene;
}

Game::GameState Game::getGameState()
{
	return m_gameState;
}

void Game::run()
{
	while (m_gameState != GameState::EXIT) {

		switch (m_currentScene->getState())
		{
		case Scene::SceneState::Running:
			m_currentScene->EventHandler();
			m_currentScene->Update();
			m_currentScene->Draw();
			break;

		case Scene::SceneState::Exit:
			switch (m_gameState)
			{
			case GameState::MENU:
				delete m_currentScene;
				m_currentScene = nullptr;
				m_gameState = GameState::EXIT;
				break;
			case GameState::LEVEL1:
				delete m_currentScene;
				m_currentScene = nullptr;
				m_gameState = GameState::EXIT;
				break;
			}
			break;

		case Scene::SceneState::GoToLevel1: //en realitat no caldria fer un switch pq sempre es cridarà al Menu? O per si de cas?
			switch (m_gameState)
			{
			case GameState::MENU:
				delete m_currentScene;
				m_currentScene = new Level(1);
				m_gameState = GameState::LEVEL1;
				break;
			default:
				break;
			}
			break;
		case Scene::SceneState::GoToLevel2: //en realitat no caldria fer un switch pq sempre es cridarà al Menu? O per si de cas?
			switch (m_gameState)
			{
			case GameState::MENU:
				delete m_currentScene;
				m_currentScene = new Level(2);
				m_gameState = GameState::LEVEL2;
				break;
			default:
				break;
			}
			break;
		case Scene::SceneState::GoToRanking:
			switch (m_gameState)
			{
			case GameState::MENU:
				delete m_currentScene;
				m_currentScene = new Ranking();
				m_gameState = GameState::RANKING;
				break;
			//case GameState::LEVEL1:
			//	delete m_currentScene;
			//	m_currentScene = new Ranking();
			//	m_gameState = GameState::RANKING;
			//	break;
			//case GameState::LEVEL2:
			//	delete m_currentScene;
			//	m_currentScene = new Ranking();
			//	m_gameState = GameState::RANKING;
			//	break;
			//default:
				break;
			}
			break;
		case Scene::SceneState::GoToMenu: //en realitat no caldria fer un switch pq sempre es cridarà al Menu? O per si de cas?
			switch (m_gameState)
			{
			case GameState::RANKING:
				delete m_currentScene;
				m_currentScene = new Menu();
				m_gameState = GameState::MENU;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}