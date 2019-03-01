#ifndef NOTE_MANAGER_HPP
# define NOTE_MANAGER_HPP

# include <vector>

# define MAX_NUMBER_OF_NOTES	7

class NoteManager
{
public:

	NoteManager(void);
	~NoteManager(void);

	void					playNote(double frequency);
	void					releaseNote(double frequency);

	const std::vector<double> &	getNotes(void) const;

private:

	std::vector<double>		_currentNotes;

	NoteManager(const NoteManager & src);
	NoteManager & operator=(const NoteManager & rhs);

};

#endif
