#ifndef EVENTHANDLER_H_INCLUDED
#define EVENTHANDLER_H_INCLUDED

class CMainFrame;
class CEventHandler : public wxEvtHandler
{

public:
	explicit CEventHandler(CMainFrame* _pMainFrame);
	~CEventHandler();

private:
	CMainFrame* m_pMainFrame;

private:
	void DoProcessReload();

private:
	void OnMenuEvent(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();
};

#endif // EVENTHANDLER_H_INCLUDED
