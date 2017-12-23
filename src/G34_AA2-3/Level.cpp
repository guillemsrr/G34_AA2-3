#include "Level.h"

Level::Level(int num, bool mute) : exit{ false }, lvlNumber{ num }, frameTime { 0 }, keyDown{ 0 }, p1{ new Player(1) }, p2{ new Player(2) }, m_hud{ new HUD(p1, p2) }
{
	m_sceneState= Scene::SceneState::Running;
	Renderer::Instance()->LoadTexture(LEVEL_BG, PATH_IMG + "bgGame.jpg");
	Renderer::Instance()->LoadTexture(ITEMS, PATH_IMG + "items.png");

	textWidth = Renderer::Instance()->GetTextureSize(ITEMS).x;
	textHeight = Renderer::Instance()->GetTextureSize(ITEMS).y;
	frameWidth = textWidth / 6;
	frameHeight = textHeight / 2;

	blockRect = { 0,0, frameWidth,frameHeight };
	wallRect = { frameWidth,0,frameWidth,frameHeight };

	//start the grid:
	for (int i = 0; i <= 10; i++)// i ->files
	{
		for (int j = 0; j <= 12; j++)//j ->columnes
		{
			if (i == 5 && j == 0)
			{
				grid[i][j] = "player1";
				p1->posI = i;
				p1->posJ = j;
			}
			else if (i == 5 && j == 12)
			{
				grid[i][j] = "player2";
				p2->posI = i;
				p2->posJ = j;
			}
			else if (i % 2 == 1 && j % 2 == 1 )
				grid[i][j] = "block";
			else if (rand() % 4 == 1)
			{
				wallList.push_back(new Wall(i,j));
				grid[i][j] = "wall";
			}
			//posar els blocks

			else grid[i][j] = "empty";
		}
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
			//********************************************************************************************************************************
			if(!p1->moving && !p2->moving) 
				keyDown = event.key.keysym.sym;
			//********************************************************************************************************************************
			break;
		default:;
		}
	}
}

void Level::Update()
{
	if (exit) m_sceneState = Scene::SceneState::Exit;
	int prova = 0;
	//prova = 78;
	if (m_hud->timer <= prova || p1->lives==0|| p2->lives==0)
	{
		p1->points = prova;

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
	//Movement:
	frameTime++;
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
	}
	if (SCREEN_FPS / frameTime <= SPEED)
	{
		if (keyDown == SDLK_w && p1->posI >0)
		{
			if (grid[p1->posI-1][p1->posJ] == "empty")
			{
				p1->moving = true;
				frameTime = 0;
				p1->playerRect.y = 0;
				p1->playerRect.x += p1->playerRect.w;
				if (p1->playerRect.x >= p1->playerRect.w * 2)
					p1->playerRect.x = 0;
				p1->playerPosition.y -= STEPS;//step;

				if (p1->isInPosition())
				{
					grid[p1->posJ][p1->posI] = "empty";
					p1->posI--;
					grid[p1->posJ][p1->posI] = "player1";
				}
			}
		}
		else if (keyDown == SDLK_a && p1->posJ>0)
		{
			if (grid[p1->posI][p1->posJ-1] == "empty")
			{
				p1->moving = true;
				frameTime = 0;
				p1->playerRect.y = p1->playerRect.h;
				p1->playerRect.x += p1->playerRect.w;
				if (p1->playerRect.x >= p1->playerRect.w * 3)
					p1->playerRect.x = 0;
				p1->playerPosition.x -= step;
				if (p1->isInPosition())
				{
					grid[p1->posJ][p1->posI] = "empty";
					p1->posJ--;
					grid[p1->posJ][p1->posI] = "player1";
				}
			}
		}
		else if (keyDown == SDLK_s && p1->posI<12)
		{
			if (grid[p1->posI+1][p1->posJ] == "empty")
			{
				p1->moving = true;
				frameTime = 0;
				p1->playerRect.y = p1->playerRect.h * 2;
				p1->playerRect.x += p1->playerRect.w;
				if (p1->playerRect.x >= p1->playerRect.w * 3)
					p1->playerRect.x = 0;
				p1->playerPosition.y += step;
				if (p1->isInPosition())
				{
					grid[p1->posJ][p1->posI] = "empty";
					p1->posI++;
					grid[p1->posJ][p1->posI] = "player1";
				}
			}
		}

		else if (keyDown == SDLK_d && p1->posJ<12)
		{
			if (grid[p1->posI][p1->posJ+1] == "empty")
			{
				p1->moving = true;
				frameTime = 0;
				p1->playerRect.y = p1->playerRect.h * 3;
				p1->playerRect.x += p1->playerRect.w;
				if (p1->playerRect.x >= p1->playerRect.w * 3)
					p1->playerRect.x = 0;
				p1->playerPosition.x += step;
				if (p1->isInPosition())
				{
					grid[p1->posJ][p1->posI] = "empty";
					p1->posJ++;
					grid[p1->posJ][p1->posI] = "player1";
				}
			}
		}
		if (keyDown == SDLK_UP && p2->posI>0)
		{
			if (grid[p2->posI-1][p2->posJ] == "empty")
			{
				p2->moving = true;
				frameTime = 0;
				p2->playerRect.y = 0;
				p2->playerRect.x += p2->playerRect.w;
				if (p2->playerRect.x >= p2->playerRect.w * 2)
					p2->playerRect.x = 0;
				p2->playerPosition.y -= step;
				if (p2->isInPosition())
				{
					grid[p2->posJ][p2->posI] = "empty";
					p2->posI--;
					grid[p2->posJ][p2->posI] = "player2";
				}
			}
		}
		else if (keyDown == SDLK_LEFT && p2->posJ>0)
		{
			if (grid[p2->posI][p2->posJ-1] == "empty")
			{
				p2->moving = true;
				frameTime = 0;
				p2->playerRect.y = p2->playerRect.h;
				p2->playerRect.x += p2->playerRect.w;
				if (p2->playerRect.x >= p2->playerRect.w * 3)
					p2->playerRect.x = 0;
				p2->playerPosition.x -= step;
				if (p2->isInPosition())
				{
					grid[p2->posJ][p2->posI] = "empty";
					p2->posJ--;
					grid[p2->posJ][p2->posI] = "player2";
				}
			}
		}
		else if (keyDown == SDLK_DOWN && p2->posI<10)
		{
			if (grid[p2->posI+1][p2->posJ] == "empty")
			{
				p2->moving = true;
				frameTime = 0;
				p2->playerRect.y = p2->playerRect.h * 2;
				p2->playerRect.x += p2->playerRect.w;
				if (p2->playerRect.x >= p2->playerRect.w * 3)
					p2->playerRect.x = 0;
				p2->playerPosition.y += step;
				if (p2->isInPosition())
				{
					grid[p2->posJ][p2->posI] = "empty";
					p2->posI++;
					grid[p2->posJ][p2->posI] = "player2";
				}
			}
		}
		else if (keyDown == SDLK_RIGHT && p2->posJ<12)
		{
			if (grid[p2->posI][p2->posJ+1] == "empty")
			{
				p2->moving = true;
				frameTime = 0;
				p2->playerRect.y = p2->playerRect.h * 3;
				p2->playerRect.x += p2->playerRect.w;
				if (p2->playerRect.x >= p2->playerRect.w * 3)
					p2->playerRect.x = 0;
				p2->playerPosition.x += step;
				if (p2->isInPosition())
				{
					grid[p2->posJ][p2->posI] = "empty";
					p2->posJ++;
					grid[p2->posJ][p2->posI] = "player2";
				}
			}
		}

		if (keyDown == SDLK_SPACE)
		{
			if (p1->ptrBomb == nullptr)
			{
				setExplosionLimits(p1);
				p1->bomb(p1->explosionLimits);
			}
		}
		if (keyDown == SDLK_RCTRL)
		{
			if (p2->ptrBomb == nullptr)
			{
				setExplosionLimits(p2);
				p2->bomb(p2->explosionLimits);//
			}
		}

		//*******************************************************************************************************
		if (!p2->moving && p1->isInPosition())
		{
			p1->moving = false;
			keyDown = NULL;
		}

		if (!p1->moving && p2->isInPosition())
		{
			p2->moving = false;
			keyDown = NULL;
		}
		//*******************************************************************************************************
	}

	//Map Limits:
	if (p1->playerPosition.x >= SCREEN_WIDTH - SCREEN_WIDTH / 15 * 2)
	{
		p1->playerPosition.x = SCREEN_WIDTH - SCREEN_WIDTH / 15 * 2;
		p1->moving = false;
	}
	if (p1->playerPosition.x < SCREEN_WIDTH / 15)
	{
		p1->playerPosition.x = SCREEN_WIDTH / 15;
		p1->moving = false;
	}
		
	if (p1->playerPosition.y < (SCREEN_HEIGHT - 80) / 13 + 80)
	{
		p1->playerPosition.y = (SCREEN_HEIGHT - 80) / 13 + 80;
		p1->moving = false;
	}
	//*******************************************************************************************************	
	if (p1->playerPosition.y > (SCREEN_HEIGHT - ((SCREEN_HEIGHT - 80) / 13 + 80)))
	{
		p1->playerPosition.y = SCREEN_HEIGHT - ((SCREEN_HEIGHT - 80) / 13 + 80);
		p1->moving = false;
	}
		

	if (p2->playerPosition.x > SCREEN_WIDTH - SCREEN_WIDTH / 15 * 2)
	{
		p2->playerPosition.x = SCREEN_WIDTH - SCREEN_WIDTH / 15 * 2;
		p2->moving = false;
	}
		
	if (p2->playerPosition.x < SCREEN_WIDTH / 15)
	{
		p2->playerPosition.x = SCREEN_WIDTH / 15;
		p2->moving = false;
	}
	if (p2->playerPosition.y < (SCREEN_HEIGHT - 80) / 13 + 80)
	{
		p2->playerPosition.y = (SCREEN_HEIGHT - 80) / 13 + 80;
		p2->moving = false;
	}
	if (p2->playerPosition.y > (SCREEN_HEIGHT - ((SCREEN_HEIGHT - 80) / 13 + 80)))
	{
		p2->playerPosition.y = SCREEN_HEIGHT - ((SCREEN_HEIGHT - 80) / 13 + 80);
		p2->moving = false;
	}

	//Blocks Collisions:
	/*for (std::list<SDL_Rect>::const_iterator it = blockList.cbegin(); it != blockList.cend(); ++it)//CANVIAR PER GRID[][]//*******************************************************************************************************
	{
		if (isCollisioning(p1->playerPosition, *it))
		{
			if (keyDown == SDLK_w)
			{
				
					p1->playerPosition.y += STEPS;
					p1->moving = false;
				}
			}
			else if (keyDown == SDLK_a)




			{
				p1->playerPosition.x += step;
				p1->moving = false;
			}
			else if (keyDown == SDLK_s)
			{
				p1->playerPosition.y -= step;
				p1->moving = false;
			}

			else if (keyDown == SDLK_d)
			{
				p1->playerPosition.x -= step;
				p1->moving = false;
			}
		}
		if (isCollisioning(p2->playerPosition, *it))
		{
			if (keyDown == SDLK_UP)
			{
				p2->playerPosition.y += step;
				p2->moving = false;
			}
			else if (keyDown == SDLK_LEFT)
			{
				p2->playerPosition.x += step;
				p2->moving = false;
			}
			else if (keyDown == SDLK_DOWN)
			{
				p2->playerPosition.y -= step;
				p2->moving = false;
			}
			else if (keyDown == SDLK_RIGHT)
			{
				p2->playerPosition.x -= step;
				p2->moving = false;
			}
		}
	}*/
	//Wall Collisions:
	
	/*for (std::list<Wall*>::const_iterator it = wallList.cbegin(); it != wallList.cend(); ++it)
	{
		w = *it;
		if (isCollisioning(p1->playerPosition, w->wallPosition))
		{
			if (keyDown == SDLK_w)
			{
				p1->playerPosition.y += step;
				p1->moving = false;

			}
			else if (keyDown == SDLK_a)
			{
				p1->playerPosition.x += step;
				p1->moving = false;
			}
			else if (keyDown == SDLK_s)
			{
				p1->playerPosition.y -= step;
				p1->moving = false;
			}

			else if (keyDown == SDLK_d)
			{
				p1->playerPosition.x -= step;
				p1->moving = false;
			}
		}
		if (isCollisioning(p2->playerPosition, w->wallPosition))
		{
			if (keyDown == SDLK_UP)
			{
				p2->playerPosition.y += step;
				p2->moving = false;
			}
			else if (keyDown == SDLK_LEFT)
			{
				p2->playerPosition.x += step;
				p2->moving = false;
			}
			else if (keyDown == SDLK_DOWN)
			{
				p2->playerPosition.y -= step;
				p2->moving = false;
			}
			else if (keyDown == SDLK_RIGHT)
			{
				p2->playerPosition.x -= step;
				p2->moving = false;
			}
		}
	}*/

	//Bombs:
	if (p1->ptrBomb != nullptr)
	{
		if (!p1->ptrBomb->end)
		{
			p1->ptrBomb->Update();
			if (p1->ptrBomb->explode)
			{
				checkDamage(p1);
				checkDamage(p2);
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
				checkDamage(p1);
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
}

void Level::Draw()
{
	Renderer::Instance()->Clear();

	//Background
	Renderer::Instance()->PushImage(LEVEL_BG, { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT });
	
	//Blocks://*******************************************************************************************************//*******************************************************************************************************
	for (int i = 0; i <= 10; i++)
	{
		for (int j = 0; j <= 12; j++)
		{
			if (grid[i][j]=="block")
			{
				SDL_Rect blockPosition = { static_cast<int>((SCREEN_WIDTH / 15)* (j + 1)), static_cast<int>(((SCREEN_HEIGHT - 80) / 13)* (i + 1) + 80), 48,48 };
				Renderer::Instance()->PushSprite(ITEMS, blockRect, blockPosition);
				blockList.push_back(blockPosition);
				//grid[i][j] = "block";
			}
			else if (grid[i][j] == "wall")
			{
				w = new Wall(i,j);
				w->Draw();
			}
		}
	}//*******************************************************************************************************//*******************************************************************************************************
	
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
	
	/*for (std::list<Wall*>::const_iterator it = wallList.cbegin(); it != wallList.cend(); ++it)
	{
		w = *it;
		w->Draw();
	}*/
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
	if (p->explosionLimits[0])
	{
		if (grid[p->ptrBomb->posJ][p->ptrBomb->posI - 2] == "player1")
		{
			p1->lives--;
			if(p->getPlayerTag() == 2) p2->points += 100;
			grid[p->ptrBomb->posJ][p->ptrBomb->posI - 2] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[p->ptrBomb->posJ][p->ptrBomb->posI - 2] == "player2")
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[p->ptrBomb->posJ][p->ptrBomb->posI - 2] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[p->ptrBomb->posJ][p->ptrBomb->posI - 2] == "wall")
		{
			p->points += 15;
			detectWall(p->ptrBomb->posJ, p->ptrBomb->posI - 2);
			grid[p->ptrBomb->posJ][p->ptrBomb->posI - 2] = "empty";
		}
	}
	if (p->explosionLimits[1])
	{
		if (grid[p->ptrBomb->posJ][p->ptrBomb->posI - 1] == "player1")
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[p->ptrBomb->posJ][p->ptrBomb->posI - 1] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[p->ptrBomb->posJ][p->ptrBomb->posI - 1] == "player2")
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[p->ptrBomb->posJ][p->ptrBomb->posI - 1] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[p->ptrBomb->posJ][p->ptrBomb->posI - 1] == "wall")
		{
			p->points += 15;
			detectWall(p->ptrBomb->posJ, p->ptrBomb->posI - 1);
			grid[p->ptrBomb->posJ][p->ptrBomb->posI - 1] = "empty";
		}
	}
	if (p->explosionLimits[2])
	{

		if (grid[p->ptrBomb->posJ-1][p->ptrBomb->posI] == "player1")
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[p->ptrBomb->posJ-1][p->ptrBomb->posI] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[p->ptrBomb->posJ-1][p->ptrBomb->posI] == "player2")
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[p->ptrBomb->posJ - 1][p->ptrBomb->posI] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[p->ptrBomb->posJ-1][p->ptrBomb->posI] == "wall")
		{
			p->points += 15;
			detectWall(p->ptrBomb->posJ -1, p->ptrBomb->posI);
			grid[p->ptrBomb->posJ - 1][p->ptrBomb->posI] = "empty";
		}
	}
	if (p->explosionLimits[3])
	{
		if (grid[p->ptrBomb->posJ - 2][p->ptrBomb->posI] == "player1")
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[p->ptrBomb->posJ - 2][p->ptrBomb->posI] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[p->ptrBomb->posJ -2][p->ptrBomb->posI] == "player2")
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[p->ptrBomb->posJ - 2][p->ptrBomb->posI] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[p->ptrBomb->posJ - 2][p->ptrBomb->posI] == "wall")
		{
			p->points += 15;
			detectWall(p->ptrBomb->posJ -2, p->ptrBomb->posI);
			grid[p->ptrBomb->posJ - 2][p->ptrBomb->posI] = "empty";
		}
	}
	if (p->explosionLimits[4])
	{
		if (grid[p->ptrBomb->posJ + 1][p->ptrBomb->posI] == "player1")
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[p->ptrBomb->posJ + 1][p->ptrBomb->posI] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[p->ptrBomb->posJ + 1][p->ptrBomb->posI] == "player2")
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[p->ptrBomb->posJ + 1][p->ptrBomb->posI] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[p->ptrBomb->posJ + 1][p->ptrBomb->posI] == "wall")
		{
			p->points += 15;
			detectWall(p->ptrBomb->posJ +1, p->ptrBomb->posI);
			grid[p->ptrBomb->posJ + 1][p->ptrBomb->posI] = "empty";
		}
	}
	if (p->explosionLimits[5])
	{
		if (grid[p->ptrBomb->posJ + 2][p->ptrBomb->posI] == "player1")
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[p->ptrBomb->posJ + 2][p->ptrBomb->posI] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[p->ptrBomb->posJ + 2][p->ptrBomb->posI] == "player2")
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[p->ptrBomb->posJ + 2][p->ptrBomb->posI] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[p->ptrBomb->posJ + 2][p->ptrBomb->posI] == "wall")
		{
			p->points += 15;
			detectWall(p->ptrBomb->posJ +2, p->ptrBomb->posI);
			grid[p->ptrBomb->posJ + 2][p->ptrBomb->posI] = "empty";
		}
	}
	if (p->explosionLimits[6])
	{
		if (grid[p->ptrBomb->posJ][p->ptrBomb->posI + 1] == "player1")
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[p->ptrBomb->posJ][p->ptrBomb->posI + 1] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[p->ptrBomb->posJ][p->ptrBomb->posI + 1] == "player2")
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[p->ptrBomb->posJ][p->ptrBomb->posI - 1] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[p->ptrBomb->posJ][p->ptrBomb->posI + 1] == "wall")
		{
			p->points += 15;
			detectWall(p->ptrBomb->posJ, p->ptrBomb->posI + 1);
			grid[p->ptrBomb->posJ][p->ptrBomb->posI + 1] = "empty";
		}
	}
	if (p->explosionLimits[7])
	{
		if (grid[p->ptrBomb->posJ][p->ptrBomb->posI + 2] == "player1")
		{
			p1->lives--;
			if (p->getPlayerTag() == 2) p2->points += 100;
			grid[p->ptrBomb->posJ][p->ptrBomb->posI + 2] = "empty";
			changePlayerLocation(p1);
		}
		else if (grid[p->ptrBomb->posJ][p->ptrBomb->posI - 2] == "player2")
		{
			p2->lives--;
			if (p->getPlayerTag() == 1) p1->points += 100;
			grid[p->ptrBomb->posJ][p->ptrBomb->posI + 2] = "empty";
			changePlayerLocation(p2);
		}
		else if (grid[p->ptrBomb->posJ][p->ptrBomb->posI + 2] == "wall")
		{
			p->points += 15;
			detectWall(p->ptrBomb->posJ, p->ptrBomb->posI + 2);
			grid[p->ptrBomb->posJ][p->ptrBomb->posI + 2] = "empty";
		}
	}	
}

void Level::changePlayerLocation(Player *p) //faltaria veure què passaria si no hi hagués cap espai.
{
	bool done = false;
	while (!done)
	{
		int i= rand() % 13;
		int j = rand() % 11;
		if (grid[i][j] == "empty")
		{
			grid[i][j] = "player" + std::to_string(p->getPlayerTag());
			p->posI = i;
			p->posJ = j;
			p->playerPosition.x = SCREEN_WIDTH - SCREEN_WIDTH / 15 * i;
			p->playerPosition.y = (SCREEN_HEIGHT - 80) / 13 * j + 80;
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
		winner w;
		bool introduced=false;

		std::ifstream fentrada(rankingFile, std::ios::in | std::ios::binary);
		fentrada.read(reinterpret_cast<char *>(&n), sizeof(n));
		std::cout << "number of winners: " << n << std::endl;

		std::vector<winner> rankingVector;

		for (int i = 0; i < n; i++)
		{
			fentrada.read(reinterpret_cast<char *>(&w), sizeof(w));
			if (!introduced && p->points > w.points)
			{
				winner newWinner;
				std::cout << "Introduce your name: \n";
				std::cin >> newWinner.name;
				newWinner.points = p->points;
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
			std::ofstream fsalida(rankingFile, std::ios::out | std::ios::binary);
			fsalida.write(reinterpret_cast<char *>(&n), sizeof(n));
			for (int i = 0; i < n; i++)
			{
				w = rankingVector[i];
				fsalida.write(reinterpret_cast<char *>(&w), sizeof(w));
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
		fsalida.write(reinterpret_cast<char *>(&n), sizeof(n));
		fsalida.write(reinterpret_cast<char *>(&w), sizeof(w));
	}
}