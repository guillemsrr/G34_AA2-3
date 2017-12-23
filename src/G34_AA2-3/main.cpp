#include <iostream>
#include "Game.h"

#undef main

int main(int, char*[])
{
	try
	{
		Game Bomberman;
		Bomberman.run();
	}
	catch (const std::exception & err)
	{
		std::cout << err.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Error detected." << std::endl;
	}
}