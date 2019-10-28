#include "./midi_player.h"

using namespace cms;

MidiPlayer::MidiPlayer()
{
	
}

MidiPlayer::~MidiPlayer()
{
	if (midiin != nullptr) {
		delete midiin;
	}
	if (midiout != nullptr) {
		delete midiout;
	}
}

void MidiPlayer::OpenPorts()
{
	midiin = new RtMidiIn();
	midiout = new RtMidiOut();
}

void MidiPlayer::GetPortInfo(std::vector<std::string>& in, std::vector<std::string>& out) const
{
	if (midiin != nullptr)
	{
		int nPorts = midiin->getPortCount();
		for (int i=0; i<nPorts; i++) {
			in.push_back(midiin->getPortName(i));
		}
	}
	if (midiout != nullptr)
	{
		int nPorts = midiout->getPortCount();
		for (int i=0; i<nPorts; i++) {
			out.push_back(midiout->getPortName(i));
		}
	}
}

void MidiPlayer::SetOutputPort(unsigned int portNum)
{
	midiout->openPort(portNum);
}
