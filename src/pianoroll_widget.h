#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/splitter.h>

#include <queue>
#include <vector>
#include <stack>
#include <array>

#include "../ext/midifile/include/MidiEventList.h"



namespace cms
{
	struct MidiNote
	{
	  int note;
	  int length;
	  int start;
	};

	struct DrawCord
	{
	  double x;
	  double y;
	  double length;
	};

	struct AutomationPoint
	{
		int value;
		int tick;
	};

	auto drawcord_comp = [](DrawCord x, DrawCord y) {
	            return x.x > y.x;
	};

	// Declaration
	class PianorollWidget;
	class PianorollCanvas;
	class ControlChangeGraph;

	class PianorollCanvas : public wxScrolledCanvas
	{
	public:
		PianorollCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const wxString &name);
		virtual ~PianorollCanvas();

		void GetScrollOffset(int& x_start, int& y_start);

		void SetControlChangeGraph(ControlChangeGraph& ccGraph);

		double white_width = 60;
		double white_height = 30;
		double black_width = 25;
		double black_height = 20;
		double pianoroll_leftmargin = 5;
		double pianoroll_upmargin = 25;
		double max_width = 2000;

		int measure_division = 4;
		double measure_width = 300;
		int resolution = 480;

		double key_heights[127];

		std::vector<MidiNote> notes;
		void CalculateNotePositions();

	private:
		void OnPaint(wxPaintEvent &event);
		void OnLeftClick(wxMouseEvent &event);
		void OnScroll(wxScrollWinEvent &event);

		wxDECLARE_EVENT_TABLE();

	protected:
		ControlChangeGraph *ccGraph;
		/*
		virtual void DrawKeyboard(wxGraphicsContext &gc);
		virtual void DrawTimeline(wxGraphicsContext &gc);
		virtual void DrawGridhelper(wxGraphicsContext &gc);
		*/
		virtual void DrawKeyboard(wxDC &dc);
		virtual void DrawTimeline(wxDC &dc);
		virtual void DrawGridhelper(wxDC &dc);
		virtual void DrawAllNotes(wxDC &dc);

		std::queue<DrawCord> note_drawed;

	};

	class ControlChangeGraph : public wxWindow
	{
	public:
		ControlChangeGraph(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const wxString &name);
		virtual ~ControlChangeGraph();

		void SetPianorollCanvas(PianorollCanvas& pianorollCanvas);
		void SetHScroll(int scroll);

	private:
		int hScroll; // in pixels

		void OnLeftClick(wxMouseEvent &event);
		void OnPaint(wxPaintEvent &event);

	protected:
		PianorollCanvas* pianorollCanvas;

		virtual void DrawTimeline(wxDC &dc);

		wxDECLARE_EVENT_TABLE();
	};


	class PianorollWidget: public wxWindow
	{
	public:
		PianorollWidget(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const wxString &name);
		virtual ~PianorollWidget();

	// void set_x_scale(double scale);
	// void set_y_scale(double scale);

	// void calculate_automation_graph();

		void ResetWithNotes(smf::MidiEventList event_list);
		void AddMidiNote(smf::MidiEvent event);
		void ClearMidiNotes();

		void RequestRedrawAll();
	// void request_automation_redraw();

	private:
	// Vector<MidiEvent> midiInfo;
		PianorollCanvas *scWindow;
		ControlChangeGraph *scAutomation;

		wxBoxSizer *topsizer;
		wxSplitterWindow *topsplitter; 
		
		wxBoxSizer *automationBox;

		wxToolBar *automationToolBar;
		wxButton *automationTestButton;

	// bool automation_on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	//bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	//std::queue<DrawCord> drawing_queue;

		std::priority_queue<DrawCord, std::vector<DrawCord>, decltype(drawcord_comp)> *automation_drawed;
		std::array<std::stack<smf::MidiEvent>, 128> _note_tmp;

	// std::vector<AutomationPoint> automation_values;

	// bool draw_initialized;

	// double y_scale = 5.0;
	// double x_scale = 5.0;

	// double tempo = 120;

	// protected:
	// virtual bool on_motion_notify_event(GdkEventMotion *motion_event);
	// void draw_note(MidiNote note);
	// virtual void update_notes_drawing(const Cairo::RefPtr<Cairo::Context>& cr);
	// virtual void draw_gridhelper(const Cairo::RefPtr<Cairo::Context>& cr);

	// virtual void automation_draw_timeline(const Cairo::RefPtr<Cairo::Context>& cr);
	// virtual void automation_draw_graph(const Cairo::RefPtr<Cairo::Context>& cr);

	};
}