#include "AudioModule.hpp"
#include "SDLException.hpp"

#define DURATION_QUARTER_NOTE	400
#define DURATION_EIGHTH_NOTE	(DURATION_QUARTER_NOTE / 2)
#define DURATION_SIXTEENTH_NOTE	(DURATION_QUARTER_NOTE / 4)

#define AMPLITUDE				(28000 / 16)
#define SAMPLE_RATE				44100
#define BUFFER_SIZE				(4096 / 8)

void				audioCallback(void * userdata, Uint8 * stream, int len)
{
	AudioModule *	audioModule = static_cast<AudioModule *>(userdata);
	Sint16 *		buffer = reinterpret_cast<Sint16 *>(stream);
	
	len = len / 2;	// 2 bytes per sample for AUDIO_S16SYS

	for (int i = 0; i < len; i++, audioModule->getSampleNumber()++)
	{
		double time = static_cast<double>(audioModule->getSampleNumber()) / SAMPLE_RATE;
		buffer[i] = 0;
		buffer[i] += (Sint16)audioModule->getWaveformValue(time, audioModule->getTrebleFrequency(), AMPLITUDE);
		buffer[i] += (Sint16)audioModule->getWaveformValue(time, audioModule->getBassFrequency(), AMPLITUDE);

		for (double frequency : audioModule->getNotes())
			buffer[i] += (Sint16)audioModule->getWaveformValue(time, frequency, AMPLITUDE);			
	}
}

double		getNoiseValue(double amplitude)
{
	return (amplitude * ((double) rand() / (RAND_MAX)));
}

double		getSineWaveValue(double time, double frequency, double amplitude)
{
	return (amplitude * sin(2.0 * M_PI * frequency * time));
}

double		getSquareWaveValue(double time, double frequency, double amplitude)
{
	double	period = 1.0 / frequency;
	return (fmod(time, period) / period > 0.5 ? amplitude : -amplitude);
}

double		getTriangleWaveValue(double time, double frequency, double amplitude)
{
	double	period = 1.0 / frequency;
	return ((abs(fmod(time, period) / period - 0.5) - 0.25) * 4.0 * amplitude);
}

double		getSawtoothWaveValue(double time, double frequency, double amplitude)
{
	double	period = 1.0 / frequency;
	return ((fmod(time, period) / period - 0.5) * 2.0 * amplitude);
}

AudioModule::AudioModule(Waveform waveform) : _isPlayingSong(false), _sampleNumber(0), _trebleFrequency(0.0), _bassFrequency(0.0)
{
	if (SDL_Init(SDL_INIT_AUDIO))
		throw SDLException("SDL_Init() failed");

	SDL_AudioSpec audioSettings = {};

	audioSettings.freq = SAMPLE_RATE;
	audioSettings.format = AUDIO_S16SYS;
	audioSettings.channels = 2;
	audioSettings.samples = BUFFER_SIZE;
	audioSettings.callback = &audioCallback;
	audioSettings.userdata = this;

	SDL_OpenAudio(&audioSettings, nullptr);
	if (audioSettings.format != AUDIO_S16SYS)
		throw SDLException("audioSettings.format does not match");

	SDL_PauseAudio(0);		// unpause; aka play forever
	this->setWaveformFunction(waveform);
}

AudioModule::~AudioModule(void)
{
	SDL_CloseAudio();
}

void		AudioModule::playNote(double frequency)
{
	this->_noteManager.playNote(frequency);
}

void		AudioModule::releaseNote(double frequency)
{
	this->_noteManager.releaseNote(frequency);
}

const std::vector<double> &		AudioModule::getNotes(void)
{
	return (this->_noteManager.getNotes());
}


bool		AudioModule::isPlayingSong(void) const
{
	return (this->_isPlayingSong);
}

int	&		AudioModule::getSampleNumber(void)
{
	return (this->_sampleNumber);
}

double		AudioModule::getTrebleFrequency(void) const
{
	return (this->_trebleFrequency);
}

double		AudioModule::getBassFrequency(void) const
{
	return (this->_bassFrequency);
}

double		AudioModule::getWaveformValue(double time, double frequency, double amplitude)
{
	return (this->_waveformFunction(time, frequency, amplitude));
}

void		AudioModule::_setTreble(double frequency)
{
	this->_trebleFrequency = frequency;
}

void		AudioModule::_setBass(double frequency)
{
	this->_bassFrequency = frequency;
}

void		AudioModule::setWaveformFunction(Waveform waveform)
{
	switch (waveform)
	{
		case SINE_WAVE:
			this->_waveformFunction = &getSineWaveValue;
			break;
		case SQUARE_WAVE:
			this->_waveformFunction = &getSquareWaveValue;
			break;
		case TRIANGLE_WAVE:
			this->_waveformFunction = &getTriangleWaveValue;
			break;
		case SAWTOOTH_WAVE:
			this->_waveformFunction = &getSawtoothWaveValue;
			break;
	}
}

void		AudioModule::playSong(void)
{
	this->_isPlayingSong = true;

	this->_setTreble(FREQ_A_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// A ------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// C -------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------

	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// G -------------------------------------

	this->_setTreble(0);
	this->_setBass(FREQ_G_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_B_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------

	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_G_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_B_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// F -------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_F_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// F G -------------------------------------

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_F_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_F_4);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_F_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_G_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------
	// A -------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// C -------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------

	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// G -------------------------------------

	this->_setTreble(0);
	this->_setBass(FREQ_G_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_B_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------

	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_G_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_B_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// F -------------------------------------

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_F_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// F G -------------------------------------

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_F_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_F_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_G_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// --------------------------------------
	// A ------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// C -------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------

	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// G -------------------------------------

	this->_setTreble(0);
	this->_setBass(FREQ_G_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_B_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------

	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_G_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_B_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// F -------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_F_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// F G -------------------------------------

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_F_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_F_4);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_F_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_G_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------
	// A -------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// C -------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------

	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	// this->_setTreble(0);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_E_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_G_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// G -------------------------------------

	this->_setTreble(0);
	this->_setBass(FREQ_G_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_B_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------

	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_G_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_B_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// F -------------------------------------

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_F_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_D_4);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setTreble(FREQ_C_4);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// F G -------------------------------------

	this->_setTreble(FREQ_E_4);
	this->_setBass(FREQ_F_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_G_SHARP_3);
	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(0);
	this->_setBass(FREQ_F_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);
	
	this->_setTreble(FREQ_A_3);
	this->_setBass(FREQ_G_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	// ---------------------------------------
	// A -------------------------------------

	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setBass(FREQ_A_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setBass(FREQ_A_3);
	SDL_Delay(DURATION_SIXTEENTH_NOTE);
	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_SIXTEENTH_NOTE);

	// ---------------------------------------

	this->_setBass(FREQ_C_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	SDL_Delay(DURATION_EIGHTH_NOTE);

	this->_setBass(FREQ_D_3);
	SDL_Delay(DURATION_EIGHTH_NOTE);

	SDL_Delay(DURATION_EIGHTH_NOTE);

	// A ------------------------------------

	this->_setBass(FREQ_A_2);
	SDL_Delay(DURATION_QUARTER_NOTE);
	SDL_Delay(DURATION_QUARTER_NOTE);
	SDL_Delay(DURATION_QUARTER_NOTE);
	SDL_Delay(DURATION_QUARTER_NOTE);

	// ---------------------------------------

	this->_setTreble(0);
	this->_setBass(0);

	this->_isPlayingSong = false;
}
