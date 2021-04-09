#ifndef PROCESSINFOIMPL_H_INCLUDED
#define PROCESSINFOIMPL_H_INCLUDED

class CProcessMonitoring;
class CProcessInfoImpl
{
	friend class CProcessMonitoring;

public:
	CProcessInfoImpl();
	~CProcessInfoImpl();

public:
	void Update();
	void Update(SHWSYSMON_PROCESS_INFO* pProcessInfo);
	bool Update(unsigned long ulProcessID);

	SHWSYSMON_PROCESS_INFO* GetProcessInfo(unsigned long ulProcessID);
	bool AddNewProcess(unsigned long ulProcessID);
	bool DelProcess(unsigned long ulProcessID);
	int KillProcess(unsigned long ulProcessID);
	void AllClearAndReload();

private:
	bool Initialize();
	void Clear();

#ifdef __WXMSW__
	IEnumWbemClassObject* ExecWMIQuery(const wxString& strQuery);
	bool SetEnableTokenPrivilege(LPCTSTR pszPrivilege);
	bool SetPrivilige(LPCTSTR lpszPrivilege, bool bEnablePrivilege = true);
	ULONGLONG SubtractTimes(const FILETIME& ftA, const FILETIME& ftB);

	void InitProcessInfo(IEnumWbemClassObject* pEnumWbemClsObj);
#endif
	void InitProcessCPU(SHWSYSMON_PROCESS_INFO* pProcessInfo);
	void UpdateProcessMemory(SHWSYSMON_PROCESS_INFO* pProcessInfo);
	void UpdateProcessCPU(SHWSYSMON_PROCESS_INFO* pProcessInfo);

protected:
	std::unordered_map<unsigned long, SHWSYSMON_PROCESS_INFO *> m_mapProcesses;

private:
#ifdef __WXMSW__
//	HRESULT m_hResult = S_OK;
//	IWbemLocator* m_pLocator = nullptr;
//	IWbemServices* m_pServices = nullptr;
#endif
};

#endif // PROCESSINFOIMPL_H_INCLUDED
