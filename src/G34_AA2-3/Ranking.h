#pragma once
#include "GameObject.h"
#include "Scene.h"
#include <fstream>

class Ranking :
	public Scene
{
public:
	Ranking();
	~Ranking();
	void EventHandler() override;
	void Update() override;
	void Draw() override;

private:
	std::vector<winner> rankingVector;
	const std::string rankingFile= PATH_FILES + "ranking.bin";
	bool fileExists(const std::string fileName);

	static constexpr SDL_Rect returnRect = { static_cast<int>(SCREEN_WIDTH / 2.5),static_cast<int>(SCREEN_HEIGHT - (SCREEN_HEIGHT / 11) * 1), 145,81 };

	SDL_Rect mouseRect;
	bool hoverReturn, exit, mouseClicked;

};