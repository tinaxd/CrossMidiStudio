#include "pianoroll_widget.h"

#include <wx/graphics.h>
#include <iostream>

#define WHITE_KEYS 75

#define SCROLL_RATE_H 10
#define SCROLL_RATE_V 10

#define GET_SCROLL_OFFSET(x, y) int x, y; GetViewStart(&x, &y); x *= SCROLL_RATE_H; y *= SCROLL_RATE_V

wxBEGIN_EVENT_TABLE(PianorollCanvas, wxScrolledCanvas)
	EVT_PAINT(PianorollCanvas::OnPaint)
	EVT_LEFT_DOWN(PianorollCanvas::OnLeftClick)
	//EVT_SCROLLWIN(PianorollCanvas::OnScroll)
wxEND_EVENT_TABLE()

PianorollWidget::PianorollWidget(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const wxString &name)
	: wxWindow(parent, id, pos, size, wxBORDER_SIMPLE, name)
{
	topsizer = new wxBoxSizer(wxVERTICAL);

	scWindow = new PianorollCanvas(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "pianoroll");
	
	topsizer->Add(scWindow, true, wxEXPAND, 0);

	SetSizerAndFit(topsizer);
}

PianorollWidget::~PianorollWidget()
{

}

PianorollCanvas::PianorollCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const wxString &name)
	: wxScrolledCanvas(parent, id, pos, size, wxHSCROLL | wxVSCROLL, name)
{
	SetScrollbars(SCROLL_RATE_H, SCROLL_RATE_V, 200, 240); 
	//SetVirtualSize(2000, 2000);
	ShowScrollbars(wxSHOW_SB_ALWAYS, wxSHOW_SB_ALWAYS);
}

PianorollCanvas::~PianorollCanvas()
{

}

void PianorollCanvas::OnScroll(wxScrollWinEvent &event)
{
	//Refresh();
}

void PianorollCanvas::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);
	DoPrepareDC(dc);

	GET_SCROLL_OFFSET(x_start, y_start);

	std::cout << "Start: " << x_start << ", " << y_start << std::endl;

	wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

	//std::cout << "OnPaint" << std::endl;

	if (gc)
	{
		gc->Translate(0, y_start);
		DrawTimeline(*gc);

		gc->Translate(x_start, pianoroll_upmargin - y_start);
        DrawKeyboard(*gc);
        

	}

	delete gc;
}

void PianorollCanvas::DrawKeyboard(wxGraphicsContext &gc)
{
	gc.SetPen(*wxBLACK_PEN);
	gc.SetBrush(*wxWHITE_BRUSH);
	gc.SetFont(wxFont(wxFontInfo(13).Family(wxFONTFAMILY_MODERN)), *wxBLACK);
    // Draw white keys
    for (int i=0; i<WHITE_KEYS-1; i++) {
    	wxGraphicsPath path = gc.CreatePath();
    	path.AddRectangle(0, i * white_height, white_width, white_height);
        gc.DrawPath(path);
        if ((i-4) % 7 == 0) {
            int index = (i-4) / 7;
            char str[4];
            sprintf(str, "C%d", 9-index);
            gc.DrawText(str, white_width/2+2, (i+1) * white_height - 20);
        }
    }

    // Draw (the half of) the first black key
    {
    	wxGraphicsPath path = gc.CreatePath();
    	path.AddRectangle(0, 0, black_width, black_height / 2);
    	gc.FillPath(path);
    }

    gc.SetBrush(*wxBLACK_BRUSH);

    int black_index = 2;
    // Draw the rest of the black keys
    for (int i=0; i<53; i++) {
        if (black_index == 3) {
            black_index++;
            continue;
        } else if (black_index == 6) {
            black_index = 0;
            continue;
        } else {
        	wxGraphicsPath path = gc.CreatePath();
            double left_up_y = white_height * (i + 1) - black_height / 2;
            path.AddRectangle(0, left_up_y, black_width, black_height);
            gc.FillPath(path);
            black_index++;
        }
    }
}

void PianorollCanvas::DrawTimeline(wxGraphicsContext &gc)
{
	const double height = WHITE_KEYS * white_height;
    const double width = max_width;
    const double left_margin = pianoroll_leftmargin + white_width;
    const double interval = measure_width / measure_division;

    gc.SetPen(wxPen(wxColour(115, 115, 115), 1));
    gc.SetFont(wxFont(wxFontInfo(13).Family(wxFONTFAMILY_MODERN)), *wxBLACK);

    int i = 0;
    double start;
    do {
        start = left_margin + i * interval;

        if (i % measure_division == 0) {
            char measures[5];
            sprintf(measures, "%d", i / measure_division + 1);
            gc.DrawText(measures, start, pianoroll_upmargin);

            gc.SetPen(wxPen(wxColour(0, 0, 255), 2));
        }
        
        {
	        auto path = gc.CreatePath();
	        path.MoveToPoint(start, 0);
	        path.AddLineToPoint(start, height);
	        gc.StrokePath(path);
	    }

        gc.SetPen(wxPen(wxColour(115, 115, 115), 1));
        i++;
    } while (start < width);
}

void PianorollCanvas::OnLeftClick(wxMouseEvent &event)
{
	wxClientDC dc(this);
	GET_SCROLL_OFFSET(x_start, y_start);

	auto point = event.GetLogicalPosition(dc);
	std::cout << "Mouse click: (" << point.x + x_start << ", " << point.y + y_start << ")" << std::endl;

	event.Skip();
}