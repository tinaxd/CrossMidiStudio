#include <wx/wxprec.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "./pianoroll_widget.h"

#include "../ext/midifile/include/MidiFile.h"

using namespace cms;

class CrossMidiStudio : public wxApp
{
public:
	virtual bool OnInit();
};

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
	void OnFile(wxCommandEvent &event);
	void OnExit(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
	void OnRedrawRequest(wxCommandEvent &event);
	void OnTrackChange(wxCommandEvent &event);

	void ReadMidiFile(std::string filepath);
	void ResetEditViewWithTrack(int track);

	PianorollWidget *piano;

	wxComboBox *trackCombo;
	smf::MidiFile *midifile;

wxDECLARE_EVENT_TABLE();
};


enum
{
	ID_File = 1,
	ID_REDRAWALL,
	ID_TT_OpenFileButton,
	ID_TT_PlayButton,
	ID_ET_TrackCombo
};

// clang-format off
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(ID_File, MainFrame::OnFile)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_MENU(ID_REDRAWALL, MainFrame::OnRedrawRequest)
	EVT_BUTTON(ID_TT_OpenFileButton, MainFrame::OnFile)
	EVT_COMBOBOX(ID_ET_TrackCombo, MainFrame::OnTrackChange)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(CrossMidiStudio);
// clang-format on

bool CrossMidiStudio::OnInit()
{
	MainFrame *frame = new MainFrame("Cross MIDI Studio", wxPoint(50, 50), wxSize(450, 340));
	frame->Show(true);
	return true;
}

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxMenu *menuFile = new wxMenu();
	menuFile->Append(ID_File, "&Open a file...\tCtrl-H", "Open a file");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu *menuHelp = new wxMenu();
	menuHelp->Append(ID_REDRAWALL, "&Request redraw", "Redraw pianoroll");
	menuHelp->Append(wxID_ABOUT);

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);

	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");

	// Top Toolbar
	wxToolBar *topToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL, "toptoolbar");
	wxButton *openFileButton = new wxButton(topToolBar, ID_TT_OpenFileButton, "Open a midi file", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "toptool-fileopen");
	wxButton *playButton = new wxButton(topToolBar, ID_TT_PlayButton, "Play", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "toptool-play");

	wxToolBar *editToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL, "edittoolbar");
	trackCombo = new wxComboBox(editToolBar, ID_ET_TrackCombo, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY, wxDefaultValidator, wxComboBoxNameStr);
	trackCombo->Append("No tracks");

	topToolBar->AddTool(openFileButton);
	topToolBar->AddTool(playButton);

	editToolBar->AddControl(trackCombo);

	// MIDI Edit view
	piano = new PianorollWidget(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Pianoroll");

	// Top Sizer
	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

	topSizer->Add(topToolBar, false, wxEXPAND, 5);
	topSizer->Add(editToolBar, false, wxEXPAND, 5);
	topSizer->Add(piano, true, wxEXPAND, 5);

	SetSizerAndFit(topSizer);
}

void MainFrame::OnExit(wxCommandEvent &event)
{
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
	wxMessageBox("This is a cross-platform midi sequencer.", "About CrossMidiStudio", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnFile(wxCommandEvent &event)
{
	ReadMidiFile("./cola-kisu.mid");

	if (midifile == nullptr)
		return;

    trackCombo->Clear();
    for (int track=0; track<midifile->size(); track++) {
    	char str[9];
    	sprintf(str, "Track %d", track+1);
    	trackCombo->Append(str);
    }

    ResetEditViewWithTrack(0);
}

void MainFrame::OnRedrawRequest(wxCommandEvent &event)
{
	if (piano != nullptr)
		piano->RequestRedrawAll();
}

void MainFrame::OnTrackChange(wxCommandEvent &event)
{
	std::cout << "Track " << trackCombo->GetSelection() << " selected" << std::endl;
	ResetEditViewWithTrack(trackCombo->GetSelection());
}

void MainFrame::ReadMidiFile(std::string filepath)
{
	delete midifile;
	midifile = new smf::MidiFile();
	midifile->read(filepath);
	midifile->doTimeAnalysis();
	std::cout << "Tracks: " << midifile->getTrackCount() << std::endl;
    std::cout << "TPQ: " << midifile->getTicksPerQuarterNote() << std::endl;
}

void MainFrame::ResetEditViewWithTrack(int track)
{
	piano->ClearMidiNotes();
	for (int event=0; event<(*midifile)[track].size(); event++) {
		piano->AddMidiNote((*midifile)[track][event]);
	}
	piano->RequestRedrawAll();
}