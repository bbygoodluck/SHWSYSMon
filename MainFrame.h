#ifndef MAINFRAME_H_INCLUDED
#define MAINFRAME_H_INCLUDED

class CPanelCPU;
class CPanelProcess;
class CSHWSYSStatusBar;
class CEventHandler;

class CMainFrame : public wxFrame
{

public:
	explicit CMainFrame(const wxString& strTitle);
	virtual ~CMainFrame();

private:
	void Initialize();
	void RegisterEventHandler();

private:
	std::unique_ptr<CSHWSYSStatusBar> m_statusBarSummary;
	wxAuiNotebook* m_auiNoteBookCPU;
	wxAuiNotebook* m_auiNoteBookProcess;
	CPanelCPU* m_panelCPU;
	CPanelProcess* m_panelProcess;
	CEventHandler*	m_pSHFEventHandler;

private:
	void OnClose(wxCloseEvent& event);
	void OnMaximized(wxMaximizeEvent& event);
	void OnIconized(wxIconizeEvent& event);
	wxDECLARE_EVENT_TABLE();
};

#endif // MAINFRAME_H_INCLUDED
