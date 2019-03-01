#include "NoteManager.hpp"

NoteManager::NoteManager(void) { }

NoteManager::~NoteManager(void) { }

void		NoteManager::playNote(double frequency)
{
	if (this->_currentNotes.size() < MAX_NUMBER_OF_NOTES &&
		std::find(this->_currentNotes.begin(), this->_currentNotes.end(), frequency) == this->_currentNotes.end())
	{
		this->_currentNotes.push_back(frequency);
	}
}

void		NoteManager::releaseNote(double frequency)
{
	this->_currentNotes.erase(
		std::remove(this->_currentNotes.begin(), this->_currentNotes.end(), frequency),
		this->_currentNotes.end());
}

const std::vector<double> &	NoteManager::getNotes(void) const
{
	return (this->_currentNotes);
}
