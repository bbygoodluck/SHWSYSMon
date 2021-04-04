#include "../../ginc.h"
#include "../SystemCommon.h"
#include "ProcessMonitoring.h"
#include "ProcessInfoImpl.h"
#include "../../panels/views/ProcessListViewCommon.h"

#include "ProcessTrace.h"

wxBEGIN_EVENT_TABLE(CProcessMonitoring, CResourceCommon)
	EVT_TIMER(ENUM_PROCESS_TIMER_ID, CProcessMonitoring::OnTimer)
	EVT_MY_CUSTOM_COMMAND(wxEVT_EVENT_NOTIFY_ADD_DELPROCESS, wxID_ANY, CProcessMonitoring::OnAddDelProcess)
	EVT_MY_CUSTOM_COMMAND(wxEVT_EVENT_NOTIFY_UPDATE_PROCESSINFO, wxID_ANY, CProcessMonitoring::OnUpdateProcessInfo)
wxEND_EVENT_TABLE()

CProcessMonitoring::CProcessMonitoring()
    : m_ProcessImpl(std::make_unique<CProcessInfoImpl>())
{
	m_timer.SetOwner(this, ENUM_PROCESS_TIMER_ID);
	m_pProcessTrace = std::make_unique<CProcessTrace>();
}

CProcessMonitoring::~CProcessMonitoring()
{
}

void CProcessMonitoring::OnTimer(wxTimerEvent& event)
{
	notify();
}

void CProcessMonitoring::notify()
{
	wxCommandEvent evt(wxEVT_EVENT_NOTIFY_UPDATE_PROCESSINFO);
	wxQueueEvent(this, evt.Clone());
}

void CProcessMonitoring::Update()
{
	m_ProcessImpl->Update();
}

SHWSYSMON_PROCESS_INFO * CProcessMonitoring::GetProcessInfo(unsigned long ulProcessID)
{
	return m_ProcessImpl->GetProcessInfo(ulProcessID);
}

void CProcessMonitoring::Update(SHWSYSMON_PROCESS_INFO *pProcessInfo)
{
	m_ProcessImpl->Update(pProcessInfo);
}

std::unordered_map<unsigned long, SHWSYSMON_PROCESS_INFO *>& CProcessMonitoring::GetProcessInfo()
{
	return m_ProcessImpl->m_mapProcesses;
}

void CProcessMonitoring::AddNewProcess(unsigned long ulProcessID)
{
	if(m_ProcessImpl->AddNewProcess(ulProcessID))
	{
		CProcessListViewCommon::SetProcessLists();
		for(auto item : m_notifyItems)
			((CProcessListViewCommon *)item)->AddNewProcess(ulProcessID);
	}
}

void CProcessMonitoring::DelProcess(unsigned long ulProcessID)
{
	if(m_ProcessImpl->DelProcess(ulProcessID))
	{
		CProcessListViewCommon::SetProcessLists();
		for(auto item : m_notifyItems)
			((CProcessListViewCommon *)item)->DelProcess(ulProcessID);
	}
}

void CProcessMonitoring::OnAddDelProcess(wxCommandEvent& event)
{
	unsigned long ulProcessId = wx_static_cast(unsigned long, event.GetInt());
	wxString strAddDel = event.GetString();

	if(strAddDel.CmpNoCase(wxT("ADD")) == 0) AddNewProcess(ulProcessId);
	else DelProcess(ulProcessId);
}

void CProcessMonitoring::OnUpdateProcessInfo(wxCommandEvent& event)
{
#ifdef __WXMSW__
	EnterCriticalSection(&_gCriProcess);
#endif // __WXMSW__

	m_ProcessImpl->Update();

	for(auto item : m_notifyItems)
		((CProcessListViewCommon *)item)->update();

#ifdef __WXMSW__
	LeaveCriticalSection(&_gCriProcess);
#endif // __WXMSW__

}

int CProcessMonitoring::KillProcess(unsigned long ulProcessID)
{
	return m_ProcessImpl->KillProcess(ulProcessID);
}
