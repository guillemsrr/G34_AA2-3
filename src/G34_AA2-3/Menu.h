#pragma once
#include "Scene.h"
#include "Menu.h"

class Menu : public Scene
{
public:
	Menu();
	~Menu();

	void EventHandler() override;
	void Update() override;
	void Draw() override; 

private:
	static constexpr SDL_Rect level1Rect = { static_cast<int>(SCREEN_WIDTH / 2.5),static_cast<int>(SCREEN_HEIGHT - (SCREEN_HEIGHT / 5 )*5),137,81 };
	static constexpr SDL_Rect level2Rect = { static_cast<int>(SCREEN_WIDTH / 2.5),static_cast<int>(SCREEN_HEIGHT - (SCREEN_HEIGHT / 5) * 4),137,81 };
	static constexpr SDL_Rect rankingRect = { static_cast<int>(SCREEN_WIDTH / 2.5),static_cast<int>(SCREEN_HEIGHT - (SCREEN_HEIGHT / 5) * 3),137,81 };
	static constexpr SDL_Rect soundRect = { static_cast<int>(SCREEN_WIDTH / 2.5),static_cast<int>(SCREEN_HEIGHT - (SCREEN_HEIGHT / 5) * 2),137,81 };
	static constexpr SDL_Rect exitRect = { static_cast<int>(SCREEN_WIDTH / 2.5),static_cast<int>(SCREEN_HEIGHT - (SCREEN_HEIGHT / 5)*1), 145,81 };

	bool hoverLevel1, hoverLevel2, hoverRanking, hoverSound, hoverExit, mute, mouseClicked, exit;
	SDL_Rect mouseRect;
};