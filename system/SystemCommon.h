#ifndef SYSTEMCOMMON_H_INCLUDED
#define SYSTEMCOMMON_H_INCLUDED

class CResourceCommon;
class CSystemCommon
{
	typedef std::unordered_map<int, CResourceCommon *>::const_iterator constResourceIter;
	typedef std::unordered_map<int, CResourceCommon *>::iterator ResourceIter;
private:
	CSystemCommon() {};

public:
	~CSystemCommon() {};
	static CSystemCommon* Get();

	const unsigned long GetCPUUsage();
	int GetCPUCoreCount();

	wxString GetProcessorInfo();
	SHWSYSMON_PROCESS_INFO* GetProcessInfo(unsigned long ulProcessID);
	PINFO_CONST_ITERATOR GetProcessMapConstBeing();
	PINFO_CONST_ITERATOR GetProcessMapConstEnd();

	void SetMonitoringResource(CResourceCommon* pBaseInfo, int iBaseInfoIndex);
	CResourceCommon* GetBaseResource(int iBaseInfoIndex);

	MEMORYINFO* GetMemoryInfo();
	unsigned long GetTotalMemory();

	int KillProcess(unsigned long ulProcessID);
	bool UpdateProcessInfo(unsigned long ulProcessID);

private:

private:
	//singleton pattern 구현을 위한 인스턴스
	static std::unique_ptr<CSystemCommon> m_pInstance;
	std::unordered_map<int, CResourceCommon *> m_mapBaseInfo;
};

#endif // SYSTEMCOMMON_H_INCLUDED
