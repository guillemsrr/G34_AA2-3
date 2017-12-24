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
	static constexpr SDL_Rect level1Rect = { static_cast<int>(SCREEN_WIDTH / 2.5 - 25),static_cast<int>(SCREEN_HEIGHT - (SCREEN_HEIGHT / 5 )*5),200,81 };
	static constexpr SDL_Rect level2Rect = { static_cast<int>(SCREEN_WIDTH / 2.5 - 25),static_cast<int>(SCREEN_HEIGHT - (SCREEN_HEIGHT / 5) * 4),200,81 };
	static constexpr SDL_Rect rankingRect = { static_cast<int>(SCREEN_WIDTH / 2.5 - 25),static_cast<int>(SCREEN_HEIGHT - (SCREEN_HEIGHT / 5) * 3),200,81 };
	static constexpr SDL_Rect soundRect = { static_cast<int>(SCREEN_WIDTH / 2.5 - 75),static_cast<int>(SCREEN_HEIGHT - (SCREEN_HEIGHT / 5) * 2),300,81 };
	static constexpr SDL_Rect exitRect = { static_cast<int>(SCREEN_WIDTH / 2.5),static_cast<int>(SCREEN_HEIGHT - (SCREEN_HEIGHT / 5)*1), 150,81 };

	bool hoverLevel1, hoverLevel2, hoverRanking, hoverSound, hoverExit, mouseClicked, exit;
	SDL_Rect mouseRect;

	//AUDIO
	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
	Mix_Music *soundtrack{ Mix_LoadMUS("../../res/au/menu.mp3") };
};