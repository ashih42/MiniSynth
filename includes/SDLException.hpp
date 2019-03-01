#ifndef SDL_EXCEPTION_HPP
# define SDL_EXCEPTION_HPP

# include <string>

class SDLException : public std::exception
{
public:

	SDLException(std::string errorMessage);
	~SDLException(void) _NOEXCEPT;

	SDLException(const SDLException & src);
	SDLException & operator=(const SDLException & rhs);

private:

	std::string		_errorMessage;

	SDLException(void);

};

#endif
