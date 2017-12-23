#include "Menu.h"


Menu::Menu() :hoverLevel1{ false }, hoverLevel2{ false }, hoverRanking{ false }, hoverExit{ false }, hoverSound{ false }, mute{ false }, mouseClicked { false }, exit{ false }//, mouseRect.w{0}, mouseRect.h{0}
{
	m_sceneState = SceneState::Running;
	mouseRect.w = 0;
	mouseRect.h = 0;

	Renderer::Instance()->LoadFont({ SAIYAN, PATH_FONT + "saiyan.ttf", 80 });
	//Renderer::Instance()->LoadTexture(MENU_BG, PATH_IMG + "Bomberman_2.jpg");
	Renderer::Instance()->LoadTextureText(SAIYAN, { MENU_TEXT_BUTTON_LEVEL1, "LEVEL ONE",{ 0, 100, 0, 255 } ,SCREEN_WIDTH / 2, 81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { MENU_TEXT_BUTTON_LEVEL1_HOVER, "LEVEL ONE",{ 0,200,0,255 } ,SCREEN_WIDTH / 2,81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { MENU_TEXT_BUTTON_LEVEL2, "LEVEL TWO",{ 0,150,0,255 } ,SCREEN_WIDTH / 2,81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { MENU_TEXT_BUTTON_LEVEL2_HOVER, "LEVEL TWO",{ 0,250,0,255 } ,SCREEN_WIDTH / 2 ,81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { MENU_TEXT_BUTTON_EXIT, "EXIT",{ 100,0,0,255 } ,SCREEN_WIDTH / 2 ,81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { MENU_TEXT_BUTTON_EXIT_HOVER, "EXIT",{ 200,0,0,255 } ,SCREEN_WIDTH / 2 ,81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { MENU_TEXT_BUTTON_RANKING, "RANKING",{ 30,150,40,255 } ,SCREEN_WIDTH / 2 ,81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { MENU_TEXT_BUTTON_RANKING_HOVER, "RANKING",{ 55,178,67,255 } ,SCREEN_WIDTH / 2 ,81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { MENU_TEXT_BUTTON_DISABLESOUND, "DISABLE SOUND",{ 23,54,56,255 } ,SCREEN_WIDTH / 2 ,81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { MENU_TEXT_BUTTON_DISABLESOUND_HOVER, "DISABLE SOUND",{ 89,11,236,255 } ,SCREEN_WIDTH / 2 ,81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { MENU_TEXT_BUTTON_ENABLESOUND, "ENABLE SOUND",{ 23,54,56,255 } ,SCREEN_WIDTH / 2 ,81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { MENU_TEXT_BUTTON_ENABLESOUND_HOVER, "ENABLE SOUND",{ 89,11,236,255 } ,SCREEN_WIDTH / 2 ,81 });
}


void Menu::Draw()
{
	Renderer::Instance()->Clear();
	//Renderer::Instance()->PushImage(MENU_BG, { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT });
	if (hoverLevel1) 
	{
		Renderer::Instance()->PushImage(MENU_TEXT_BUTTON_LEVEL1_HOVER, level1Rect);
	}
	else 
	{
		Renderer::Instance()->PushImage(MENU_TEXT_BUTTON_LEVEL1, level1Rect);
	}

	if (hoverExit) 
	{
		Renderer::Instance()->PushImage(MENU_TEXT_BUTTON_EXIT_HOVER, exitRect);
	}
	else 
	{
		Renderer::Instance()->PushImage(MENU_TEXT_BUTTON_EXIT, exitRect);
	}
	if (hoverLevel2) 
	{
		Renderer::Instance()->PushImage(MENU_TEXT_BUTTON_LEVEL2_HOVER, level2Rect);
	}
	else
	{
		Renderer::Instance()->PushImage(MENU_TEXT_BUTTON_LEVEL2, level2Rect);
	}
	if (hoverRanking) 
	{
		Renderer::Instance()->PushImage(MENU_TEXT_BUTTON_RANKING_HOVER, rankingRect);
	}
	else 
	{
		Renderer::Instance()->PushImage(MENU_TEXT_BUTTON_RANKING, rankingRect);
	}
	if (hoverSound) 
	{
		if (mute) Renderer::Instance()->PushImage(MENU_TEXT_BUTTON_ENABLESOUND_HOVER, soundRect);
		else Renderer::Instance()->PushImage(MENU_TEXT_BUTTON_DISABLESOUND_HOVER, soundRect);

		
	}
	else
	{
		if(mute) Renderer::Instance()->PushImage(MENU_TEXT_BUTTON_ENABLESOUND, soundRect);
		else Renderer::Instance()->PushImage(MENU_TEXT_BUTTON_DISABLESOUND, soundRect);
		
	}

	Renderer::Instance()->Render();
}



Menu::~Menu()
{
}

void Menu::EventHandler()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:	
			exit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)exit = true; 
			break;
		case SDL_MOUSEBUTTONDOWN: 
			mouseClicked = true; 
			break;
		case SDL_MOUSEBUTTONUP:
			mouseClicked = false;
			break;
		case SDL_MOUSEMOTION: 	
			mouseRect.x = event.motion.x; 
			mouseRect.y = event.motion.y; 
			break;
		default:;
		}
	}
}

void Menu::Update()
{
	if (isCollisioning(mouseRect, level1Rect)) hoverLevel1 = true;
	else hoverLevel1 = false;

	if (isCollisioning(mouseRect, level2Rect)) hoverLevel2 = true;
	else hoverLevel2 = false;

	if (isCollisioning(mouseRect, rankingRect)) hoverRanking = true;
	else hoverRanking = false;

	if (isCollisioning(mouseRect, soundRect)) hoverSound = true;
	else hoverSound= false;

	if (isCollisioning(mouseRect, exitRect)) hoverExit = true;
	else hoverExit = false;
	
	if (hoverLevel1 && mouseClicked) m_sceneState = Scene::SceneState::GoToLevel1;
	if (hoverLevel2 && mouseClicked) m_sceneState = Scene::SceneState::GoToLevel2;
	if (hoverRanking && mouseClicked) m_sceneState = Scene::SceneState::GoToRanking;
	if (hoverSound && mouseClicked)
	{
		mute = !mute;
		mouseClicked = false;
		//treure el so.
	}
	if (exit||hoverExit && mouseClicked) m_sceneState = Scene::SceneState::Exit;
}