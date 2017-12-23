#include "Wall.h"



Wall::Wall(int x, int y): posI{x}, posJ{y}, powerUpMode{false},destroy{false}
{
	Renderer::Instance()->LoadTexture(ITEMS, PATH_IMG + "items.png");
	textWidth = Renderer::Instance()->GetTextureSize(ITEMS).x;
	textHeight = Renderer::Instance()->GetTextureSize(ITEMS).y;
	frameWidth = textWidth / 3;
	frameHeight = textHeight / 2;
	wallRect = { frameWidth, 0,frameWidth,frameHeight };
	wallPosition = { static_cast<int>((SCREEN_WIDTH / 15)* (posJ + 1)), static_cast<int>(((SCREEN_HEIGHT - 80) / 13)* (posI + 1) + 80), 48,48 };
}


Wall::~Wall()
{
}

void Wall::Update()
{

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
	if ((rand() % 100) < 20)
	{
		powerUpMode = true;
		createPowerUp();
	} 
	else{
		destroy = true;
	}
}
void Wall::createPowerUp()
{
	wallRect = { frameWidth*(rand() % 2 + 1), frameHeight,frameWidth,frameHeight };
}


