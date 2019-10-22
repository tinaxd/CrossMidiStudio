#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <queue>
#include <vector>
#include <stack>
#include <array>

//#include "../subprojects/midifile/include/MidiEventList.h"


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

class PianorollCanvas : public wxScrolledCanvas
{
public:
	PianorollCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const wxString &name);
	virtual ~PianorollCanvas();


private:
	void OnPaint(wxPaintEvent &event);
	void OnLeftClick(wxMouseEvent &event);

	wxDECLARE_EVENT_TABLE();

protected:
	virtual void DrawKeyboard(wxGraphicsContext *gc);

	double white_width = 60;
	double white_height = 30;
	double black_width = 25;
	double black_height = 20;
	double pianoroll_leftmargin = 5;
	double pianoroll_upmargin = 25;
};


class PianorollWidget: public wxWindow
{
public:
	PianorollWidget(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const wxString &name);
	virtual ~PianorollWidget();
// void add_note(MidiNote note);

// void set_x_scale(double scale);
// void set_y_scale(double scale);

// void calculate_note_positions();
// void calculate_automation_graph();

// void reset_with_notes(smf::MidiEventList event_list);
// void add_midi_note(smf::MidiEvent event);

// void request_redraw();
// void request_automation_redraw();

// void update_size_request();

private:
// Vector<MidiEvent> midiInfo;
	PianorollCanvas *scWindow;
// Gtk::ScrolledWindow *scwindow;
// Gtk::Viewport *viewport;

// Gtk::Paned *paned;
// Gtk::DrawingArea *automation;
// Gtk::ScrolledWindow *scautomation;
// Gtk::Box *automation_box;
// Gtk::Toolbar *automation_toolbar;

// Gtk::ToolButton *automation_test_button;

// bool drawarea_on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
// bool automation_on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
//bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
//std::queue<DrawCord> drawing_queue;

//std::queue<DrawCord> note_drawed;
// std::priority_queue<DrawCord, std::vector<DrawCord>, decltype(drawcord_comp)> *automation_drawed;
// std::array<std::stack<smf::MidiEvent>, 128> _note_tmp;

// std::vector<MidiNote> notes;
// std::vector<AutomationPoint> automation_values;

// bool draw_initialized;

// double y_scale = 5.0;
// double x_scale = 5.0;

// double key_heights[127];

// int resolution = 480;
// double tempo = 120;

// protected:
// virtual bool on_motion_notify_event(GdkEventMotion *motion_event);
// void draw_note(MidiNote note);
// virtual void update_notes_drawing(const Cairo::RefPtr<Cairo::Context>& cr);
// virtual void draw_gridhelper(const Cairo::RefPtr<Cairo::Context>& cr);
// virtual void draw_timeline(const Cairo::RefPtr<Cairo::Context>& cr);

// virtual void automation_draw_timeline(const Cairo::RefPtr<Cairo::Context>& cr);
// virtual void automation_draw_graph(const Cairo::RefPtr<Cairo::Context>& cr);


// double max_width = 100;

// int measure_division = 4;
// double measure_width = 300;
};
