#include "../ginc.h"
#include "SystemCommon.h"
#include "./ResourceCommon.h"
#include "./cpu/CPUMonitoring.h"
#include "./process/ProcessMonitoring.h"
#include "./memory/MemoryMonitoring.h"

std::unique_ptr<CSystemCommon> CSystemCommon::m_pInstance(nullptr);

CSystemCommon* CSystemCommon::Get()
{
	if (m_pInstance.get() == NULL)
		m_pInstance.reset(new CSystemCommon());

	return m_pInstance.get();
}

void CSystemCommon::SetMonitoringResource(CResourceCommon* pBaseInfo, int iBaseInfoIndex)
{
	constResourceIter fIter = m_mapBaseInfo.find(iBaseInfoIndex);
	if(fIter != m_mapBaseInfo.end())
		return;

	std::unordered_map<int, CResourceCommon *>::value_type val(iBaseInfoIndex, pBaseInfo);
	m_mapBaseInfo.insert(val);
}

CResourceCommon* CSystemCommon::GetBaseResource(int iBaseInfoIndex)
{
	constResourceIter fIter = m_mapBaseInfo.find(iBaseInfoIndex);
	if(fIter == m_mapBaseInfo.end())
		return nullptr;

	return fIter->second;
}

const unsigned long CSystemCommon::GetCPUUsage()
{
	CCPUMonitoring* pCPUInfo = (CCPUMonitoring *)GetBaseResource(S_BASE_CLASS_CPU);
	if(!pCPUInfo)
		return 0;

	return pCPUInfo->m_lCPUTotalUsage;
}

wxString CSystemCommon::GetProcessorInfo()
{
	CCPUMonitoring* pCPUInfo = (CCPUMonitoring *)GetBaseResource(S_BASE_CLASS_CPU);
	if(!pCPUInfo)
		return wxT("");

	return pCPUInfo->m_strProcessorInfo;
}

int CSystemCommon::GetCPUCoreCount()
{
	CCPUMonitoring* pCPUInfo = (CCPUMonitoring *)GetBaseResource(S_BASE_CLASS_CPU);
	if(!pCPUInfo)
		return 0;

	return pCPUInfo->m_iCPUCoreCount;
}

MEMORYINFO* CSystemCommon::GetMemoryInfo()
{
	CMemoryMonitoring* pMemInfo = (CMemoryMonitoring *)GetBaseResource(S_BASE_CLASS_MEMORY);
	if(!pMemInfo)
		return nullptr;

	return &pMemInfo->m_memInfo;
}

unsigned long CSystemCommon::GetTotalMemory()
{
	CMemoryMonitoring* pMemInfo = (CMemoryMonitoring *)GetBaseResource(S_BASE_CLASS_MEMORY);
	if(!pMemInfo)
		return 0;

	return pMemInfo->m_memInfo._ullPhysicalTotalRam;
}

int CSystemCommon::KillProcess(unsigned long ulProcessID)
{
	CProcessMonitoring* pMonitoring = (CProcessMonitoring *)theSystem->GetBaseResource(S_BASE_CLASS_PROCESS);
	if(!pMonitoring)
		return KILL_PROCESS_MSG_PROCESS_NULL;

	return pMonitoring->m_ProcessImpl->KillProcess(ulProcessID);
}

SHWSYSMON_PROCESS_INFO* CSystemCommon::GetProcessInfo(unsigned long ulProcessID)
{
	CProcessMonitoring* pMonitoring = (CProcessMonitoring *)theSystem->GetBaseResource(S_BASE_CLASS_PROCESS);
	if(!pMonitoring)
		return nullptr;

	return pMonitoring->m_ProcessImpl->GetProcessInfo(ulProcessID);
}

bool CSystemCommon::UpdateProcessInfo(unsigned long ulProcessID)
{
	CProcessMonitoring* pMonitoring = (CProcessMonitoring *)theSystem->GetBaseResource(S_BASE_CLASS_PROCESS);
	if(!pMonitoring)
		return false;

	return pMonitoring->m_ProcessImpl->Update(ulProcessID);
}
