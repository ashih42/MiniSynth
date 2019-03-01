#ifndef EVENT_MANAGER_HPP
# define EVENT_MANAGER_HPP

# include <SDL.h>

class MiniSynth;

class EventManager
{
public:

	EventManager(MiniSynth & miniSynth);
	~EventManager(void);

	void			handleEvents(void);

private:

	MiniSynth &			_miniSynth;
	const Uint8 *		_keyState;

	EventManager(void);
	EventManager(const EventManager & src);
	EventManager & operator=(const EventManager & rhs);
	
	void			_handleKeyDownEvent(SDL_Event & event);
	void			_handleKeyUpEvent(SDL_Event & event);
	// void			_handleKeyHoldEvents(void);

};

#endif
