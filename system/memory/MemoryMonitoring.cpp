#include "../../ginc.h"
#include "../SystemCommon.h"
#include "MemoryMonitoring.h"
#include "../../panels/views/ViewCommon.h"

wxBEGIN_EVENT_TABLE(CMemoryMonitoring, CResourceCommon)
	EVT_TIMER(ENUM_MEM_TIMER_ID, CMemoryMonitoring::OnTimer)
wxEND_EVENT_TABLE()

CMemoryMonitoring::CMemoryMonitoring()
{
	m_timer.SetOwner(this, ENUM_MEM_TIMER_ID);
	InitMemoryInfo();
}

CMemoryMonitoring::~CMemoryMonitoring()
{

}

void CMemoryMonitoring::InitMemoryInfo()
{
	update();
}

void CMemoryMonitoring::OnTimer(wxTimerEvent& event)
{
	update();
	notify();
}

void CMemoryMonitoring::update()
{
	MEMORYSTATUSEX MemoryStatus = { 0 };
	MemoryStatus.dwLength = sizeof(MemoryStatus);
	::GlobalMemoryStatusEx(&MemoryStatus);

	//물리 메모리
	m_memInfo._ullPhysicalUsedRam = ((MemoryStatus.ullTotalPhys - MemoryStatus.ullAvailPhys) / (1024 * 1024));
	m_memInfo._ullPhysicalTotalRam = MemoryStatus.ullTotalPhys / (1024 * 1024);
	//가상메모리
	m_memInfo._ullVirtualUsedRam = ((MemoryStatus.ullTotalVirtual - MemoryStatus.ullAvailVirtual) / (1024 * 1024));
	m_memInfo._ullVirtualTotalRam = ((MemoryStatus.ullTotalVirtual) / (1024 * 1024));
	//페이지파일
	m_memInfo._ullPageFileUsed   = ((MemoryStatus.ullTotalPageFile - MemoryStatus.ullAvailPageFile) / (1024 * 1024));
	m_memInfo._ullPageFileTotal = ((MemoryStatus.ullTotalPageFile) / (1024 * 1024));
}

void CMemoryMonitoring::notify()
{
	for(auto item : m_notifyItems)
		((CViewCommon *)item)->update();
}
