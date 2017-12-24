#pragma once
#include "Scene.h"
#include "Player.h"
#include <list>
#include <vector>
#include "HUD.h"
#include "Wall.h"
#include <fstream>
#include <vector>

#define SPEED 10

class Level :
	public Scene
{
public:
	Level(int num, bool mute);
	~Level();
	void EventHandler() override;
	void Update() override;
	void Draw() override;

private:
	Player *p1, *p2;
	int lvlNumber, textWidth, textHeight, frameWidth, frameHeight, frameTime;
	//control:
	std::string grid[11][13]; //grid[files][columnes] o grid[Y][X]
	int keyDown, keyUp;
	SDL_Rect blockRect, wallRect;
	bool exit;

	//explosions:
	void setExplosionLimits(Player *p);
	void checkDamage(Player *p);
	void changePlayerLocation(Player *p);

	//hud:
	HUD *m_hud;

	//Walls and PowerUps:
	Wall *w;
	std::list<Wall*> wallList;
	void detectWall(int x, int y);
	void upgradeWallList();

	//RANKING:
	const std::string rankingFile = PATH_FILES + "ranking.bin";
	bool fileExists(const std::string fileName);
	void putNameRanking(Player *p);

	//AUDIO
	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
	Mix_Music *soundtrack{ Mix_LoadMUS("../../res/au/game_theme.mp3") };
};

