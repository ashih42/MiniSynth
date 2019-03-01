#include "AudioModule.hpp"
#include "MiniSynth.hpp"
#include "SDLException.hpp"
#include <iostream>

int			main(void)
{
	try
	{
		MiniSynth miniSynth;
	}
	catch (SDLException & exception)
	{
		std::cout << "SDLException: " << exception.what() << std::endl;
	}
	return (EXIT_SUCCESS);
}
