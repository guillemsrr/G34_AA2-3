#pragma once
#include "GameObject.h"
#include "Player.h"
#include <time.h>

class HUD:
	public GameObject
{
public:
	HUD(Player *p1, Player *p2);
	~HUD();
	virtual void Update() override;
	virtual void Draw() override;

	int timer;
private:
	Player *player1;
	Player *player2;

	//TIME:
	time_t start;
};

