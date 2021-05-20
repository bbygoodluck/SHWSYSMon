#include "../../ginc.h"
#include "../SystemCommon.h"
#include "CPUMonitoring.h"
#include "../../panels/views/ViewCommon.h"//cpu/CPUUsageGraphView.h"


wxBEGIN_EVENT_TABLE(CCPUMonitoring, CResourceCommon)
	EVT_TIMER(ENUM_CPU_TIMER_ID, CCPUMonitoring::OnTimer)
wxEND_EVENT_TABLE()

CCPUMonitoring::CCPUMonitoring()
{
	m_timer.SetOwner(this, ENUM_CPU_TIMER_ID);
	InitializeCPUInfo();
}

CCPUMonitoring::~CCPUMonitoring()
{

#ifdef __WXMSW__
	PdhRemoveCounter(m_hCounterCPUTotal);

	for (int iIndex = 0; iIndex < m_iCPUCoreCount; iIndex++)
		PdhRemoveCounter(m_phCounterCPUCore[iIndex]);
#endif
}

void CCPUMonitoring::InitializeCPUInfo()
{
	m_bNotifying = true;
	m_vecCPUUsage.clear();
#ifdef __WXMSW__
	//프로세서 정보
	wchar_t wCpuInfo[100] = {0x00, };
	HKEY hKey;
	DWORD dwSize = sizeof(wCpuInfo);

	//레지스트리를 조사하여 프로세서의 모델명을 얻어냅니다.
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);
    RegQueryValueEx(hKey, L"ProcessorNameString", NULL, NULL, (LPBYTE)wCpuInfo, &dwSize);
    RegCloseKey(hKey);

	m_strProcessorInfo = wCpuInfo;
	//CPU 정보 초기화
	PDH_STATUS status;
	status = PdhOpenQuery(NULL, 0, &m_hCpuQuery);
	if(status != ERROR_SUCCESS)
	{
		theUtility->ShowMessage(wxString::Format(wxT("InitializeCPUInfo() PhdOpenQuery error=%0x"), status), wxOK | wxICON_ERROR);
		m_bNotifying = false;
		return;
	}
	//CPU Total
	status = PdhAddCounter(m_hCpuQuery, TEXT("\\Processor(_Total)\\% Processor Time"), 1, &m_hCounterCPUTotal);

	SYSTEM_INFO SystemInfo = { 0 };
	GetSystemInfo(&SystemInfo);
	if (SystemInfo.dwNumberOfProcessors > 0)
	{
		m_iCPUCoreCount = SystemInfo.dwNumberOfProcessors;
		m_phCounterCPUCore = new HCOUNTER[m_iCPUCoreCount];

		for(int iIndex = 0; iIndex < m_iCPUCoreCount; iIndex++)
		{
			TCHAR szFullCounterPath[1024] = { 0 };
			wsprintf(szFullCounterPath, TEXT("\\Processor(%d)\\%% Processor Time"), iIndex);
			status = PdhAddCounter(m_hCpuQuery, szFullCounterPath, 1, &m_phCounterCPUCore[iIndex]);
			if(status != ERROR_SUCCESS)
			{
				m_bNotifying = false;
				theUtility->ShowMessage(wxString::Format(wxT("InitializeCPUInfo() PdhAddCounter error=%0x"), status), wxOK | wxICON_ERROR);
				return;
			}
		}
	}
#endif // __WXMSW__
}

void CCPUMonitoring::OnTimer(wxTimerEvent& event)
{
//	if(m_bNotifying)
//		return;
	UpdateCPUUsage();
	notify();
}

void CCPUMonitoring::UpdateCPUUsage()
{
	m_lCPUTotalUsage = 0;
	m_vecCPUUsage.clear();
	unsigned long ulCPUUsage = 0;

	if(!m_bNotifying)
		return;
#ifdef __WXMSW__
	PdhCollectQueryData(m_hCpuQuery);
	// 전체 CPU 사용량
	PDH_FMT_COUNTERVALUE PFC_Value = { 0 };
	PDH_STATUS status = PdhGetFormattedCounterValue(m_hCounterCPUTotal, PDH_FMT_DOUBLE, NULL, &PFC_Value);
	if (status == ERROR_SUCCESS)
	{
		ulCPUUsage = (unsigned long)PFC_Value.doubleValue;
		m_lCPUTotalUsage = ulCPUUsage;
	}

	m_vecCPUUsage.emplace_back(ulCPUUsage);
	// 개별 코어 사용량
	for (int c1 = 0; c1 < m_iCPUCoreCount; c1++)
	{
		ulCPUUsage = 0;
		PDH_FMT_COUNTERVALUE PFC_Value = { 0 };
		status = PdhGetFormattedCounterValue(m_phCounterCPUCore[c1], PDH_FMT_DOUBLE, NULL, &PFC_Value);
		if (status == ERROR_SUCCESS)
			ulCPUUsage = (unsigned long)PFC_Value.doubleValue;

		m_vecCPUUsage.emplace_back(ulCPUUsage);
	}
#endif // __WXMSW__
}

void CCPUMonitoring::notify()
{
	int iCpuCore = 0;
	for(auto item : m_notifyItems)
	{
		if(m_vecCPUUsage.size() == 0)
			continue;

		CViewCommon* pView = (CViewCommon *)item;
		iCpuCore = pView->GetCPUCoreNumber();

		pView->SetUsageData(m_vecCPUUsage.at(iCpuCore));
		pView->update();

	//	int iCPUUsage = wx_static_cast(int, m_vecCPUUsage.at(iCpuCore));
	//	wxCommandEvent evt(wxEVT_EVENT_NOTIFY_GRAPHVIEW);

	//	evt.SetInt(iCPUUsage);
	//	wxPostEvent(item, evt);

	//	wxQueueEvent(item, evt.Clone());
	}
}
