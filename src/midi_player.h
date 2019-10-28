#pragma once

#include <vector>

#include "../ext/rtmidi/RtMidi.h"


namespace cms
{
	class MidiPlayer
	{
	public:
		MidiPlayer();
		virtual ~MidiPlayer();

		/*! Throws RtMidiError on error */
		void OpenPorts();
		void GetPortInfo(std::vector<std::string>& in, std::vector<std::string>& out) const;

		void SetOutputPort(unsigned int portNum);

		void SendMessage(const std::vector<unsigned char>& message);

	private:
		RtMidiIn *midiin;
		RtMidiOut *midiout;
	};
}

inline void cms::MidiPlayer::SendMessage(const std::vector<unsigned char>& message)
{
	midiout->sendMessage(&message);
}