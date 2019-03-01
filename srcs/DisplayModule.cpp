#include "DisplayModule.hpp"
#include "ResourceManager.hpp"
#include "SDLException.hpp"

# define FONT_FILE				"fonts/sample_font.ttf"
# define FONT_SIZE				30

# define SINE_WAVE_IMAGE		"images/sine.png"
# define Triangle_WAVE_IMAGE	"images/triangle.png"
# define SQUARE_WAVE_IMAGE		"images/square.png"
# define SAWTOOTH_WAVE_IMAGE	"images/sawtooth.png"

# define SCREEN_WIDTH			800
# define SCREEN_HEIGHT			600

DisplayModule::DisplayModule(Waveform waveform)
{
	const int		IMG_Flags = IMG_INIT_JPG | IMG_INIT_PNG;
	
	if ((IMG_Init(IMG_Flags) & IMG_Flags) != IMG_Flags)
		throw SDLException("IMG_Init() failed");
	if (TTF_Init())
		throw SDLException("TTF_Init() failed");

	if (!(this->_window = SDL_CreateWindow("MiniSynth", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN)))
		throw SDLException("SDL_CreateWindow() failed");

	if (!(this->_renderer = SDL_CreateRenderer(this->_window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
		throw SDLException("SDL_CreateRenderer() failed");

	if (!(this->_font = TTF_OpenFont(ResourceManager::getInstance().getAbsolutePathname(FONT_FILE).c_str(), FONT_SIZE)))
		throw SDLException("TTF_OpenFont() failed");

	this->_sineWaveTexture = this->_loadTexture(SINE_WAVE_IMAGE);
	this->_triangleWaveTexture = this->_loadTexture(Triangle_WAVE_IMAGE);
	this->_squareWaveTexture = this->_loadTexture(SQUARE_WAVE_IMAGE);
	this->_sawtoothWaveTexture = this->_loadTexture(SAWTOOTH_WAVE_IMAGE);

	this->displayWaveform(waveform);
}

SDL_Texture *		DisplayModule::_loadTexture(std::string filename)
{
	SDL_Texture *	texture = IMG_LoadTexture(this->_renderer,
		ResourceManager::getInstance().getAbsolutePathname(filename).c_str());
	if (texture == nullptr)
		throw SDLException("Failed to load texture: " + filename);
	return texture;
}

DisplayModule::~DisplayModule(void)
{
	TTF_CloseFont(this->_font);
	SDL_DestroyRenderer(this->_renderer);
	SDL_DestroyWindow(this->_window);

	SDL_DestroyTexture(this->_sineWaveTexture);
	SDL_DestroyTexture(this->_triangleWaveTexture);
	SDL_DestroyTexture(this->_squareWaveTexture);
	SDL_DestroyTexture(this->_sawtoothWaveTexture);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void				DisplayModule::displayWaveform(Waveform waveform)
{
	switch (waveform)
	{
		case SINE_WAVE:
			this->_currentWaveTexture = this->_sineWaveTexture;
			break;
		case TRIANGLE_WAVE:
			this->_currentWaveTexture = this->_triangleWaveTexture;
			break;
		case SQUARE_WAVE:
			this->_currentWaveTexture = this->_squareWaveTexture;
			break;
		case SAWTOOTH_WAVE:
			this->_currentWaveTexture = this->_sawtoothWaveTexture;
			break;
	}
	this->_render();
}

void				DisplayModule::_render(void)
{
	SDL_SetRenderDrawColor(this->_renderer, 255, 255, 255, 0);
	SDL_RenderClear(this->_renderer);
	SDL_RenderCopy(this->_renderer, this->_currentWaveTexture, nullptr, nullptr);
	SDL_RenderPresent(this->_renderer);
}
