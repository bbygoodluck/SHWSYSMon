#include "ginc.h"
#include "MainFrame.h"
#include "SHWSYSStatusBar.h"

#include "./panels/PanelCPU.h"
#include "./panels/PanelProcess.h"

wxBEGIN_EVENT_TABLE(CMainFrame, wxFrame)
	EVT_CLOSE(CMainFrame::OnClose)
	EVT_MAXIMIZE(CMainFrame::OnMaximized)
	EVT_ICONIZE(CMainFrame::OnIconized)
wxEND_EVENT_TABLE()

CMainFrame::CMainFrame(const wxString& strTitle)
	: wxFrame(nullptr, wxID_ANY, strTitle, wxPoint(theSettings->GetXPos(), theSettings->GetYPos()),
		                                wxSize(theSettings->GetWidth(), theSettings->GetHeight()))
{
	SetIcon(wxIcon("wxwin"));
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	Initialize();

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_auiNoteBookCPU = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN );

	m_panelCPU = new CPanelCPU(m_auiNoteBookCPU);
	m_auiNoteBookCPU->AddPage(m_panelCPU, wxT("CPU Usage Graph"), false, wxArtProvider::GetBitmap( wxART_ADD_BOOKMARK, wxART_MENU ) );

	mainSizer->Add( m_auiNoteBookCPU, 0, wxEXPAND, 5 );

	m_auiNoteBookProcess = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelProcess = new CPanelProcess( m_auiNoteBookProcess);
	m_auiNoteBookProcess->AddPage( m_panelProcess, wxT("Process Information"), false, wxArtProvider::GetBitmap( wxART_HARDDISK, wxART_MENU ) );
	mainSizer->Add( m_auiNoteBookProcess, 1, wxEXPAND|wxTOP, 5 );

	this->SetSizer( mainSizer );
	this->Layout();

	if (theSettings->IsMaximized())
		((wxTopLevelWindow*)wxTheApp->GetTopWindow())->Maximize(true);

	m_statusBarSummary = std::make_unique<CSHWSYSStatusBar>(this);
	SetStatusBar(m_statusBarSummary.get());

	this->Center(wxBOTH);
}

CMainFrame::~CMainFrame()
{

}

void CMainFrame::Initialize()
{
	int iMonitors = wxDisplay::GetCount();

	int iCurrDisp = wxDisplay::GetFromWindow(this);
	if (iCurrDisp < 0)
		iCurrDisp = 0;

	wxDisplay disp(iCurrDisp);

	if (!theSettings->IsMaximized())
	{
		//모니터 하나 && 기존 모니터 인덱스와 다를 경우는 Default로 셋팅
		int iMonitorPos = theSettings->GetMonitorPos();
		bool bDefaultSetting = (iMonitors == 1) && (iCurrDisp != iMonitorPos);
		if (bDefaultSetting)
		{
			wxRect rcDefault = theSettings->GetDefaultRect();

			theSettings->SetChnageOptionVal(wxT("DispMonitor"), 0);
			theSettings->SetChnageOptionVal(wxT("XPos"), rcDefault.GetLeft());
			theSettings->SetChnageOptionVal(wxT("YPos"), rcDefault.GetRight());
			theSettings->SetChnageOptionVal(wxT("Width"), rcDefault.GetWidth());
			theSettings->SetChnageOptionVal(wxT("Height"), rcDefault.GetHeight());

			if (!theSettings->IsMaximized())
				this->SetSize(rcDefault);
		}
	}
}

void CMainFrame::OnClose(wxCloseEvent& event)
{
	int iCurrDisp = wxDisplay::GetFromWindow(this);
	theSettings->SetChnageOptionVal(wxT("DispMonitor"), iCurrDisp);
	bool bMaximized = this->IsMaximized();
	theSettings->SetChnageOptionVal(wxT("Maximized"), bMaximized ? 1 : 0);

	wxSize szFrame = this->GetSize();
	theSettings->SetChnageOptionVal(wxT("Width"), szFrame.GetWidth());
	theSettings->SetChnageOptionVal(wxT("Height"), szFrame.GetHeight());

	Destroy();
}

void CMainFrame::OnMaximized(wxMaximizeEvent& event)
{
	theSettings->SetChnageOptionVal(wxT("Maximized"), 1);
}

void CMainFrame::OnIconized(wxIconizeEvent& event)
{

}
