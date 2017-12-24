#include "Wall.h"



Wall::Wall(int x, int y): posI{x}, posJ{y}, powerUpMode{false},destroy{false},timer{10}, pati{false},casc{false}
{
	Renderer::Instance()->LoadTexture(ITEMS, PATH_IMG + "items.png");
	textWidth = Renderer::Instance()->GetTextureSize(ITEMS).x;
	textHeight = Renderer::Instance()->GetTextureSize(ITEMS).y;
	frameWidth = textWidth / 3;
	frameHeight = textHeight / 2;
	wallRect = { frameWidth, 0,frameWidth,frameHeight };
	wallPosition = { static_cast<int>((SCREEN_WIDTH / 15)* (posJ + 1)), static_cast<int>(((SCREEN_HEIGHT - 80) / 13)* (posI + 1) + 80), 48,48 };

	start = time(0);
}


Wall::~Wall()
{
}

void Wall::Update()
{
	//if (powerUpMode)
	//{
	//	if (difftime(time(0), start) == timer)
	//	{
	//		destroy = true;//tens 10 segons per agafar-lo, després es destrueix.
	//	}
	//}
}

void Wall::Draw()
{
	if (!destroy)
	{
		Renderer::Instance()->PushSprite(ITEMS, wallRect, wallPosition);
	}	
}

void Wall::destroyWall()
{
	if ((rand() % 100) < 100)
	{
		powerUpMode = true;
		createPowerUp();
	} 
	else destroy = true;
}

void Wall::createPowerUp()
{
	int n = rand() % 2 + 1;
	if (n == 1) pati = true;
	else if (n == 2) casc = true;

	wallRect = { frameWidth*(n), frameHeight,frameWidth,frameHeight };
}

bool Wall::playerGotIt(Player *p)
{
	if (p->posI == posI && p->posJ == posJ)
	{
		destroy = true;
		return true;
	}
	else
	{
		return false;
	}
	
}


