#include "Ranking.h"


Ranking::Ranking():hoverReturn{false}, mouseClicked{false}, exit{false},gotFile{false}
{
	m_sceneState = Scene::SceneState::Running;

	mouseRect.w = 0;
	mouseRect.h = 0;

	Renderer::Instance()->LoadFont({ SAIYAN, PATH_FONT + "saiyan.ttf", 80 });
	Renderer::Instance()->LoadFont({ GAME_OVER, PATH_FONT + "game_over.ttf", 30 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { RANKING_TEXT_BUTTON_RETURN, "RETURN",{ 0, 100, 0, 255 } ,SCREEN_WIDTH / 2, 81 });
	Renderer::Instance()->LoadTextureText(SAIYAN, { RANKING_TEXT_BUTTON_RETURN_HOVER, "RETURN",{ 0,200,0,255 } ,SCREEN_WIDTH / 2,81 });
}


Ranking::~Ranking()
{
}

void Ranking::Update()
{
	if (!gotFile)
	{
		getFileWinners();
		gotFile = true;
	}

	if (isCollisioning(mouseRect, returnRect)) hoverReturn = true;
	else hoverReturn = false;

	if (hoverReturn && mouseClicked) m_sceneState = Scene::SceneState::GoToMenu;
}

void Ranking::Draw()
{
	Renderer::Instance()->Clear();

	if (fileExists(rankingFile) && rankingVector.size()!=0)
	{
		for (int i = 0; i < rankingVector.size(); ++i)
		{
			SDL_Rect temporalRect = { static_cast<int>(SCREEN_WIDTH / 2.8), static_cast<int>( 25+ 50*(i+1) ) , SCREEN_WIDTH / 4, 40 };
			Renderer::Instance()->PushImage(RANKING_TEXT_WINNER + std::to_string(i + 1), temporalRect);
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


bool Ranking::fileExists(const std::string fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void Ranking::getFileWinners()
{
	//check if ranking exists
	if (fileExists(rankingFile))
	{
		int n;
		
		std::ifstream fentrada(rankingFile, std::ios::in | std::ios::binary);
		fentrada.read(reinterpret_cast<char *>(&n), sizeof(int));
		std::cout << "[RANKING] Number of winners: " << n << std::endl;

		for (int i = 0; i < n; i++)
		{
			winner *w = new winner();
			fentrada.read(reinterpret_cast<char *>(&w->name), sizeof(std::string));
			fentrada.read(reinterpret_cast<char *>(&w->points), sizeof(int));
			std::cout << "[RANKING] Winner " << w->name << " " << w->points << std::endl;
			rankingVector.push_back(w);
		}

		fentrada.close();

		for (int i = 0; i < rankingVector.size(); i++)
		{
			std::string temporal =rankingVector[i]->name + " won with " + std::to_string(rankingVector[i]->points) + " points";
			std::cout << "[RANKING]" + temporal << std::endl;
			std::string temp = RANKING_TEXT_WINNER + std::to_string(i + 1);
			std::cout << temp << std::endl;
			Renderer::Instance()->LoadTextureText(GAME_OVER, { temp , temporal,{ 0, 0, 0, 255 } ,0 , 0});
		}
	}
}