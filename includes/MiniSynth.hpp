#ifndef MINISYNTH_HPP
# define MINISYNTH_HPP

# include <map>

# include "AudioModule.hpp"
# include "DisplayModule.hpp"
# include "EventManager.hpp"
# include "waveform.hpp"

class MiniSynth
{
public:

	MiniSynth(void);
	~MiniSynth(void);

	void			setWaveform(Waveform waveform);

	void			playSong(void);
	void			stopSong(void);

	void			playNote(double frequency);
	void			releaseNote(double frequency);

	void			exit(void);

private:

	AudioModule *	_audioModule;
	DisplayModule *	_displayModule;
	EventManager *	_eventManager;

	bool			_exit;

	MiniSynth(const MiniSynth & src);
	MiniSynth & operator=(const MiniSynth & rhs);

};

#endif
