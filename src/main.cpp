#include <iostream>

#include "Game.h"

int main(int argc, char **argv)
{
	try
	{
		printf("Program Start!\n");
		Game game;
		bool success = game.Initialize();
		printf("Loading Done!\n");
		if (success)
		{
			game.RunLoop();
		}
		game.Shutdown();
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}