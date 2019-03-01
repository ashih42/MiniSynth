#ifndef DISPLAY_MODULE_HPP
# define DISPLAY_MODULE_HPP

# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include <string>

# include "waveform.hpp"

class DisplayModule
{
public:

	DisplayModule(Waveform waveform);
	~DisplayModule(void);

	void						displayWaveform(Waveform waveform);

private:

	SDL_Window *				_window;
	SDL_Renderer *				_renderer;

	TTF_Font *					_font;

	SDL_Texture *				_sineWaveTexture;
	SDL_Texture *				_triangleWaveTexture;
	SDL_Texture *				_squareWaveTexture;
	SDL_Texture *				_sawtoothWaveTexture;

	SDL_Texture *				_currentWaveTexture;

	DisplayModule(void);
	DisplayModule(const DisplayModule & src);
	DisplayModule & operator=(const DisplayModule & rhs);

	SDL_Texture *				_loadTexture(std::string filename);
	void						_render(void);
	
};

#endif
