#include "main.hpp"

bool MyApp::OnInit() {
	if (!wxApp::OnInit())
		return false;

	wxFrame* frame = new MyFrame1(NULL, wxID_ANY, "LicensePlateDetection", wxDefaultPosition, wxWindow::FromDIP(wxSize(300, 160), NULL));
	frame->Show(true);

	return true;
}


wxBEGIN_EVENT_TABLE(MyFrame1, wxFrame)
	EVT_MENU(ID_BUTTON, MyFrame1::on_m_button_click)
wxEND_EVENT_TABLE()


MyFrame1::MyFrame1(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxSize(230, 160), wxSize(230, 160));

	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer(0, 0);
	gbSizer1->SetFlexibleDirection(wxBOTH);
	gbSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_button1 = new wxButton(this, wxID_ANY, wxT("Submit"), wxDefaultPosition, wxDefaultSize, 0);
	gbSizer1->Add(m_button1, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	wxArrayString m_choice_cvChoices;
	m_choice_cvChoices.Add("Picture", 1);
	m_choice_cvChoices.Add("Video", 1);
	m_choice_cvChoices.Add("Camera", 1);
	m_choice_cv = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_cvChoices, 0);
	m_choice_cv->SetSelection(0);
	gbSizer1->Add(m_choice_cv, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);

	m_textCtrl_input = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, 50), 0);
	gbSizer1->Add(m_textCtrl_input, wxGBPosition(2, 0), wxGBSpan(1, 1), wxALL, 5);

	m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("Input Stream, Picture"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	gbSizer1->Add(m_staticText2, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALL, 5);

	this->SetSizer(gbSizer1);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	m_button1->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::on_m_button_click), NULL, this);
}

MyFrame1::~MyFrame1()
{
	// Disconnect Events
	m_button1->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame1::on_m_button_click), NULL, this);

}

void MyFrame1::on_m_button_click(wxCommandEvent& event) { 
	wxString input;
	unsigned short choice;
	if (m_textCtrl_input->IsEmpty()) {
		wxLogMessage("Add a file, camera or stream to the Input box");
		Close(true);
	}
	else {
		input = m_textCtrl_input->GetValue();
		choice = m_choice_cv->GetSelection();	
		// Send over our choice of video or picture and our input stream to the opencv program
		wxString exec_use = wxString::Format("/home/joachim/projects/opencv_part/bin/x64/Debug/opencv_part.out %i %s", choice, input);
		wxExecute(exec_use, wxEXEC_ASYNC | wxEXEC_SHOW_CONSOLE);
	}
	event.Skip();
}

//void main_frame::on_exit(wxCommandEvent& WXUNUSED(event)) {
//	Close(true);
//}
//
//void main_frame::on_about(wxCommandEvent& WXUNUSED(event)) {
//	wxMessageBox(wxT("ABOUT BOX: \nJoachim Flottorp"), wxT("About Menu"), wxOK, this);
//}