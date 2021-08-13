#ifndef MAIN_HPP
#define MAIN_HPP

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <string>

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#define wxUSE_ANY
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/button.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/gbsizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

class MyApp : public wxApp {
public:
	bool OnInit();
};

wxDECLARE_APP(MyApp);
wxIMPLEMENT_APP_CONSOLE(MyApp);

///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame1
///////////////////////////////////////////////////////////////////////////////
class MyFrame1 : public wxFrame {
private:
	void on_m_button_click(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
protected:
	wxButton* m_button1;
	wxChoice* m_choice_cv;
	wxTextCtrl* m_textCtrl_input;
	wxStaticText* m_staticText2;


public:

	MyFrame1(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(640, 360), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

	~MyFrame1();

};

enum {
	ID_BUTTON = 5000
};



#endif // !MAIN_HPP