#pragma once

#include <memory>
#include <vector>

#include <wx/thread.h>

#include "../ext/midifile/include/MidiFile.h"
#include "../ext/rtmidi/RtMidi.h"

#if _WIN32
    #include <Windows.h>
    #define ISLEEP(ms) do {Sleep(ms);} while (false)
#elif __linux__
    #include <time.h>
    #define ISLEEP(ms) ({const struct timespec ts {0, (ms)*1000000}; nanosleep(&ts, NULL);})
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
  void SetTickOffset(int tick);

private:
  std::unique_ptr<MidiPlayer> mp;
  smf::MidiFile *copied_mf;
  int tick_offset;

  virtual wxThread::ExitCode Entry();
};
} // namespace cms

inline void
cms::MidiPlayer::SendMessage(const std::vector<unsigned char> &message) {
  midiout->sendMessage(&message);
}