#include "./midi_player.h"

using namespace cms;

MidiPlayer::MidiPlayer() {}

MidiPlayer::~MidiPlayer() {
  if (midiin != nullptr) {
    delete midiin;
  }
  if (midiout != nullptr) {
    delete midiout;
  }
}

void MidiPlayer::OpenPorts() {
  midiin = new RtMidiIn();
  midiout = new RtMidiOut();
}

void MidiPlayer::GetPortInfo(std::vector<std::string> &in,
                             std::vector<std::string> &out) const {
  if (midiin != nullptr) {
    int nPorts = midiin->getPortCount();
    for (int i = 0; i < nPorts; i++) {
      in.push_back(midiin->getPortName(i));
    }
  }
  if (midiout != nullptr) {
    int nPorts = midiout->getPortCount();
    for (int i = 0; i < nPorts; i++) {
      out.push_back(midiout->getPortName(i));
    }
  }
}

void MidiPlayer::SetOutputPort(unsigned int portNum) {
  midiout->openPort(portNum);
}

BackgroundMidiPlayer::BackgroundMidiPlayer(MidiPlayer *mp) {
  this->mp.reset(std::move(mp));
}

BackgroundMidiPlayer::~BackgroundMidiPlayer() {}

void BackgroundMidiPlayer::SetMidiFile(const smf::MidiFile &mf) {
  this->copied_mf = new smf::MidiFile(mf);
  copied_mf->doTimeAnalysis();
  copied_mf->joinTracks();
}

wxThread::ExitCode BackgroundMidiPlayer::Entry() {
  for (int e = 0; e < (*copied_mf)[0].size(); e++) {
    if (TestDestroy())
      break;
    smf::MidiEvent *mev = &(*copied_mf)[0][e];
    double delta_real;
    if (e == 0)
      delta_real = mev->seconds;
    else
      delta_real = mev->seconds - (*copied_mf)[0][e - 1].seconds;

    ISLEEP(delta_real * 1000);
    std::vector<unsigned char> vec;
    for (int i = 0; i < mev->size(); i++) {
      vec.push_back((unsigned char)(*mev)[i]);
    }
    mp->SendMessage(vec);
  }

  if (copied_mf != nullptr)
    delete copied_mf;
  return 0;
}