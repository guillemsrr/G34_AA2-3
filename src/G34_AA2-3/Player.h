#pragma once
#include "GameObject.h"
#include "Scene.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Constants.h"
#include <SDL_ttf.h>
#include "Bomb.h"

class Player//:
	//public GameObject//caldria fer que també fes ell mateix Update i Draw
{
public:
	Player(int num);
	~Player();
	int getPlayerTag();
	SDL_Rect playerPosition, playerRect;
	void bomb(bool explodingLimits[8]);
	Bomb *ptrBomb;
	bool isInPosition();
	bool moving;
	int posJ, posI, lives, points;

private:
	int playerTag, textWidth, textHeight, frameWidth, frameHeight, frameTime;
};