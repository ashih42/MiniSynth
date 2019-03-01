#include "SDLException.hpp"
#include <SDL.h>

SDLException::SDLException(std::string errorMessage)
{
	this->_errorMessage = errorMessage;
	this->_errorMessage += "\nSDL Error: ";
	this->_errorMessage += SDL_GetError();
}

SDLException::~SDLException(void) _NOEXCEPT { }

SDLException::SDLException(const SDLException & src)
{
	*this = src;
}

SDLException & SDLException::operator=(const SDLException & rhs)
{
	this->_errorMessage = rhs._errorMessage;
	return (*this);
}
