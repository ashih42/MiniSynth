#include "ResourceManager.hpp"
#include "SDLException.hpp"

ResourceManager::ResourceManager(void)
{
	char * basePath = SDL_GetBasePath();		// does NOT depend on SDL_Init()

	if (basePath)
	{
		this->_basePath = std::string(basePath);
		SDL_free(basePath);
	}
	else
		throw SDLException("SDL_GetBasePath() failed");
}

ResourceManager::~ResourceManager(void) { }

ResourceManager &	ResourceManager::getInstance(void)
{
	static ResourceManager instance;

	return (instance);
}

std::string		ResourceManager::getAbsolutePathname(std::string filename)
{
	return (this->_basePath + filename);
}
