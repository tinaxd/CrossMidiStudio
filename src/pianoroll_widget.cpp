#include "pianoroll_widget.hpp"

#include <iostream>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

#define WHITE_KEYS 75

#define SCROLL_RATE_H 10
#define SCROLL_RATE_V 10

#define GET_SCROLL_OFFSET(x, y)                                                \
  int x, y;                                                                    \
  GetViewStart(&x, &y);                                                        \
  x *= SCROLL_RATE_H;                                                          \
  y *= SCROLL_RATE_V

#define QUARTER_LENGTH (measure_width / measure_division)

using namespace cms;

// clang-format off
wxBEGIN_EVENT_TABLE(PianorollCanvas, wxScrolledCanvas)
	EVT_PAINT(PianorollCanvas::OnPaint)
	EVT_LEFT_DOWN(PianorollCanvas::OnLeftClick)
	EVT_SCROLLWIN(PianorollCanvas::OnScroll)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(ControlChangeGraph, wxScrolledCanvas)
	EVT_PAINT(ControlChangeGraph::OnPaint)
	EVT_LEFT_DOWN(ControlChangeGraph::OnLeftClick)
wxEND_EVENT_TABLE()
    // clang-format on

    PianorollWidget::PianorollWidget(wxWindow *parent, wxWindowID id,
                                     const wxPoint &pos, const wxSize &size,
                                     const wxString &name)
    : wxWindow(parent, id, pos, size, wxBORDER_SIMPLE, name) {
  topsizer = new wxBoxSizer(wxVERTICAL);
  topsplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition,
                                     wxDefaultSize, wxSP_3D);

  scWindow = new PianorollCanvas(topsplitter, wxID_ANY, wxDefaultPosition,
                                 wxDefaultSize, "pianoroll");

  wxWindow *automationTmpWindow = new wxWindow(topsplitter, wxID_ANY);
  automationBox = new wxBoxSizer(wxVERTICAL);
  automationToolBar =
      new wxToolBar(automationTmpWindow, wxID_ANY, wxDefaultPosition,
                    wxDefaultSize, wxTB_HORIZONTAL, "ccgraphtools");
  automationTestButton =
      new wxButton(automationToolBar, wxID_ANY, "FOO!", wxDefaultPosition,
                   wxDefaultSize, 0, wxDefaultValidator, "ccgraphtoolbutton");
  scAutomation =
      new ControlChangeGraph(automationTmpWindow, wxID_ANY, wxDefaultPosition,
                             wxDefaultSize, "ccgraph");
  scAutomation->SetPianorollCanvas(*scWindow);
  scWindow->SetControlChangeGraph(*scAutomation);

  automationToolBar->AddControl(automationTestButton);

  automationBox->Add(automationToolBar, false, wxEXPAND, 0);
  automationBox->Add(scAutomation, true, wxEXPAND, 0);

  automationTmpWindow->SetSizerAndFit(automationBox);

  topsplitter->SplitHorizontally(scWindow, automationTmpWindow);

  topsizer->Add(topsplitter, true, wxEXPAND, 0);
  SetSizerAndFit(topsizer);
}

PianorollWidget::~PianorollWidget() {}

void PianorollWidget::AddMidiNote(smf::MidiEvent event) {
  if (event.isNoteOn()) {
    short key = event[1];
    short velocity = event[2];
    // std::cout << "raw velocity: " << velocity << std::endl;
    _note_tmp[key].push(event);
  } else if (event.isNoteOff()) {
    if (_note_tmp.empty())
      return;
    short key = event[1];
    smf::MidiEvent on_event = _note_tmp[key].top();
    MidiNote note = {
        key,
        event.tick - on_event.tick,
        on_event.tick,
    };
    scWindow->notes.push_back(note);
    _note_tmp[key].pop();
    std::cout << "New note! note: " << key << " time(tick): " << on_event.tick
              << "(length " << event.tick - on_event.tick << ")" << std::endl;

    // AutomationPoint ap = {
    //     on_event[2], // velocity
    //     on_event.tick,
    // };
    // automation_values.push_back(ap);
    // std::cout << "New AutoPoint! time(tick): " << on_event.tick << " value: "
    // << on_event[2] << std::endl;

    if (event.tick > scWindow->max_width) {
			// update window's size request (maybe unnecessary)
      scWindow->max_width =
          event.tick / scWindow->resolution *
              (scWindow->measure_width / scWindow->measure_division) +
          10 * scWindow->measure_width;
			// expand scrollbar
			scWindow->GetScroll
    }
  }
}

void PianorollWidget::ClearMidiNotes() { scWindow->notes.clear(); }

void PianorollWidget::RequestRedrawAll() {
  if (scWindow != nullptr) {
    scWindow->Refresh();
    scWindow->Update();
  }
  if (scAutomation != nullptr) {
    scAutomation->Refresh();
    scAutomation->Update();
  }
}

int PianorollWidget::GetTickBar()
{
    return 0; // TODO
}

void PianorollWidget::SetTickBar(int tick)
{
    return; // TODO
}

PianorollCanvas::PianorollCanvas(wxWindow *parent, wxWindowID id,
                                 const wxPoint &pos, const wxSize &size,
                                 const wxString &name)
    : wxScrolledCanvas() {
  //SetBackgroundStyle(wxBG_STYLE_PAINT);
  Create(parent, id, pos, size, wxHSCROLL | wxVSCROLL, name);

  SetScrollbars(SCROLL_RATE_H, SCROLL_RATE_V, 200, 240);
  // SetVirtualSize(2000, 2000);
  ShowScrollbars(wxSHOW_SB_ALWAYS, wxSHOW_SB_ALWAYS);
}

PianorollCanvas::~PianorollCanvas() {}

void PianorollCanvas::SetControlChangeGraph(ControlChangeGraph &ccGraph) {
  this->ccGraph = &ccGraph;
}

void PianorollCanvas::OnScroll(wxScrollWinEvent &event) {
  this->HandleOnScroll(event);
  Refresh();
  if (event.GetOrientation() == wxHORIZONTAL) {
    if (ccGraph != nullptr) {
      ccGraph->SetHScroll(event.GetPosition() * SCROLL_RATE_H);
    }
  }
}

void PianorollCanvas::OnPaint(wxPaintEvent &event) {
  //wxAutoBufferedPaintDC dc(this);
  wxPaintDC dc(this);
  DoPrepareDC(dc);

  GET_SCROLL_OFFSET(x_start, y_start);

  // std::cout << "Start: " << x_start << ", " << y_start << std::endl;

  // std::cout << "OnPaint" << std::endl;

  dc.SetBackground(*wxWHITE_BRUSH);
  dc.SetBackgroundMode(wxTRANSPARENT);

  int width, height;
  GetClientSize(&width, &height);

  dc.SetBrush(*wxWHITE_BRUSH);
  dc.DrawRectangle(x_start, y_start, x_start + width, y_start + height);

  auto init_transform = dc.GetTransformMatrix();
  wxAffineMatrix2D transform;
  transform.Concat(init_transform);

  // transform.Translate(white_width + pianoroll_leftmargin, 0);
  // dc.SetTransformMatrix(transform);
  transform.Translate(white_width + pianoroll_leftmargin, pianoroll_upmargin);
  dc.SetTransformMatrix(transform);
  DrawGridhelper(dc);

  // transform.Translate(0, y_start);
  // dc.SetTransformMatrix(transform);
  init_transform.Translate(0, y_start);
  dc.SetTransformMatrix(init_transform);
  DrawTimeline(dc);

  // transform.Translate(x_start, pianoroll_upmargin - y_start);
  // dc.SetTransformMatrix(transform);
  init_transform.Translate(x_start, -y_start + pianoroll_upmargin);
  dc.SetTransformMatrix(init_transform);
  DrawKeyboard(dc);

  dc.SetTransformMatrix(transform);
  CalculateNotePositions();
  DrawAllNotes(dc);
}

void PianorollCanvas::DrawKeyboard(wxDC &dc) {
  dc.SetPen(*wxBLACK_PEN);
  dc.SetBrush(*wxWHITE_BRUSH);
  dc.SetTextForeground(*wxBLACK);
  dc.SetFont(wxFont(wxFontInfo(13).Family(wxFONTFAMILY_MODERN)));
  // Draw white keys
  for (int i = 0; i < WHITE_KEYS - 1; i++) {
    dc.DrawRectangle(0, i * white_height, white_width, white_height);
    if ((i - 4) % 7 == 0) {
      int index = (i - 4) / 7;
      char str[4];
      sprintf(str, "C%d", 9 - index);
      dc.DrawText(str, white_width / 2 + 2, (i + 0.5) * white_height);
    }
  }

  dc.SetBrush(*wxBLACK_BRUSH);

  // Draw (the half of) the first black key
  dc.DrawRectangle(0, 0, black_width, black_height / 2);

  int black_index = 2;
  // Draw the rest of the black keys
  for (int i = 0; i < 53; i++) {
    if (black_index == 3) {
      black_index++;
      continue;
    } else if (black_index == 6) {
      black_index = 0;
      continue;
    } else {
      double left_up_y = white_height * (i + 1) - black_height / 2;
      dc.DrawRectangle(0, left_up_y, black_width, black_height);
      black_index++;
    }
  }
}

void PianorollCanvas::DrawTimeline(wxDC &dc) {
  const double height = WHITE_KEYS * white_height;
  const double width = max_width;
  const double left_margin = pianoroll_leftmargin + white_width;
  const double interval = measure_width / measure_division;

  dc.SetPen(wxPen(wxColour(115, 115, 115), 1));
  dc.SetTextForeground(*wxBLACK);
  dc.SetFont(wxFont(wxFontInfo(13).Family(wxFONTFAMILY_MODERN).Bold()));

  int i = 0;
  double start;
  do {
    start = left_margin + i * interval;

    if (i % measure_division == 0) {
      char measures[5];
      sprintf(measures, "%d", i / measure_division + 1);
      dc.DrawText(measures, start, 0);

      dc.SetPen(wxPen(wxColour(0, 0, 255), 2));
    }

    dc.DrawLine(start, 0, start, height);

    dc.SetPen(wxPen(wxColour(115, 115, 115), 1));
    i++;
  } while (start < width);
}

void PianorollCanvas::OnLeftClick(wxMouseEvent &event) {
  wxClientDC dc(this);
  GET_SCROLL_OFFSET(x_start, y_start);

  auto point = event.GetLogicalPosition(dc);
  std::cout << "Mouse click: (" << point.x + x_start << ", "
            << point.y + y_start << ")" << std::endl;

  event.Skip();
}

void PianorollCanvas::DrawGridhelper(wxDC &dc) {
  const double key_height = white_height * 7 / 12;
  const double pianoroll_width = max_width - white_width;

  dc.SetPen(wxPen(wxColour(200, 200, 200), 1));
  dc.SetBrush(*wxTRANSPARENT_BRUSH);

  double last_base_white = -2 * white_height;
  int key_index = 3;
  int octave_index = 0;
  for (int i = 0; i < 127; i++) {
    if (key_index == 12) {
      key_index = 0;
      octave_index++;
      i--;
      continue;
    } else if (key_index == 0) {
      dc.SetPen(wxPen(wxColour(51, 51, 51), 2));
      last_base_white = (7 * octave_index - 2) * white_height;
      // dc.DrawRectangle(0, last_base_white, pianoroll_width, key_height);
      dc.DrawLine(0, last_base_white, pianoroll_width, last_base_white);
      // std::cout << "set: " << last_base_white << std::endl;
      key_heights[i] = last_base_white;
      dc.SetPen(wxPen(wxColour(200, 200, 200), 1));
    } else if (key_index == 1 || key_index == 3 || key_index == 5 ||
               key_index == 8 || key_index == 10) {
      double y = last_base_white + key_height * key_index;
      dc.SetPen(wxPen(wxColour(200, 200, 200), 1));
      dc.SetBrush(wxBrush(wxColour(231, 255, 241), wxBRUSHSTYLE_SOLID));
      dc.DrawRectangle(0, y, pianoroll_width, key_height);
      dc.SetBrush(*wxTRANSPARENT_BRUSH);
      key_heights[i] = y;
    } else {
      double y = last_base_white + key_height * key_index;
      // dc.DrawRectangle(0, y, pianoroll_width, key_height);
      dc.DrawLine(0, y, pianoroll_width, y);
      // std::cout << "set: " << y << std::endl;
      key_heights[i] = y;
    }

    key_index++;
  }

  dc.DrawLine(0, 0, 0, WHITE_KEYS * white_height);
}

void PianorollCanvas::GetScrollOffset(int &x_start, int &y_start) {
  GetViewStart(&x_start, &y_start);
}

void PianorollCanvas::CalculateNotePositions() {
  for (auto it = notes.begin(); it != notes.end(); it++) {
    auto note = *it;
    // std::cout << key_heights[127-note.note] << std::endl;
    DrawCord cord = DrawCord{double(note.start) / resolution * QUARTER_LENGTH,
                             key_heights[128 - note.note],
                             double(note.length) / resolution * QUARTER_LENGTH};
    this->note_drawed.push(cord);
  }
}

void PianorollCanvas::DrawAllNotes(wxDC &dc) {
  dc.SetPen(*wxBLACK_PEN);
  dc.SetBrush(*wxBLUE_BRUSH);
  const double key_height = white_height * 7 / 12;

  while (!note_drawed.empty()) {
    DrawCord cord = note_drawed.front();
    // std::cout << "Cord: " << cord.x << ", " << cord.y << std::endl;
    dc.DrawRectangle(cord.x, cord.y, cord.length, key_height);

    note_drawed.pop();
  }
}

void ControlChangeGraph::SetPianorollCanvas(PianorollCanvas &canvas) {
  this->pianorollCanvas = &canvas;
}

ControlChangeGraph::ControlChangeGraph(wxWindow *parent, wxWindowID id,
                                       const wxPoint &pos, const wxSize &size,
                                       const wxString &name)
    : wxWindow() {
  // SetBackgroundStyle(wxBG_STYLE_PAINT);
  Create(parent, id, pos, size, 0, name);
}

ControlChangeGraph::~ControlChangeGraph() {}

void ControlChangeGraph::OnPaint(wxPaintEvent &event) {
  wxPaintDC dc(this);

  dc.SetBackground(*wxWHITE_BRUSH);
  dc.SetBackgroundMode(wxTRANSPARENT);

  int width, height;
  GetClientSize(&width, &height);

  dc.SetBrush(*wxWHITE_BRUSH);
  dc.DrawRectangle(0, 0, width, height);

  auto transform = dc.GetTransformMatrix();
  // wxAffineMatrix2D transform;
  // transform.Concat(init_transform);

  transform.Translate(-hScroll, 0);
  dc.SetTransformMatrix(transform);
  DrawTimeline(dc);
}

void ControlChangeGraph::DrawTimeline(wxDC &dc) {
  const double height = WHITE_KEYS * pianorollCanvas->white_height;
  const double width = pianorollCanvas->max_width;
  const double left_margin =
      pianorollCanvas->pianoroll_leftmargin + pianorollCanvas->white_width;
  const double interval =
      pianorollCanvas->measure_width / pianorollCanvas->measure_division;

  dc.SetPen(wxPen(wxColour(115, 115, 115), 1));
  dc.SetTextForeground(*wxBLACK);
  dc.SetFont(wxFont(wxFontInfo(13).Family(wxFONTFAMILY_MODERN).Bold()));

  int i = 0;
  double start;
  do {
    start = left_margin + i * interval;

    if (i % pianorollCanvas->measure_division == 0) {
      char measures[5];
      sprintf(measures, "%d", i / pianorollCanvas->measure_division + 1);
      dc.DrawText(measures, start, 0);

      dc.SetPen(wxPen(wxColour(0, 0, 255), 2));
    }

    dc.DrawLine(start, 0, start, height);

    dc.SetPen(wxPen(wxColour(115, 115, 115), 1));
    i++;
  } while (start < width);
}

void ControlChangeGraph::OnLeftClick(wxMouseEvent &event) {
  wxClientDC dc(this);

  auto point = event.GetLogicalPosition(dc);
  std::cout << "Mouse click: (" << point.x + hScroll << ", " << point.y << ")"
            << std::endl;

  event.Skip();
}

void ControlChangeGraph::SetHScroll(int scroll) {
  this->hScroll = scroll;
  Refresh();
  //Update();
}