#pragma once
#include "Renderer.h"
#include "Types.h"
#include "Constants.h"
#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "GameObject.h"

class Scene:
	public GameObject
{
public:
	Scene();
	~Scene();

	enum class SceneState
	{
		Running,
		GoToLevel1,
		GoToLevel2,
		GoToRanking,
		GoToMenu,
		Exit
	};

	virtual void EventHandler() = 0;
	SceneState getState();
	bool m_mute = true;
protected:
	bool isCollisioning(SDL_Rect r1, SDL_Rect r2);
	SceneState m_sceneState;
};
