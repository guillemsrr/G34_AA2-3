#pragma once
#include "GameObject.h"
#include "Scene.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Constants.h"
#include <SDL_ttf.h>
#include "Bomb.h"
#include <time.h>

class Player//:
	//public GameObject//caldria fer que també fes ell mateix Update i Draw
{
public:
	Player(int num);
	~Player();
	int getPlayerTag();
	SDL_Rect playerPosition, playerRect;
	bool explosionLimits[8];
	void restartExplosionLimits();
	void bomb(bool explodingLimits[8]);
	Bomb *ptrBomb;
	bool isInPosition();
	bool moving, stop;
	int key,posJ, posI, lives, points;
	bool powerSpeed, powerShield;
	float extraSteps;
	void setPowerSpeed();

private:
	int playerTag, textWidth, textHeight, frameWidth, frameHeight, frameTime, start;
	bool begin;
};