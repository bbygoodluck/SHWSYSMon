#include "../ginc.h"
#include "ProcessManager.h"
#include "../system/process/ProcessMonitoring.h"
#include "./views/ProcessListViewCommon.h"

CProcessManager::CProcessManager(CProcessMonitoring* pMonitoring)
	: m_pMonitoring(pMonitoring)
{
	if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR)
	{
		theUtility->ShowMessage(wxT("Could not create the worker thread!"), wxOK | wxICON_ERROR);
		return;
	}
}

CProcessManager::~CProcessManager()
{
	if(GetThread() && GetThread()->IsRunning())
	{
		m_bThreadStop = true;
		GetThread()->Wait();
	}
}

wxThread::ExitCode CProcessManager::Entry()
{
	while(true)
	{
		if(m_bThreadStop)
			break;

		m_pMonitoring->Update();
		for(auto item : m_notifyItems)
			((CProcessListViewCommon *)item)->update();

		if(m_bThreadStop)
			break;

		Sleep(1000);
	}

	return (wxThread::ExitCode)0;
}

void CProcessManager::StartMonitoring()
{
	GetThread()->Run();
}
