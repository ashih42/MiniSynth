#ifndef AUDIO_MODULE_HPP
# define AUDIO_MODULE_HPP

# include <SDL.h>
# include <SDL_audio.h>
# include <cmath>
# include <iostream>
# include "waveform.hpp"
# include "frequency.hpp"
# include "NoteManager.hpp"

void					audioCallback(void * userdata, Uint8 * stream, int len);

double					getNoiseValue(double amplitude);

double					getSineWaveValue(double time, double frequency, double amplitude);
double					getSquareWaveValue(double time, double frequency, double amplitude);
double					getTriangleWaveValue(double time, double frequency, double amplitude);
double					getSawtoothWaveValue(double time, double frequency, double amplitude);

typedef double (*WaveformFunction)(double, double, double);

class AudioModule
{
public:

	AudioModule(Waveform waveform);
	~AudioModule(void);

	void				playNote(double frequency);
	void				releaseNote(double frequency);
	const std::vector<double> &		getNotes(void);

	void				setWaveformFunction(Waveform waveform);

	void				playSong(void);
	bool				isPlayingSong(void) const;

	/* Needed for audioCallback */
	int	&				getSampleNumber(void);
	double				getTrebleFrequency(void) const;
	double				getBassFrequency(void) const;
	double				getWaveformValue(double time, double frequency, double amplitude);

private:

	bool				_isPlayingSong;
	int					_sampleNumber;
	double				_trebleFrequency;
	double				_bassFrequency;

	WaveformFunction	_waveformFunction;
	NoteManager			_noteManager;

	AudioModule(void);
	AudioModule(const AudioModule & src);
	AudioModule & operator=(const AudioModule & rhs);

	void				_setTreble(double frequency);
	void				_setBass(double frequency);

};

#endif
