#include "./midi_player.hpp"

#include <vector>
#include <string>

#if _WIN32
	#include <Windows.h>
	#define ISLEEP(ms) Sleep(ms);
#endif

#undef SendMessage

#include "../ext/midifile/include/MidiFile.h"

using namespace cms;

int main()
{
	MidiPlayer *mp = new MidiPlayer();
	mp->OpenPorts();
	std::vector<std::string> out;
	std::vector<std::string> in;
	mp->GetPortInfo(in, out);
	for (auto it=in.begin(); it!=in.end(); it++) {
		std::cout << "In: " << *it << std::endl;
	}
	for (auto it=out.begin(); it!=out.end(); it++) {
		std::cout << "Out: " << *it << std::endl;
	}
	mp->SetOutputPort(0);

	// Read midifile //
	smf::MidiFile midifile;
	midifile.read("cola-kisu.mid");
	midifile.doTimeAnalysis();
	midifile.joinTracks();

	for (int e=0; e<midifile[0].size(); e++) {
		smf::MidiEvent *mev = &midifile[0][e];
		double delta_real;
		if (e == 0)
			delta_real = mev->seconds;
		else
			delta_real = mev->seconds - midifile[0][e-1].seconds;

		ISLEEP(delta_real * 1000);
		std::vector<unsigned char> vec;
		for (int i=0; i<mev->size(); i++) {
			vec.push_back((unsigned char) (*mev)[i]);
		}
		mp->SendMessage(vec);
	}
	printf("\n");

	delete mp;
	return 0;
}