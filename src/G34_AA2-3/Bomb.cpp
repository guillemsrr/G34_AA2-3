#include "Bomb.h"
#define SPEED 10

Bomb::Bomb(int x, int y, SDL_Rect pos, bool explosionLimits[8]): bombPosition{pos}, explode{false}, end{false}, posI{x}, posJ{y}
{
	Renderer::Instance()->LoadTexture(ITEMS, PATH_IMG + "items.png");
	textWidth = Renderer::Instance()->GetTextureSize(ITEMS).x;
	textHeight = Renderer::Instance()->GetTextureSize(ITEMS).y;
	frameWidth = textWidth / 3;
	frameHeight = textHeight / 2;
	bombRect = { 0,frameHeight,frameWidth,frameHeight };

	Renderer::Instance()->LoadTexture(EXPLOSIONS, PATH_IMG + "explosion.png");
	textWidth = Renderer::Instance()->GetTextureSize(EXPLOSIONS).x;
	textHeight = Renderer::Instance()->GetTextureSize(EXPLOSIONS).y;
	frameWidth = textWidth / 4;
	frameHeight = textHeight / 7;

	centralExplosionRect = { 0,0,frameWidth,frameHeight };
	explosion0Rect = { 0,frameHeight * 3,frameWidth,frameHeight };
	explosion1Rect = { 0,frameHeight * 6,frameWidth,frameHeight };
	explosion2Rect = { 0,frameHeight * 5,frameWidth,frameHeight };
	explosion3Rect = { 0,frameHeight,frameWidth,frameHeight };
	explosion4Rect = { 0,frameHeight * 5,frameWidth,frameHeight };
	explosion5Rect = { 0,frameHeight * 2,frameWidth,frameHeight };
	explosion6Rect = { 0,frameHeight * 6,frameWidth,frameHeight };
	explosion7Rect = { 0,frameHeight * 4,frameWidth,frameHeight };

	for (int i = 0; i <= 8; i++)
	{
		explodingLimits[i] = explosionLimits[i];
	}
	//comencem el temps:
	start = time(0);
}

Bomb::~Bomb()
{
}
void Bomb :: Update()
{
	if ((difftime(time(0),start)>=3))
	{
		if (!explode) 
		{
			explode = true;
			start = time(0);
		}
		else
		{
			end = true;
		}
	}
	else if (explode)
	{
		frameTime++;
		if (SCREEN_FPS / frameTime <= SPEED)
		{
			frameTime = 0;
			centralExplosionRect.x += centralExplosionRect.w;
			if (centralExplosionRect.x >= textWidth)
				centralExplosionRect.x = 0;

			if (explodingLimits[0])
			{
				explosion0Rect.x += explosion0Rect.w;
				if (explosion0Rect.x >= textWidth)
					explosion0Rect.x = 0;
			}
			if (explodingLimits[1])
			{
				explosion1Rect.x += explosion1Rect.w;
				if (explosion1Rect.x >= textWidth)
					explosion1Rect.x = 0;
			}
			if (explodingLimits[2])
			{
				explosion2Rect.x += explosion2Rect.w;
				if (explosion2Rect.x >= textWidth)
					explosion2Rect.x = 0;
			}
			if (explodingLimits[3])
			{
				explosion3Rect.x += explosion3Rect.w;
				if (explosion3Rect.x >= textWidth)
					explosion3Rect.x = 0;
			}
			if (explodingLimits[4])
			{
				explosion4Rect.x += explosion4Rect.w;
				if (explosion4Rect.x >= textWidth)
					explosion4Rect.x = 0;
			}
			if (explodingLimits[5])
			{
				explosion5Rect.x += explosion5Rect.w;
				if (explosion5Rect.x >= textWidth)
					explosion5Rect.x = 0;
			}
			if (explodingLimits[6])
			{
				explosion6Rect.x += explosion6Rect.w;
				if (explosion6Rect.x >= textWidth)
					explosion6Rect.x = 0;
			}
			if (explodingLimits[7])
			{
				explosion7Rect.x += explosion7Rect.w;
				if (explosion7Rect.x >= textWidth)
					explosion7Rect.x = 0;
			}
		}
	}
}

void Bomb::Draw()
{
	if(!explode) Renderer::Instance()->PushSprite(ITEMS, bombRect, bombPosition);
	else 
	{
		Renderer::Instance()->PushSprite(EXPLOSIONS, centralExplosionRect, bombPosition);
		if (explodingLimits[0])
		{
			Renderer::Instance()->PushSprite(EXPLOSIONS, explosion0Rect, { bombPosition.x,bombPosition.y - 48*2,48,48 });
		}
		if (explodingLimits[1])
		{
			Renderer::Instance()->PushSprite(EXPLOSIONS, explosion1Rect, { bombPosition.x,bombPosition.y - 48,48,48 });
		}
		if (explodingLimits[2])
		{
			Renderer::Instance()->PushSprite(EXPLOSIONS, explosion2Rect, { bombPosition.x - 48,bombPosition.y,48,48 });
		}
		if (explodingLimits[3])
		{
			Renderer::Instance()->PushSprite(EXPLOSIONS, explosion3Rect, { bombPosition.x - 48*2,bombPosition.y,48,48 });
		}
		if (explodingLimits[4])
		{
			Renderer::Instance()->PushSprite(EXPLOSIONS, explosion4Rect, { bombPosition.x + 48,bombPosition.y,48,48 });
		}
		if (explodingLimits[5])
		{
			Renderer::Instance()->PushSprite(EXPLOSIONS, explosion5Rect, { bombPosition.x + 48*2,bombPosition.y,48,48 });
		}
		if (explodingLimits[6])
		{
			Renderer::Instance()->PushSprite(EXPLOSIONS, explosion6Rect, { bombPosition.x,bombPosition.y + 48,48,48 });
		}
		if (explodingLimits[7])
		{
			Renderer::Instance()->PushSprite(EXPLOSIONS, explosion7Rect, { bombPosition.x,bombPosition.y + 48 * 2,48,48 });
		}
	} 
}
