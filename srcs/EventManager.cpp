#include "EventManager.hpp"
#include "MiniSynth.hpp"
#include "SDLException.hpp"
#include "frequency.hpp"
#include "waveform.hpp"

EventManager::EventManager(MiniSynth & miniSynth) : _miniSynth(miniSynth)
{
	if (!(this->_keyState = SDL_GetKeyboardState(nullptr)))
		throw SDLException("SDL_GetKeyboardState() failed");
}

EventManager::~EventManager(void)
{
	/* Do NOT free this->_keyState */
}

void		EventManager::handleEvents(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			this->_miniSynth.exit();
		else if (event.type == SDL_KEYDOWN)
			this->_handleKeyDownEvent(event);
		else if (event.type == SDL_KEYUP)
			this->_handleKeyUpEvent(event);
	}
}

/* Handle single press key down events */
void		EventManager::_handleKeyDownEvent(SDL_Event & event)
{
	switch (event.key.keysym.sym)
	{
		case SDLK_ESCAPE:
			this->_miniSynth.exit();
			return;
		case SDLK_1:
			this->_miniSynth.setWaveform(SINE_WAVE);
			break;
		case SDLK_2:
			this->_miniSynth.setWaveform(TRIANGLE_WAVE);
			break;
		case SDLK_3:
			this->_miniSynth.setWaveform(SQUARE_WAVE);
			break;
		case SDLK_4:
			this->_miniSynth.setWaveform(SAWTOOTH_WAVE);
			break;
		case SDLK_SPACE:
			this->_miniSynth.playSong();
			break;
		/* white keys */
		case SDLK_a:
			this->_miniSynth.playNote(FREQ_A_2);
			break;
		case SDLK_s:
			this->_miniSynth.playNote(FREQ_B_2);
			break;
		case SDLK_d:
			this->_miniSynth.playNote(FREQ_C_3);
			break;
		case SDLK_f:
			this->_miniSynth.playNote(FREQ_D_3);
			break;
		case SDLK_g:
			this->_miniSynth.playNote(FREQ_E_3);
			break;
		case SDLK_h:
			this->_miniSynth.playNote(FREQ_F_3);
			break;
		case SDLK_j:
			this->_miniSynth.playNote(FREQ_G_3);
			break;
		case SDLK_k:
			this->_miniSynth.playNote(FREQ_A_3);
			break;
		case SDLK_l:
			this->_miniSynth.playNote(FREQ_B_3);
			break;
		case SDLK_SEMICOLON:
			this->_miniSynth.playNote(FREQ_C_4);
			break;
		case SDLK_QUOTE:
			this->_miniSynth.playNote(FREQ_D_4);
			break;
		/* black keys */
		case SDLK_q:
			this->_miniSynth.playNote(FREQ_G_SHARP_2);
			break;
		case SDLK_w:
			this->_miniSynth.playNote(FREQ_A_SHARP_2);
			break;
		case SDLK_r:
			this->_miniSynth.playNote(FREQ_C_SHARP_3);
			break;
		case SDLK_t:
			this->_miniSynth.playNote(FREQ_D_SHARP_3);
			break;
		case SDLK_u:
			this->_miniSynth.playNote(FREQ_F_SHARP_3);
			break;
		case SDLK_i:
			this->_miniSynth.playNote(FREQ_G_SHARP_3);
			break;
		case SDLK_o:
			this->_miniSynth.playNote(FREQ_A_SHARP_3);
			break;
		case SDLK_LEFTBRACKET:
			this->_miniSynth.playNote(FREQ_C_SHARP_4);
			break;
		default:
			return;
	}
}

void		EventManager::_handleKeyUpEvent(SDL_Event & event)
{
	switch (event.key.keysym.sym)
	{
		/* white keys */
		case SDLK_a:
			this->_miniSynth.releaseNote(FREQ_A_2);
			break;
		case SDLK_s:
			this->_miniSynth.releaseNote(FREQ_B_2);
			break;
		case SDLK_d:
			this->_miniSynth.releaseNote(FREQ_C_3);
			break;
		case SDLK_f:
			this->_miniSynth.releaseNote(FREQ_D_3);
			break;
		case SDLK_g:
			this->_miniSynth.releaseNote(FREQ_E_3);
			break;
		case SDLK_h:
			this->_miniSynth.releaseNote(FREQ_F_3);
			break;
		case SDLK_j:
			this->_miniSynth.releaseNote(FREQ_G_3);
			break;
		case SDLK_k:
			this->_miniSynth.releaseNote(FREQ_A_3);
			break;
		case SDLK_l:
			this->_miniSynth.releaseNote(FREQ_B_3);
			break;
		case SDLK_SEMICOLON:
			this->_miniSynth.releaseNote(FREQ_C_4);
			break;
		case SDLK_QUOTE:
			this->_miniSynth.releaseNote(FREQ_D_4);
			break;
		/* black keys */
		case SDLK_q:
			this->_miniSynth.releaseNote(FREQ_G_SHARP_2);
			break;
		case SDLK_w:
			this->_miniSynth.releaseNote(FREQ_A_SHARP_2);
			break;
		case SDLK_r:
			this->_miniSynth.releaseNote(FREQ_C_SHARP_3);
			break;
		case SDLK_t:
			this->_miniSynth.releaseNote(FREQ_D_SHARP_3);
			break;
		case SDLK_u:
			this->_miniSynth.releaseNote(FREQ_F_SHARP_3);
			break;
		case SDLK_i:
			this->_miniSynth.releaseNote(FREQ_G_SHARP_3);
			break;
		case SDLK_o:
			this->_miniSynth.releaseNote(FREQ_A_SHARP_3);
			break;
		case SDLK_LEFTBRACKET:
			this->_miniSynth.releaseNote(FREQ_C_SHARP_4);
			break;
		default:
			return;
	}
}

/* Handle simultaneous, continuously held key events */
// void		EventManager::_handleKeyHoldEvents(void)
// {
// 	// if (this->_keyState[SDL_SCANCODE_A])
// 	// {
// 	// 	this->_miniSynth.playNote(FREQ_A2);
// 	// }
// }
