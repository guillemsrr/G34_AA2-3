#include "Player.h"

Player::Player(int num) : playerTag{ num }, ptrBomb{ nullptr }, moving{ false }, stop{ false }, lives { 3 }, points{ 0 }, powerShield{false}, powerSpeed{false},extraSteps{1},begin{true}
{
	Renderer::Instance()->LoadTexture(PLAYER1_SPRITE, PATH_IMG + "player1.png");
	Renderer::Instance()->LoadTexture(PLAYER2_SPRITE, PATH_IMG + "player2.png");

	//les mesures del png de player1 i player2 s�n les mateixes.
	textWidth = Renderer::Instance()->GetTextureSize(PLAYER1_SPRITE).x;
	textHeight = Renderer::Instance()->GetTextureSize(PLAYER1_SPRITE).y;
	frameWidth = textWidth / 3;
	frameHeight = textHeight / 4;
	//per tant tots dos tenen el mateix PlayerRect.
	playerRect = { 0,0,frameWidth,frameHeight};

	//nom�s canvia el punt inicial x,y.
	if (num == 1)
	{
		playerPosition = { SCREEN_WIDTH/15,  (SCREEN_HEIGHT - 80) / 13 * 6 + 80, 48, 48 };
	}
	else if (num == 2)
	{
		playerPosition = { SCREEN_WIDTH- SCREEN_WIDTH / 15 * 2,  (SCREEN_HEIGHT - 80) / 13*6 + 80, 48, 48 };
	}
}

Player::~Player()
{
}

int Player::getPlayerTag()
{
	return playerTag;
}

void Player::bomb(bool explodingLimits[8])
{
	ptrBomb = new Bomb(posI,posJ,playerPosition, explodingLimits);
}

bool Player::isInPosition()
{
	if (((playerPosition.x % (SCREEN_WIDTH / 15)) == 0) && (((playerPosition.y - 80) % ((SCREEN_HEIGHT - 80) / 13)== 0)))
	{
		return true;
	}
	else 
		return false;
}

void Player::restartExplosionLimits()
{
	for (int i = 0; i <= 7; i++)
	{
		explosionLimits[i] = false;
	}
}

void Player::setPowerSpeed()
{
	if (begin)
	{
		extraSteps = 1.8;
		start=time(0);
		begin = false;
	}
	if (difftime(time(0), start) == 10)
	{
		extraSteps = 1.0;
		powerSpeed = false;
		begin = true;
	}
}

