#include "Level.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_print.hpp"
#include <algorithm>
#include <vector>
#include <fstream>

Level::Level(int num, bool mute) : exit{ false }, lvlNumber{ num }, frameTime { 0 }, keyDown{ 0 }, p1{ new Player(1) }, p2{ new Player(2) }, m_hud{ new HUD(p1, p2) }
{
	srand(time(NULL));
	m_sceneState= Scene::SceneState::Running;
	Renderer::Instance()->LoadTexture(LEVEL_BG, PATH_IMG + "bgGame.jpg");
	Renderer::Instance()->LoadTexture(ITEMS, PATH_IMG + "items.png");

	textWidth = Renderer::Instance()->GetTextureSize(ITEMS).x;
	textHeight = Renderer::Instance()->GetTextureSize(ITEMS).y;
	frameWidth = textWidth / 6;
	frameHeight = textHeight / 2;

	blockRect = { 0,0, frameWidth,frameHeight };
	frameWidth = textWidth / 3;
	frameHeight = textHeight / 2;
	wallRect = { frameWidth, 0,frameWidth,frameHeight };
	
	for (int i = 0; i <= 10; i++)// i ->files
	{
		for (int j = 0; j <= 12; j++)//j ->columnes
		{
			grid[i][j] = "empty";
		}
	}

	if (lvlNumber == 1)
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("../../res/files/config.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc
		doc.parse<0>(&buffer[0]);

		root_node = doc.first_node("Game");
		rapidxml::xml_node<> * level = root_node->first_node("Level");
		m_hud->timer = 5;// atoi(level->first_attribute("time")->value());
		p1->lives = atoi(level->first_attribute("lives")->value());
		p2->lives = atoi(level->first_attribute("lives")->value());
		rapidxml::xml_node<> * estructura2 = level->first_node("Fixed");
		for (rapidxml::xml_node<> * block = estructura2->first_node("Wall"); block; block = block->next_sibling())
		{
			int tempx = atoi(block->first_attribute("i")->value());
			int	tempy = atoi(block->first_attribute("j")->value());;
			grid[tempx][tempy] = "block";
			std::cout << "block  i:  " << tempx << "  j: " << tempy << std::endl;
		}

		rapidxml::xml_node<> * estructura = level->first_node("Destructible");
		for (rapidxml::xml_node<> * walls = estructura->first_node("Wall"); walls; walls = walls->next_sibling())
		{
			int tempy = atoi(walls->first_attribute("i")->value());
			int	tempx = atoi(walls->first_attribute("j")->value());;
			grid[tempy][tempx] = "wall";
			wallList.push_back(new Wall(tempy, tempx));
		}

		grid[5][0] = "player1";
		grid[5][12] = "player2";
		p2->posI = 5;
		p2->posJ = 12;

		p1->posI = 5;
		p1->posJ = 0;
	}
	else
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("../../res/files/config.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc
		doc.parse<0>(&buffer[0]);

		root_node = doc.first_node("Game");
		rapidxml::xml_node<> * level = root_node->last_node("Level");
		//level->next_sibling();
		m_hud->timer = atoi(level->first_attribute("time")->value());
		p1->lives = atoi(level->first_attribute("lives")->value());
		p2->lives = atoi(level->first_attribute("lives")->value());
		rapidxml::xml_node<> * estructura = level->first_node("Destructible");
		for (rapidxml::xml_node<> * walls = estructura->first_node("Wall"); walls; walls = walls->next_sibling())
		{
			int tempy = atoi(walls->first_attribute("i")->value());
			int	tempx = atoi(walls->first_attribute("j")->value());;
			grid[tempy][tempx] = "wall";
			wallList.push_back(new Wall(tempy, tempx));
		}
		rapidxml::xml_node<> * estructura2 = level->first_node("Fixed");
		for (rapidxml::xml_node<> * block = estructura2->first_node("Wall"); block; block = block->next_sibling())
		{
			int tempx = atoi(block->first_attribute("i")->value());
			int	tempy = atoi(block->first_attribute("j")->value());;
			grid[tempx][tempy] = "block";
			std::cout << "block  i:  " << tempx << "  j: " << tempy << std::endl;
		}
		grid[5][0] = "player1";
		grid[5][12] = "player2";
		p2->posI = 5;
		p2->posJ = 12;

		p1->posI = 5;
		p1->posJ = 0;

	}



	// Audio:
	m_mute = mute;
	if (!m_mute)
	{
		if (!(Mix_Init(mixFlags) &mixFlags)) throw "Error: SDL_Mix init";
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) throw "";


		if (!soundtrack) throw "unable to open SDL soundtrack";
		Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
		Mix_PlayMusic(soundtrack, -1);
	}
	p1->restartExplosionLimits();
	p2->restartExplosionLimits();
}

Level::~Level()
{
}

void Level::EventHandler()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:	
			exit = true;	 
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) exit = true;
			keyDown = event.key.keysym.sym;
			if (keyUp == keyDown) keyUp = NULL;
			break;
		case SDL_KEYUP:
			keyUp = event.key.keysym.sym;
				if (keyUp == keyDown) keyDown = NULL;
			break;
			
		default:;
		}
	}
}

void Level::Update()
{
	if (exit) m_sceneState = Scene::SceneState::Exit;
	if (m_hud->timer <= 0 || p1->lives==0|| p2->lives==0)
	{
		if (p1->points > p2->points)
		{
			putNameRanking(p1);
		}
		else if(p2->points> p1->points)
		{
			putNameRanking(p2);
		}
		else
		{
			std::cout << "Tie!\n";
		}
		m_sceneState = Scene::SceneState::GoToRanking;
	}

	if (keyDown == SDLK_m)
	{
		for (int i = 0; i <= 10; i++)
		{
			for (int j = 0; j <= 12; j++)
			{
				std::cout << grid[i][j] << " ";
			}
			std::cout << std::endl;
		}

		keyDown = NULL;
		std::cout << std::endl; std::cout << std::endl; std::cout << std::endl; std::cout << std::endl; std::cout << std::endl; std::cout << std::endl;

	}
	//Players movement:

	//Handle KeyDown
	//Player 1:
	if (!p1->moving)
	{
		if (keyDown == SDLK_w || keyDown == SDLK_a || keyDown == SDLK_s || keyDown == SDLK_d || keyDown == SDLK_SPACE)
		{
			p1->key = keyDown;
		}		
	}
	//Player 2:
	if (!p2->moving)
	{
		if (keyDown == SDLK_UP || keyDown == SDLK_LEFT || keyDown == SDLK_DOWN || keyDown == SDLK_RIGHT || keyDown == SDLK_RCTRL)
		{
			p2->key = keyDown;
		}
	}
	//Handle KeyUp
	//Player 1:
	if (keyUp == SDLK_w || keyUp == SDLK_a || keyUp == SDLK_s || keyUp == SDLK_d || keyUp == SDLK_SPACE)
	{
		p1->stop = true;
	}
	if (keyUp == SDLK_UP || keyUp == SDLK_LEFT || keyUp == SDLK_DOWN || keyUp == SDLK_RIGHT || keyUp == SDLK_RCTRL)
	{
		p2->stop = true;
	}

	//check speed power ups:
	if (p1->powerSpeed)
	{
		p1->setPowerSpeed();
	}
	if (p2->powerSpeed)
	{
		p2->setPowerSpeed();
	}

	//Actual Movement:
	frameTime++;
	if (SCREEN_FPS / frameTime <= p1->extraSteps*SPEED)
	{
		//Player 1:
		if (p1->key == SDLK_w && p1->posI > 0)
		{
			if (grid[p1->posI - 1][p1->posJ] == "empty")
			{
				p1->moving = true;
				frameTime = 0;
				p1->playerRect.y = 0;
				p1->playerRect.x += p1->playerRect.w;
				if (p1->playerRect.x >= p1->playerRect.w * 2)
					p1->playerRect.x = 0;
				p1->playerPosition.y -= STEPS;

				if (p1->isInPosition())
				{
					grid[p1->posI][p1->posJ] = "empty";
					p1->posI--;
					grid[p1->posI][p1->posJ] = "player1";
				}
			}
		}
		else if (p1->key == SDLK_a && p1->posJ > 0)
		{
			if (grid[p1->posI][p1->posJ - 1] == "empty")
			{
				p1->moving = true;
				frameTime = 0;
				p1->playerRect.y = p1->playerRect.h;
				p1->playerRect.x += p1->playerRect.w;
				if (p1->playerRect.x >= p1->playerRect.w * 3)
					p1->playerRect.x = 0;
				p1->playerPosition.x -= STEPS;
				if (p1->isInPosition())
				{
					grid[p1->posI][p1->posJ] = "empty";
					p1->posJ--;
					grid[p1->posI][p1->posJ] = "player1";
				}
			}
		}
		else if (p1->key == SDLK_s && p1->posI < 12)
		{
			if (grid[p1->posI + 1][p1->posJ] == "empty")
			{
				p1->moving = true;
				frameTime = 0;
				p1->playerRect.y = p1->playerRect.h * 2;
				p1->playerRect.x += p1->playerRect.w;
				if (p1->playerRect.x >= p1->playerRect.w * 3)
					p1->playerRect.x = 0;
				p1->playerPosition.y += STEPS;
				if (p1->isInPosition())
				{
					grid[p1->posI][p1->posJ] = "empty";
					p1->posI++;
					grid[p1->posI][p1->posJ] = "player1";
				}
			}
		}

		else if (p1->key == SDLK_d && p1->posJ < 12)
		{
			if (grid[p1->posI][p1->posJ + 1] == "empty")
			{
				p1->moving = true;
				frameTime = 0;
				p1->playerRect.y = p1->playerRect.h * 3;
				p1->playerRect.x += p1->playerRect.w;
				if (p1->playerRect.x >= p1->playerRect.w * 3)
					p1->playerRect.x = 0;
				p1->playerPosition.x += STEPS;
				if (p1->isInPosition())
				{
					grid[p1->posI][p1->posJ] = "empty";
					p1->posJ++;
					grid[p1->posI][p1->posJ] = "player1";
				}
			}
		}
		else if (p1->key == SDLK_SPACE)
		{
			if (p1->ptrBomb == nullptr)
			{
				setExplosionLimits(p1);
				p1->bomb(p1->explosionLimits);
				p1->key = NULL;
			}
		}
	}
	if (SCREEN_FPS / frameTime <= p2->extraSteps*SPEED)
	{
		//Player 2:
		if (p2->key == SDLK_UP && p2->posI > 0)
		{
			if (grid[p2->posI - 1][p2->posJ] == "empty")
			{
				p2->moving = true;
				frameTime = 0;
				p2->playerRect.y = 0;
				p2->playerRect.x += p2->playerRect.w;
				if (p2->playerRect.x >= p2->playerRect.w * 2)
					p2->playerRect.x = 0;
				p2->playerPosition.y -= STEPS;
				if (p2->isInPosition())
				{
					grid[p2->posI][p2->posJ] = "empty";
					p2->posI--;
					grid[p2->posI][p2->posJ] = "player2";
				}
			}
		}
		else if (p2->key == SDLK_LEFT && p2->posJ > 0)
		{
			if (grid[p2->posI][p2->posJ - 1] == "empty")
			{
				p2->moving = true;
				frameTime = 0;
				p2->playerRect.y = p2->playerRect.h;
				p2->playerRect.x += p2->playerRect.w;
				if (p2->playerRect.x >= p2->playerRect.w * 3)
					p2->playerRect.x = 0;
				p2->playerPosition.x -= STEPS;
				if (p2->isInPosition())
				{
					grid[p2->posI][p2->posJ] = "empty";
					p2->posJ--;
					grid[p2->posI][p2->posJ] = "player2";
				}
			}
		}
		else if (p2->key == SDLK_DOWN && p2->posI < 10)
		{
			if (grid[p2->posI + 1][p2->posJ] == "empty")
			{
				p2->moving = true;
				frameTime = 0;
				p2->playerRect.y = p2->playerRect.h * 2;
				p2->playerRect.x += p2->playerRect.w;
				if (p2->playerRect.x >= p2->playerRect.w * 3)
					p2->playerRect.x = 0;
				p2->playerPosition.y += STEPS;
				if (p2->isInPosition())
				{
					grid[p2->posI][p2->posJ] = "empty";
					p2->posI++;
					grid[p2->posI][p2->posJ] = "player2";
				}
			}
		}
		else if (p2->key == SDLK_RIGHT && p2->posJ < 12)
		{
			if (grid[p2->posI][p2->posJ + 1] == "empty")
			{
				p2->moving = true;
				frameTime = 0;
				p2->playerRect.y = p2->playerRect.h * 3;
				p2->playerRect.x += p2->playerRect.w;
				if (p2->playerRect.x >= p2->playerRect.w * 3)
					p2->playerRect.x = 0;
				p2->playerPosition.x += STEPS;
				if (p2->isInPosition())
				{
					grid[p2->posI][p2->posJ] = "empty";
					p2->posJ++;
					grid[p2->posI][p2->posJ] = "player2";
				}
			}
		}
		else if (p2->key == SDLK_RCTRL)
		{

			if (p2->ptrBomb == nullptr)
			{
				setExplosionLimits(p2);
				p2->bomb(p2->explosionLimits);
				p2->key = NULL;
			}
		}
	}

	if (p1->isInPosition() && p1->stop)
	{
		p1->key = NULL;
		p1->moving = false;
		p1->stop = false;
	}
	if (p2->isInPosition() && p2->stop)
	{
		p2->key = NULL;
		p2->moving = false;
		p2->stop = false;
	}

	//Bombs:
	if (p1->ptrBomb != nullptr)
	{
		if (!p1->ptrBomb->end)
		{
			p1->ptrBomb->Update();
			if (p1->ptrBomb->explode)
			{
				checkDamage(p1);
			}
		}
		if (p1->ptrBomb->end)
		{
			p1->restartExplosionLimits();
			delete p1->ptrBomb;
			p1->ptrBomb = nullptr;
		}
	}
	
	if (p2->ptrBomb != nullptr)
	{
		if (!p2->ptrBomb->end)
		{
			p2->ptrBomb->Update();
			if (p2->ptrBomb->explode)
			{
				checkDamage(p2);
			}
		}
		if (p2->ptrBomb->end)
		{
			p2->restartExplosionLimits();
			delete p2->ptrBomb;
			p2->ptrBomb = nullptr;
		}
	}

	//HUD:
	m_hud->Update();

	//WALLS and powerUps:
	upgradeWallList();
}

void Level::Draw()
{
	Renderer::Instance()->Clear();

	//Background
	Renderer::Instance()->PushImage(LEVEL_BG, { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT });
	
	//Blocks://**************************************************************************************************************************************************
	for (int i = 0; i <= 10; i++)
	{
		for (int j = 0; j <= 12; j++)
		{
			if (grid[i][j]=="block")
			{
				SDL_Rect blockPosition = { static_cast<int>((SCREEN_WIDTH / 15)* (j + 1)), static_cast<int>(((SCREEN_HEIGHT - 80) / 13)* (i + 1) + 80), 48,48 };
				Renderer::Instance()->PushSprite(ITEMS, blockRect, blockPosition);
			}
			//if (grid[i][j] == "wall")
			//{
			//	SDL_Rect wallPosition = { static_cast<int>((SCREEN_WIDTH / 15)* (j + 1)), static_cast<int>(((SCREEN_HEIGHT - 80) / 13)* (i + 1) + 80), 48,48 };
			//	Renderer::Instance()->PushSprite(ITEMS, wallRect, wallPosition);
			//}
		}
	}
	//Walls:***********************************************************************************************************************************************************
	for (std::list<Wall*>::const_iterator it = wallList.cbegin(); it != wallList.cend(); ++it)
	 {
	 w = *it;
	 w->Draw();
	 }
	
	//Bombs:
	if (p1->ptrBomb != nullptr)
	{
		if(!p1->ptrBomb->end)
			p1->ptrBomb->Draw();
	}
	if (p2->ptrBomb != nullptr)
	{
		if(!p2->ptrBomb->end)
			p2->ptrBomb->Draw();
	}

	//Animated Sprite
	Renderer::Instance()->PushSprite(PLAYER1_SPRITE, p1->playerRect, p1->playerPosition);
	Renderer::Instance()->PushSprite(PLAYER2_SPRITE, p2->playerRect, p2->playerPosition);
	
	//HUD:
	m_hud->Draw();
	
	Renderer::Instance()->Render();
}

void Level::setExplosionLimits(Player *p)
{
	if (p->posI - 1 >= 0 && grid[p->posJ][p->posI - 1] != "block")
	{
		p->explosionLimits[1] = true;
		if (p->posI - 2 >= 0 && grid[p->posJ][p->posI - 2] != "block")
		{
			p->explosionLimits[0] = true;
		}
	}
	
	if (p->posI + 1 <= 10 && grid[p->posJ][p->posI + 1] != "block")
	{
		p->explosionLimits[6] = true;
		if (p->posI + 2 <= 10 && grid[p->posJ][p->posI + 2] != "block")
		{
			p->explosionLimits[7] = true;
		}
	}
	
	if (p->posJ - 1 >= 0 && grid[p->posJ - 1][p->posI] != "block")
	{
		p->explosionLimits[2] = true;
		if (p->posJ - 2 >= 0 && grid[p->posJ - 2][p->posI] != "block")
		{
			p->explosionLimits[3] = true;
		}
	}
	
	if (p->posJ + 1 <= 12 && grid[p->posJ + 1][p->posI] != "block")
	{
		p->explosionLimits[4] = true;
		if (p->posJ + 2 <= 12 && grid[p->posJ + 2][p->posI] != "block")
		{
			p->explosionLimits[5] = true;
		}
	}
	

}

void Level::checkDamage(Player *p)
{

	int f = p->ptrBomb->posI;
	int c = p->ptrBomb->posJ;

	if (grid[f][c] == "player1" && !p1->powerShield)
	{
		p1->lives--;
		if (p->getPlayerTag() == 2) p2->points += 100;
		grid[f][c] = "empty";
		changePlayerLocation(p1);
	}
	else if (grid[f][c] == "player2" && !p2->powerShield)
	{
		p2->lives--;
		if (p->getPlayerTag() == 1) p1->points += 100;
		grid[f][c] = "empty";
		changePlayerLocation(p2);
	}


	if (p->explosionLimits[0])
	{
		f = p->ptrBomb->posI - 2;
		c = p->ptrBomb->posJ;

		if (grid[f][c] == "player1" && !p1->powerShield)
		{
			p1->lives--;
			if(p->getPlayerTag() == 2) p2->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[f][c] == "player2" && !p2->powerShield)
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[f][c] == "wall")
		{
			p->points += 15;
			detectWall(f, c);
			grid[f][c] = "empty";
		}
	}
	if (p->explosionLimits[1])
	{
		f = p->ptrBomb->posI - 1;
		c = p->ptrBomb->posJ;

		if (grid[f][c] == "player1" && !p1->powerShield)
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[f][c] == "player2" && !p2->powerShield)
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[f][c] == "wall")
		{
			p->points += 15;
			detectWall(f, c);
			grid[f][c] = "empty";
		}
	}
	if (p->explosionLimits[2])
	{
		f = p->ptrBomb->posI;
		c = p->ptrBomb->posJ-1;

		if (grid[f][c] == "player1" && !p1->powerShield)
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[f][c] == "player2" && !p2->powerShield)
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[f][c] == "wall")
		{
			p->points += 15;
			detectWall(f, c);
			grid[f][c] = "empty";
		}
	}
	if (p->explosionLimits[3])
	{
		f = p->ptrBomb->posI;
		c = p->ptrBomb->posJ-2;

		if (grid[f][c] == "player1" && !p1->powerShield)
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[f][c] == "player2" && !p2->powerShield)
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[f][c] == "wall")
		{
			p->points += 15;
			detectWall(f, c);
			grid[f][c] = "empty";
		}
	}
	if (p->explosionLimits[4])
	{
		f = p->ptrBomb->posI;
		c = p->ptrBomb->posJ+1;

		if (grid[f][c] == "player1" && !p1->powerShield)
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[f][c] == "player2" && !p2->powerShield)
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[f][c] == "wall")
		{
			p->points += 15;
			detectWall(f, c);
			grid[f][c] = "empty";
		}
	}
	if (p->explosionLimits[5])
	{
		f = p->ptrBomb->posI;
		c = p->ptrBomb->posJ+2;

		if (grid[f][c] == "player1" && !p1->powerShield)
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[f][c] == "player2" && !p2->powerShield)
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[f][c] == "wall")
		{
			p->points += 15;
			detectWall(f, c);
			grid[f][c] = "empty";
		}
	}
	if (p->explosionLimits[6])
	{
		f = p->ptrBomb->posI + 1;
		c = p->ptrBomb->posJ;

		if (grid[f][c] == "player1" && !p1->powerShield)
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[f][c] == "player2" && !p2->powerShield)
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[f][c] == "wall")
		{
			p->points += 15;
			detectWall(f, c);
			grid[f][c] = "empty";
		}
	}
	if (p->explosionLimits[7])
	{
		f = p->ptrBomb->posI + 2;
		c = p->ptrBomb->posJ;

		if (grid[f][c] == "player1" && !p1->powerShield)
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[f][c] == "player2" && !p2->powerShield)
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[f][c] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[f][c] == "wall")
		{
			p->points += 15;
			detectWall(f, c);
			grid[f][c] = "empty";
		}
	}	
}

void Level::changePlayerLocation(Player *p) //faltaria veure què passaria si no hi hagués cap espai.
{
	bool done = false;
	while (!done)
	{
		int i= rand() % 11;
		int j = rand() % 13;
		if (grid[i][j] == "empty")
		{
			grid[i][j] = "player" + std::to_string(p->getPlayerTag());
			p->posI = i;
			p->posJ = j;
			p->playerPosition = { static_cast<int>((SCREEN_WIDTH / 15)* (j + 1)), static_cast<int>(((SCREEN_HEIGHT - 80) / 13)* (i + 1) + 80), 48,48 };
			done = true;
		}
	}
}

void Level::detectWall(int x, int y)
{
	for (std::list<Wall*>::const_iterator it = wallList.cbegin(); it != wallList.cend(); ++it)
	{
		w = *it;
		if (w->posI == x && w->posJ == y)
		{
			w->destroyWall();
		}
	}
}

bool Level::fileExists(const std::string fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void Level::putNameRanking(Player *p)
{
	std::cout << "Player " + std::to_string(p->getPlayerTag()) + " wins!\n";

	//check if ranking exists
	if (fileExists(rankingFile))
	{
		int n;
		bool introduced=false;

		std::ifstream fentrada(rankingFile, std::ios::in | std::ios::binary);
		fentrada.read(reinterpret_cast<char *>(&n), sizeof(int));
		std::cout << "Number of winners: " << n << std::endl;

		std::vector<winner*> rankingVector;

		for (int i = 0; i < n; i++)
		{
			winner *w = new winner;
			fentrada.read(reinterpret_cast<char *>(&w->name), sizeof(std::string));
			fentrada.read(reinterpret_cast<char *>(&w->points), sizeof(int));

			if (!introduced && p->points >= w->points)
			{
				winner *newWinner=new winner;
				std::cout << "Introduce your name: \n";
				std::cin >> newWinner->name;
				newWinner->points = p->points;
				rankingVector.push_back(newWinner);
				if (n < 10)
				{
					++n;
					++i;
				}
				introduced = true;
			}
			if (i < 10)
			{
				rankingVector.push_back(w);
			}
		}
		fentrada.close();

		if (introduced)//upgrade binary file
		{
			winner *w;
			std::ofstream fsalida(rankingFile, std::ios::out | std::ios::binary);
			fsalida.write(reinterpret_cast<char *>(&n), sizeof(n));
			for (int i = 0; i < n; i++)
			{
				w = rankingVector[i];
				fsalida.write(reinterpret_cast<char *>(&w->name), sizeof(std::string));
				fsalida.write(reinterpret_cast<char *>(&w->points), sizeof(int));
			}
			fsalida.close();
		}	
	}
	else
	{
		int n=1;
		winner w;
		std::cout << "Introduce your name: \n";
		std::cin >> w.name;
		w.points = p->points;
		std::ofstream fsalida(rankingFile, std::ios::out | std::ios::binary);
		fsalida.write(reinterpret_cast<char *>(&n), sizeof(int));
		fsalida.write(reinterpret_cast<char *>(&w.name), sizeof(std::string));
		fsalida.write(reinterpret_cast<char *>(&w.points), sizeof(int));
		std::cout << "Winner introduced\n";
		fsalida.close();
	}
}

void Level::upgradeWallList()
{
	std::list<Wall*>::iterator it = wallList.begin();
	while(it != wallList.end())
	{
		w = *it;
		w->Update();
		
		if (w->playerGotIt(p1))
		{
			if (w->casc)
			{
				p1->powerShield = true;
			}
			else if (w->pati)
			{
				p1->powerSpeed = true;
			}
		}
		else if (w->playerGotIt(p2))
		{
			if (w->casc)
			{
				p2->powerShield = true;
			}
			else if (w->pati)
			{
				p2->powerSpeed = true;
			}
		}
		if (w->destroy)
		{
			w = nullptr;
			it=wallList.erase(it);
		}
		else
		{
			it++;
		}
	}
}