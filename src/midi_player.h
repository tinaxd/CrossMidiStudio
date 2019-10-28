#pragma once

#include <memory>
#include <vector>

#include <wx/thread.h>

#include "../ext/midifile/include/MidiFile.h"
#include "../ext/rtmidi/RtMidi.h"

#if _WIN32
#include <Windows.h>
#define ISLEEP(ms) Sleep(ms);
#endif

namespace cms {
class MidiPlayer {
public:
  MidiPlayer();
  virtual ~MidiPlayer();

  /*! Throws RtMidiError on error */
  void OpenPorts();
  void GetPortInfo(std::vector<std::string> &in,
                   std::vector<std::string> &out) const;

  void SetOutputPort(unsigned int portNum);

  void SendMessage(const std::vector<unsigned char> &message);

private:
  RtMidiIn *midiin;
  RtMidiOut *midiout;
};

class BackgroundMidiPlayer : public wxThread {
public:
  BackgroundMidiPlayer(MidiPlayer *mp);
  virtual ~BackgroundMidiPlayer();

  void SetMidiFile(const smf::MidiFile &mf);

private:
  std::unique_ptr<MidiPlayer> mp;
  smf::MidiFile *copied_mf;

  virtual wxThread::ExitCode Entry();
};
} // namespace cms

inline void
cms::MidiPlayer::SendMessage(const std::vector<unsigned char> &message) {
  midiout->sendMessage(&message);
}