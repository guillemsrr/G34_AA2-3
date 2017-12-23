#include "HUD.h"

HUD::HUD(Player *p1, Player *p2): player1{p1}, player2{p2}
{
	start = time(0);
	timer = 80;
	Renderer::Instance()->LoadFont({ GAME_OVER, PATH_FONT + "game_over.ttf", 80 });	
	Renderer::Instance()->LoadTextureText(GAME_OVER, { PLAYER1_TEXT, "Player 1:     lives,    points",{ 255, 255, 255, 255 } ,HUD_WIDTH , HUD_HEIGHT });
	Renderer::Instance()->LoadTextureText(GAME_OVER, { PLAYER2_TEXT, "Player 2:     lives,    points",{ 255, 255, 255, 255 } ,HUD_WIDTH , HUD_HEIGHT});
}

HUD::~HUD()
{
}

void HUD::Update()
{
	if (difftime(time(0), start) == 1)
	{
		timer--;
		Renderer::Instance()->LoadTextureText(GAME_OVER, { TIME_TEXT, std::to_string(timer),{ 255, 255, 255, 255 } ,HUD_WIDTH/3 *2, HUD_HEIGHT/2 });
		start = time(0);
	}
	Renderer::Instance()->LoadTextureText(GAME_OVER, { ACTUAL_PLAYER1_LIVES_TEXT, std::to_string(player1->lives),{ 255, 0, 0, 255 } ,HUD_WIDTH, HUD_HEIGHT});
	Renderer::Instance()->LoadTextureText(GAME_OVER, { ACTUAL_PLAYER2_LIVES_TEXT, std::to_string(player2->lives),{ 255, 0, 0, 255 } ,HUD_WIDTH , HUD_HEIGHT });
	Renderer::Instance()->LoadTextureText(GAME_OVER, { ACTUAL_PLAYER1_POINTS_TEXT, std::to_string(player1->points),{ 255, 255, 0, 255 } ,HUD_WIDTH, HUD_HEIGHT });
	Renderer::Instance()->LoadTextureText(GAME_OVER, { ACTUAL_PLAYER2_POINTS_TEXT, std::to_string(player2->points),{ 255, 255, 0, 255 } ,HUD_WIDTH , HUD_HEIGHT });
}

void HUD::Draw()
{
	Renderer::Instance()->PushImage(TIME_TEXT, { static_cast<int>(HUD_WIDTH),static_cast<int>(HUD_HEIGHT / 3) - 10,70,50 });
	Renderer::Instance()->PushImage(PLAYER1_TEXT, { SCREEN_WIDTH - HUD_WIDTH,static_cast<int>(HUD_HEIGHT / 2) - 30,200,30 });
	Renderer::Instance()->PushImage(PLAYER2_TEXT, { SCREEN_WIDTH - HUD_WIDTH,static_cast<int>(HUD_HEIGHT / 2) + 10,200,30 });
	Renderer::Instance()->PushImage(ACTUAL_PLAYER1_LIVES_TEXT, { SCREEN_WIDTH - HUD_WIDTH + 70,static_cast<int>(HUD_HEIGHT / 2) - 25,15,20 });
	Renderer::Instance()->PushImage(ACTUAL_PLAYER2_LIVES_TEXT, { SCREEN_WIDTH - HUD_WIDTH + 70,static_cast<int>(HUD_HEIGHT / 2) + 15,15,20 });
	Renderer::Instance()->PushImage(ACTUAL_PLAYER1_POINTS_TEXT, { SCREEN_WIDTH - HUD_WIDTH + 130,static_cast<int>(HUD_HEIGHT / 2) - 25,15,20 });
	Renderer::Instance()->PushImage(ACTUAL_PLAYER2_POINTS_TEXT, { SCREEN_WIDTH - HUD_WIDTH + 130,static_cast<int>(HUD_HEIGHT / 2) + 15,15,20 });
}
