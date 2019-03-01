#include "MiniSynth.hpp"
#include "SDLException.hpp"
#include <iostream>
#include <ctime>
#include <thread>

# define DEFAULT_WAVEFORM SINE_WAVE

MiniSynth::MiniSynth(void) : _exit(false)
{
	std::srand(std::time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING))
		throw SDLException("SDL_Init() failed");

	this->_audioModule = new AudioModule(DEFAULT_WAVEFORM);
	this->_displayModule = new DisplayModule(DEFAULT_WAVEFORM);
	this->_eventManager = new EventManager(*this);

	while (!this->_exit)
		this->_eventManager->handleEvents();
}

MiniSynth::~MiniSynth(void)
{
	delete this->_audioModule;
	delete this->_displayModule;
	delete this->_eventManager;
}

void			MiniSynth::setWaveform(Waveform waveform)
{
	this->_displayModule->displayWaveform(waveform);
	this->_audioModule->setWaveformFunction(waveform);
}

/* Create and detach a thread */
void			MiniSynth::playSong(void)
{
	if (!this->_audioModule->isPlayingSong())
	{
		std::thread th(&AudioModule::playSong, this->_audioModule);
		th.detach();
	}
}

void			MiniSynth::stopSong(void)
{
	// ¯\_(ツ)_/¯
	std::cout << "Somebody kill the thread that's playing that song pls" << std::endl;
}

void			MiniSynth::playNote(double frequency)
{
	this->_audioModule->playNote(frequency);
}

void			MiniSynth::releaseNote(double frequency)
{
	this->_audioModule->releaseNote(frequency);
}

void			MiniSynth::exit(void)
{
	this->_exit = true;
}

