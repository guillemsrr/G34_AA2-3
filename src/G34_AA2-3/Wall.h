#pragma once
#include "GameObject.h"
#include "Player.h"
#include <time.h>
class Wall :
	public GameObject
{
public:
	Wall(int x, int y);
	~Wall();

	virtual void Update() override;
	virtual void Draw() override;

	void destroyWall();
	
	int posI,posJ;
	int textWidth, textHeight, frameWidth, frameHeight;
	SDL_Rect wallRect, wallPosition;

	bool destroy, pati, casc;
	bool playerGotIt(Player *p);
private:
	void createPowerUp();
	bool powerUpMode;
	//TIME:
	time_t start;
	int timer;
};

