#include "./ginc.h"
#include "MainFrame.h"
#include "EventHandler.h"
#include "./system/process/ProcessMonitoring.h"

wxBEGIN_EVENT_TABLE(CEventHandler, wxEvtHandler)
	EVT_MENU(wxID_ANY, CEventHandler::OnMenuEvent)
wxEND_EVENT_TABLE()

CEventHandler::CEventHandler(CMainFrame* _pMainFrame)
	: m_pMainFrame(_pMainFrame)
{

}

CEventHandler::~CEventHandler()
{

}

void CEventHandler::OnMenuEvent(wxCommandEvent& event)
{
	int _iMenuID = event.GetId();

	if (_iMenuID == XRCID("systemProcessReload")) DoProcessReload();
}

void CEventHandler::DoProcessReload()
{
	CProcessMonitoring* pProcessMonObj = (CProcessMonitoring *)theSystem->GetBaseResource(S_BASE_CLASS_PROCESS);

	wxCommandEvent evt(wxEVT_EVENT_PROCESS_RELOAD);
	wxPostEvent(pProcessMonObj, evt);
}
