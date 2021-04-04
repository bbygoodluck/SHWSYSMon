#ifndef CPUMONITORING_H_INCLUDED
#define CPUMONITORING_H_INCLUDED

#include "../ResourceCommon.h"

class CSystemCommon;
class CCPUMonitoring : public CResourceCommon
{
	friend class CSystemCommon;
protected:
	enum {
		ENUM_CPU_TIMER_ID = CPU_TIMER_ID
	};

public:
	CCPUMonitoring();
	~CCPUMonitoring();

	int GetCPUCoreCount() { return m_iCPUCoreCount; }

private:
	virtual void notify() override;
	void InitializeCPUInfo();
	void UpdateCPUUsage();

protected:
	int m_iCPUCoreCount = 0;
	unsigned long m_lCPUTotalUsage = 0;

	wxString m_strProcessorInfo = wxT("");

private:
	bool m_bNotifying = false;

private:
#ifdef __WXMSW__
	HQUERY m_hCpuQuery;
	HCOUNTER  m_hCounterCPUTotal;
	HCOUNTER* m_phCounterCPUCore = nullptr;
#endif

	std::vector<unsigned long> m_vecCPUUsage;
private:
	void OnTimer(wxTimerEvent& event);
	wxDECLARE_EVENT_TABLE();
};

#endif // CPUMONITORING_H_INCLUDED
