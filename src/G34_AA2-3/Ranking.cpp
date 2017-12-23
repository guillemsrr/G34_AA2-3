#include "Ranking.h"



Ranking::Ranking():hoverReturn{false}, mouseClicked{false}, exit{false}
{
	m_sceneState = SceneState::Running;
	mouseRect.w = 0;
	mouseRect.h = 0;

	Renderer::Instance()->LoadFont({ SAIYAN, PATH_FONT + "saiyan.ttf", 80 });
	Renderer::Instance()->LoadFont({ GAME_OVER, PATH_FONT + "game_over.ttf", 20 });
	//Renderer::Instance()->LoadTexture(MENU_BG, PATH_IMG + "Bomberman_2.jpg");
	Renderer::Instance()->LoadTextureText(SAIYAN, { RANKING_TEXT_BUTTON_RETURN, "RETURN",{ 0, 100, 0, 255 } ,SCREEN_WIDTH / 2, 81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { RANKING_TEXT_BUTTON_RETURN_HOVER, "RETURN",{ 0,200,0,255 } ,SCREEN_WIDTH / 2,81 });
	

	//check if ranking exists
	if (fileExists(rankingFile))
	{
		int n;
		winner w;
		std::ifstream fentrada(rankingFile, std::ios::in | std::ios::binary);
		fentrada.read(reinterpret_cast<char *>(&n), sizeof(n));
		std::cout << "[RANKING] number of winners: " << n << std::endl;

		for (int i = 0; i <= n; i++)
		{
			fentrada.read(reinterpret_cast<char *>(&w), sizeof(w));
			rankingVector.push_back(w);
		}
		fentrada.close();	
	}

	for (int i = 0; i < rankingVector.size(); i++)
	{
		Renderer::Instance()->LoadTextureText(GAME_OVER, { RANKING_TEXT_WINNER + std::to_string(i) , rankingVector[i].name + " won with " + std::to_string(rankingVector[i].points) + " points",{ 255, 255, 255, 255 } ,SCREEN_WIDTH , 81 });
	}
	
}


Ranking::~Ranking()
{
}

void Ranking::EventHandler()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			exit = true;
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

void Ranking::Update()
{
	if (isCollisioning(mouseRect, returnRect)) hoverReturn = true;
	else hoverReturn = false;

	if (hoverReturn && mouseClicked) m_sceneState = Scene::SceneState::GoToMenu;
}

void Ranking::Draw()
{
	Renderer::Instance()->Clear();

	if (rankingVector.size() != 0)
	{
		for (int i = 0; i <= rankingVector.size(); i++)
		{
			Renderer::Instance()->PushImage(RANKING_TEXT_WINNER + std::to_string(i), { static_cast<int>(SCREEN_WIDTH / 2.5), static_cast<int>(SCREEN_HEIGHT - (SCREEN_HEIGHT / rankingVector.size()) * i), 145, 81 });
		}
	}
	
	if (hoverReturn)
	{
		Renderer::Instance()->PushImage(RANKING_TEXT_BUTTON_RETURN_HOVER, returnRect);
	}
	else
	{
		Renderer::Instance()->PushImage(RANKING_TEXT_BUTTON_RETURN, returnRect);
	}

	Renderer::Instance()->Render();
}

bool Ranking::fileExists(const std::string fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}
