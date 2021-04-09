#ifndef PROCESSMONITORING_H_INCLUDED
#define PROCESSMONITORING_H_INCLUDED

#include "../ResourceCommon.h"
#include "ProcessInfoImpl.h"
class CSystemCommon;
class CProcessTrace;

class CProcessMonitoring : public CResourceCommon
{
	friend class CSystemCommon;
	friend class CProcessTrace;

protected:
	enum {
		ENUM_PROCESS_TIMER_ID = PROCESS_TIMER_ID
	};

public:
	CProcessMonitoring();
	~CProcessMonitoring();

	std::unordered_map<unsigned long, SHWSYSMON_PROCESS_INFO *>& GetProcessInfo();
	SHWSYSMON_PROCESS_INFO * GetProcessInfo(unsigned long ulProcessID);

	void AddNewProcess(unsigned long ulProcessID);
	void DelProcess(unsigned long ulProcessID);

protected:
	std::unique_ptr<CProcessInfoImpl> m_ProcessImpl;
	std::unique_ptr<CProcessTrace> m_pProcessTrace;

private:
	virtual void notify() override;

private:
	void OnTimer(wxTimerEvent& event);
	void OnAddDelProcess(wxCommandEvent& event);
	void OnUpdateProcessInfo(wxCommandEvent& event);
	void OnProcessReload(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};


#endif // PROCESSMONITORING_H_INCLUDED
