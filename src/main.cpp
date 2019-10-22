#include <wx/wxprec.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "./pianoroll_widget.h"

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

wxDECLARE_EVENT_TABLE();
};


enum
{
	ID_File = 1
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(ID_File, MainFrame::OnFile)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(CrossMidiStudio);

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
	menuFile->Append(ID_File, "&File...\tCtrl-H", "Open a file");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu *menuHelp = new wxMenu();
	menuHelp->Append(wxID_ABOUT);

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);

	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");
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
	PianorollWidget *piano = new PianorollWidget(this, wxID_ANY, wxPoint(50, 50), wxSize(600, 700), "Pianoroll");
	piano->Show(true);
}