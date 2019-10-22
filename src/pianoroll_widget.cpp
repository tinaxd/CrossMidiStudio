#include "pianoroll_widget.h"

#include <wx/graphics.h>
#include <iostream>

#define WHITE_KEYS 75

wxBEGIN_EVENT_TABLE(PianorollCanvas, wxScrolledCanvas)
	EVT_PAINT(PianorollCanvas::OnPaint)
	EVT_LEFT_DOWN(PianorollCanvas::OnLeftClick)
wxEND_EVENT_TABLE()

PianorollWidget::PianorollWidget(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const wxString &name)
	: wxWindow(parent, id, pos, size, wxBORDER_SIMPLE, name)
{
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

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

}

PianorollCanvas::~PianorollCanvas()
{

}

void PianorollCanvas::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);

	int x_start, y_start;
	GetViewStart(&x_start, &y_start);

	//std::cout << "Start: " << x_start << ", " << y_start << std::endl;

	wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

	std::cout << "Context" << std::endl;

	if (gc)
	{
		/*
		std::cout << "Paiting" << std::endl;
		gc->SetPen(*wxRED_PEN);
        wxGraphicsPath path = gc->gceatePath();
        path.AddCircle( 50.0, 50.0, 50.0 );
        path.MoveToPoint(0.0, 50.0);
        path.AddLineToPoint(100.0, 50.0);
        path.MoveToPoint(50.0, 0.0);
        path.AddLineToPoint(50.0, 100.0 );
        path.CloseSubpath();
        path.AddRectangle(25.0, 25.0, 50.0, 50.0);

        gc->StrokePath(path);*/
        DrawKeyboard(gc);

        delete gc;
	}
}

void PianorollCanvas::DrawKeyboard(wxGraphicsContext *gc)
{
	gc->SetPen(*wxBLACK_PEN);
	gc->SetBrush(*wxWHITE_BRUSH);
	gc->SetFont(wxFont(wxFontInfo(13).Family(wxFONTFAMILY_MODERN)), *wxBLACK);
    // Draw white keys
    for (int i=0; i<WHITE_KEYS-1; i++) {
    	wxGraphicsPath path = gc->CreatePath();
    	path.AddRectangle(0, i * white_height, white_width, white_height);
        gc->DrawPath(path);
        if ((i-4) % 7 == 0) {
            int index = (i-4) / 7;
            char str[4];
            sprintf(str, "C%d", 9-index);
            gc->DrawText(str, white_width/2+2, (i+1) * white_height - 20);
        }
    }

    // Draw (the half of) the first black key
    {
    	wxGraphicsPath path = gc->CreatePath();
    	path.AddRectangle(0, 0, black_width, black_height / 2);
    	gc->FillPath(path);
    }

    gc->SetBrush(*wxBLACK_BRUSH);

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
        	wxGraphicsPath path = gc->CreatePath();
            double left_up_y = white_height * (i + 1) - black_height / 2;
            path.AddRectangle(0, left_up_y, black_width, black_height);
            gc->FillPath(path);
            black_index++;
        }
    }
}

void PianorollCanvas::OnLeftClick(wxMouseEvent &event)
{
	wxClientDC dc(this);

	auto point = event.GetLogicalPosition(dc);
	std::cout << "Mouse click: (" << point.x << ", " << point.y << ")" << std::endl;

	event.Skip();
}