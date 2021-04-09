#include "../../ginc.h"
#include "ProcessInfoImpl.h"
#include "ProcessMonitoring.h"
#include "FileVersionInfo.h"
/*
 * SE_xxxx 선언은 <ddk/wdm.h> 헤더에 적용되어 있으나
 * wdm.h 헤더를 include하게되면 컴파일 에러가 발생함
 * 하여 SE_xxxx 선언을 직접 가져와 처리함
 */
#define SE_MIN_WELL_KNOWN_PRIVILEGE         2
#define SE_CREATE_TOKEN_PRIVILEGE           2
#define SE_ASSIGNPRIMARYTOKEN_PRIVILEGE     3
#define SE_LOCK_MEMORY_PRIVILEGE            4
#define SE_INCREASE_QUOTA_PRIVILEGE         5
#define SE_MACHINE_ACCOUNT_PRIVILEGE        6
#define SE_TCB_PRIVILEGE                    7
#define SE_SECURITY_PRIVILEGE               8
#define SE_TAKE_OWNERSHIP_PRIVILEGE         9
#define SE_LOAD_DRIVER_PRIVILEGE            10
#define SE_SYSTEM_PROFILE_PRIVILEGE         11
#define SE_SYSTEMTIME_PRIVILEGE             12
#define SE_PROF_SINGLE_PROCESS_PRIVILEGE    13
#define SE_INC_BASE_PRIORITY_PRIVILEGE      14
#define SE_CREATE_PAGEFILE_PRIVILEGE        15
#define SE_CREATE_PERMANENT_PRIVILEGE       16
#define SE_BACKUP_PRIVILEGE                 17
#define SE_RESTORE_PRIVILEGE                18
#define SE_SHUTDOWN_PRIVILEGE               19
#define SE_DEBUG_PRIVILEGE                  20
#define SE_AUDIT_PRIVILEGE                  21
#define SE_SYSTEM_ENVIRONMENT_PRIVILEGE     22
#define SE_CHANGE_NOTIFY_PRIVILEGE          23
#define SE_REMOTE_SHUTDOWN_PRIVILEGE        24
#define SE_UNDOCK_PRIVILEGE                 25
#define SE_SYNC_AGENT_PRIVILEGE             26
#define SE_ENABLE_DELEGATION_PRIVILEGE      27
#define SE_MANAGE_VOLUME_PRIVILEGE          28
#define SE_IMPERSONATE_PRIVILEGE            29
#define SE_CREATE_GLOBAL_PRIVILEGE          30
#define SE_TRUSTED_CREDMAN_ACCESS_PRIVILEGE 31
#define SE_RELABEL_PRIVILEGE                32
#define SE_INC_WORKING_SET_PRIVILEGE        33
#define SE_TIME_ZONE_PRIVILEGE              34
#define SE_CREATE_SYMBOLIC_LINK_PRIVILEGE   35
#define SE_MAX_WELL_KNOWN_PRIVILEGE         SE_CREATE_SYMBOLIC_LINK_PRIVILEGE

CProcessInfoImpl::CProcessInfoImpl()
{
	Initialize();
}

CProcessInfoImpl::~CProcessInfoImpl()
{
	Clear();
}

bool CProcessInfoImpl::Initialize()
{
#ifdef __WXMSW__
//	SetPrivilige(SE_ASSIGNPRIMARYTOKEN_NAME);
	SetPrivilige(SE_DEBUG_NAME);
	//Windows process  정보 읽기
	IEnumWbemClassObject* pEnumWbemClsObj = ExecWMIQuery(wxString(wxT("SELECT * FROM Win32_Process")));
	if(pEnumWbemClsObj == nullptr)
		return false;

	InitProcessInfo(pEnumWbemClsObj);
	SafeRelease(&pEnumWbemClsObj);

#endif
    return true;
}

void CProcessInfoImpl::Clear()
{
	std::unordered_map<unsigned long, SHWSYSMON_PROCESS_INFO *>::iterator fIter;
	for (fIter = m_mapProcesses.begin(); fIter != m_mapProcesses.end(); ++fIter)
	{
		SHWSYSMON_PROCESS_INFO* pProcessInfo = fIter->second;
#ifdef __WXMSW__
		CloseHandle(pProcessInfo->_hProcess);
#endif // __WXMSW__
		delete pProcessInfo;
	}

	m_mapProcesses.clear();
}

#ifdef __WXMSW__
bool CProcessInfoImpl::SetEnableTokenPrivilege(LPCTSTR pszPrivilege)
{
	HANDLE hToken = 0;
	if (!OpenProcessToken(GetCurrentProcess(),
						  TOKEN_ADJUST_PRIVILEGES |
						  TOKEN_QUERY, &hToken))
	{
		theUtility->ShowMessage(wxString::Format(wxT("OpenProcessToken function error=%0x"), GetLastError()), wxOK | wxICON_ERROR);
        return false;
	}

	int iArrayPrivileges[] = {
		SE_DEBUG_PRIVILEGE,
		SE_INC_BASE_PRIORITY_PRIVILEGE,
		SE_INC_WORKING_SET_PRIVILEGE,
		SE_LOAD_DRIVER_PRIVILEGE,
	//	SE_PROF_SINGLE_PROCESS_PRIVILEGE,
	//	SE_BACKUP_PRIVILEGE,
	//	SE_RESTORE_PRIVILEGE,
	//	SE_SHUTDOWN_PRIVILEGE,
		SE_TAKE_OWNERSHIP_PRIVILEGE,
	//	SE_ASSIGNPRIMARYTOKEN_PRIVILEGE,
	//	SE_TCB_PRIVILEGE,
	//	SE_MACHINE_ACCOUNT_PRIVILEGE,
	//	SE_INCREASE_QUOTA_PRIVILEGE,
	//	SE_TRUSTED_CREDMAN_ACCESS_PRIVILEGE,
	//	SE_SYSTEM_PROFILE_PRIVILEGE,
		SE_SYSTEMTIME_PRIVILEGE
	};

	constexpr int iPrivilegeCount = WXSIZEOF(iArrayPrivileges);
	CHAR privilegesBuffer[FIELD_OFFSET(TOKEN_PRIVILEGES, Privileges) + sizeof(LUID_AND_ATTRIBUTES) * iPrivilegeCount];
	PTOKEN_PRIVILEGES privileges;
	ULONG i;

	privileges = (PTOKEN_PRIVILEGES)privilegesBuffer;
	privileges->PrivilegeCount = iPrivilegeCount;

	for (i = 0; i < privileges->PrivilegeCount; i++)
	{
		privileges->Privileges[i].Attributes = SE_PRIVILEGE_ENABLED;
		privileges->Privileges[i].Luid.HighPart = 0;
		privileges->Privileges[i].Luid.LowPart = iArrayPrivileges[i];
	}

	if(!AdjustTokenPrivileges(hToken,              //HANDLE
	                      FALSE,                   //BOOL(DisableAllPrivileges
						  privileges,              //NewState
						  0,                       //BufferLength
						  (PTOKEN_PRIVILEGES)NULL, //PreviousState
						  NULL))                   //ReturnLength
	{
		if (GetLastError() != ERROR_NOT_ALL_ASSIGNED)
			theUtility->ShowMessage(wxString::Format(wxT("AdjustTokenPrivileges error=%0x"), GetLastError()), wxOK | wxICON_ERROR);
	}

	return true;
}

bool CProcessInfoImpl::SetPrivilige(LPCTSTR lpszPrivilege, bool bEnablePrivilege)
{
	HANDLE hToken = 0;
	if (!OpenProcessToken(GetCurrentProcess(),
						  TOKEN_ADJUST_PRIVILEGES |
						  TOKEN_QUERY, &hToken))
	{
		theUtility->ShowMessage(wxString::Format(wxT("OpenProcessToken function error=%0x"), GetLastError()), wxOK | wxICON_ERROR);
        return false;
	}

	TOKEN_PRIVILEGES tp;
	// Used by local system to identify the privilege
	LUID luid;

	if(!LookupPrivilegeValue(NULL,          // lookup privilege on local system
							lpszPrivilege,  // privilege to lookup
							&luid))         // receives LUID of privilege

	{
		theUtility->ShowMessage(wxString::Format(wxT("LLookupPrivilegeValue() failed, error : %0x\n"), GetLastError()), wxOK | wxICON_ERROR);
		return false;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;

	// If TRUE, enable
	if(bEnablePrivilege) tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else tp.Privileges[0].Attributes = 0; // else, disable

	if(!AdjustTokenPrivileges(hToken,
							  FALSE,
							  &tp,
							  sizeof(TOKEN_PRIVILEGES),
							  (PTOKEN_PRIVILEGES) NULL,
							  (PDWORD) NULL))
	{
		theUtility->ShowMessage(wxString::Format(wxT("AdjustTokenPrivileges() failed, error : %0x\n"), GetLastError()), wxOK | wxICON_ERROR);
		return false;
	}

	return true;
}

IEnumWbemClassObject* CProcessInfoImpl::ExecWMIQuery(const wxString& strQuery)
{
	if(!_gServices)
	{
		theUtility->ShowMessage(wxT("IWbemServices is null"), wxOK | wxICON_ERROR);
		return nullptr;
	}

	HRESULT hResult;
	IEnumWbemClassObject *pEnumWbemClsObj  = nullptr;
	hResult = _gServices->ExecQuery(_TEXT("WQL"), strQuery.wchar_str(), WBEM_FLAG_FORWARD_ONLY, nullptr, &pEnumWbemClsObj);
	if (FAILED(hResult))
	{
		theUtility->ShowMessage(wxString::Format(wxT("ExecQuery() is failed Error = %0x"), GetLastError()), wxOK | wxICON_ERROR);
		return nullptr;
	}

	return pEnumWbemClsObj;
}

void CProcessInfoImpl::InitProcessInfo(IEnumWbemClassObject* pEnumWbemClsObj)
{
	HRESULT hResult;
	IWbemClassObject *pWebClsObjResult = nullptr;
	ULONG returnedCount = 0;

	while((hResult = pEnumWbemClsObj->Next(WBEM_INFINITE, 1, &pWebClsObjResult, &returnedCount)) == S_OK)
	{
		VARIANT vProcessId;
		VARIANT vProcessName;
		VARIANT vCommandLine;
		VARIANT vExecutablePath;

		// access the properties
		hResult = pWebClsObjResult->Get(L"ProcessId",      0, &vProcessId,      0, 0);
		hResult = pWebClsObjResult->Get(L"Name",           0, &vProcessName,    0, 0);
		hResult = pWebClsObjResult->Get(L"CommandLine",    0, &vCommandLine,    0, 0);
		hResult = pWebClsObjResult->Get(L"ExecutablePath", 0, &vExecutablePath, 0, 0);

		SHWSYSMON_PROCESS_INFO* pProcessInfo = new SHWSYSMON_PROCESS_INFO;

		pProcessInfo->_ulProcessID = (unsigned long)vProcessId.lVal;
		pProcessInfo->_strProcessName = vProcessName.bstrVal;
		pProcessInfo->_strCommandLine = vCommandLine.bstrVal;
		pProcessInfo->_strProcessFullPath = vExecutablePath.bstrVal;
		pProcessInfo->fCPUUsage = 0.0f;
		pProcessInfo->_PrivateSize = 0;
		pProcessInfo->_WorkingSetSize = 0;

		pProcessInfo->_hProcess = OpenProcess(MAXIMUM_ALLOWED/*PROCESS_QUERY_INFORMATION | PROCESS_VM_READ*/, FALSE, pProcessInfo->_ulProcessID);
#ifndef NDEBUG
		DWORD dwError = GetLastError();
		if(dwError == ERROR_ACCESS_DENIED)
			theUtility->LogDisplay(wxT("ERROR_ACCESS_DENIED : ") +pProcessInfo->_strProcessName);
#endif // NDEBUG

		InitProcessCPU(pProcessInfo);
		UpdateProcessMemory(pProcessInfo);

		theUtility->GetIconIndexFromImageList(pProcessInfo->_strProcessFullPath, pProcessInfo->iIconIndex, pProcessInfo->iOvelayIndex);

		CFileVersionInfo data;
		if(data.Create(pProcessInfo->_strProcessFullPath))
		{
			pProcessInfo->_strFileDescription = data.GetFileDescription();
			pProcessInfo->_strCompanyName = data.GetCompanyName();
		}

		//데이터 저장
		std::unordered_map<unsigned long, SHWSYSMON_PROCESS_INFO *>::value_type val(pProcessInfo->_ulProcessID, pProcessInfo);
		m_mapProcesses.insert(val);

		SafeRelease(&pWebClsObjResult);
	}
}
#endif

bool CProcessInfoImpl::AddNewProcess(unsigned long ulProcessID)
{
	PINFO_CONST_ITERATOR iter = m_mapProcesses.find(ulProcessID);
	if(iter != m_mapProcesses.end())
		return false;

	wxString strQuery = wxString::Format(wxT("select * from win32_process where processid=%u"), ulProcessID);

	IEnumWbemClassObject* pEnumWbemClsObj = ExecWMIQuery(strQuery);
	if(pEnumWbemClsObj == nullptr)
		return false;

	wxSleep(0.3);
	InitProcessInfo(pEnumWbemClsObj);
	SafeRelease(&pEnumWbemClsObj);

	return true;
}

bool CProcessInfoImpl::DelProcess(unsigned long ulProcessID)
{
	PINFO_CONST_ITERATOR iter = m_mapProcesses.find(ulProcessID);
	if(iter == m_mapProcesses.end())
		return false;

	SHWSYSMON_PROCESS_INFO* pProcess = iter->second;
	CloseHandle(pProcess->_hProcess);

	m_mapProcesses.erase(iter);
	return true;
}

int CProcessInfoImpl::KillProcess(unsigned long ulProcessID)
{
	PINFO_CONST_ITERATOR iter = m_mapProcesses.find(ulProcessID);
	if(iter == m_mapProcesses.end())
		return KILL_PROCESS_MSG_PROCESS_NULL;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, ulProcessID);

	if(hProcess != NULL)
	{
		bool bTerminate = ::TerminateProcess(hProcess, -1);
		if(!bTerminate)
			return KILL_PROCESS_MSG_PROCESS_TERMINATE_FAIL;

		unsigned long ulRetCode = 0;
		GetExitCodeProcess(hProcess, &ulRetCode);

		if (ulRetCode == STILL_ACTIVE)
			return KILL_PROCESS_MSG_PROCESS_ALIVE;

		return KILL_PROCESS_MSG_SUCCESS;
	}

	return KILL_PROCESS_MSG_PROCESS_NULL;
}

void CProcessInfoImpl::Update()
{
#ifdef __WXMSW__
	PINFO_ITERATOR iterStart = m_mapProcesses.begin();
	PINFO_ITERATOR iterEnd = m_mapProcesses.end();

	while(iterStart != iterEnd)
	{
		SHWSYSMON_PROCESS_INFO* pProcessInfo = iterStart->second;
		UpdateProcessMemory(pProcessInfo);
		UpdateProcessCPU(pProcessInfo);

		++iterStart;
	}
#endif
}

void CProcessInfoImpl::Update(SHWSYSMON_PROCESS_INFO* pProcessInfo)
{
	UpdateProcessMemory(pProcessInfo);
	UpdateProcessCPU(pProcessInfo);
}

bool CProcessInfoImpl::Update(unsigned long ulProcessID)
{
	PINFO_CONST_ITERATOR iter = m_mapProcesses.find(ulProcessID);
	if(iter == m_mapProcesses.end())
		return false;

	SHWSYSMON_PROCESS_INFO* pProcessInfo = iter->second;
	theUtility->GetIconIndexFromImageList(pProcessInfo->_strProcessFullPath, pProcessInfo->iIconIndex, pProcessInfo->iOvelayIndex);

	CFileVersionInfo data;
	bool bFileVersion = data.Create(pProcessInfo->_strProcessFullPath);
	if(!bFileVersion)
		return false;

	pProcessInfo->_strFileDescription = data.GetFileDescription();
	pProcessInfo->_strCompanyName = data.GetCompanyName();

	return true;
}

void CProcessInfoImpl::InitProcessCPU(SHWSYSMON_PROCESS_INFO* pProcessInfo)
{
	if(pProcessInfo->_hProcess == NULL)
		return;

	//cpu 사용시간
	FILETIME ftime, fsys, fuser;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&pProcessInfo->_lastCPU, &ftime, sizeof(FILETIME));

	GetProcessTimes(pProcessInfo->_hProcess, &ftime, &ftime, &fsys, &fuser);
	memcpy(&pProcessInfo->_lastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&pProcessInfo->_lastUserCPU, &fuser, sizeof(FILETIME));
}

void CProcessInfoImpl::UpdateProcessMemory(SHWSYSMON_PROCESS_INFO* pProcessInfo)
{
#ifdef __WXMSW__
	if(pProcessInfo->_hProcess == NULL)
		return;

	PROCESS_MEMORY_COUNTERS_EX pmc;
	BOOL bInfo = GetProcessMemoryInfo(pProcessInfo->_hProcess, (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));

	if (!bInfo)
	{
		pProcessInfo->_PrivateSize = 0;
		pProcessInfo->_WorkingSetSize = 0;
		return;
	}

	pProcessInfo->_PrivateSize = pmc.PrivateUsage;
	pProcessInfo->_WorkingSetSize = pmc.WorkingSetSize;
#endif
}

void CProcessInfoImpl::UpdateProcessCPU(SHWSYSMON_PROCESS_INFO* pProcessInfo)
{
#ifdef __WXMSW__
	if(pProcessInfo->_hProcess == NULL)
		return;

	FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(pProcessInfo->_hProcess, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));

    percent = (sys.QuadPart - pProcessInfo->_lastSysCPU.QuadPart) + (user.QuadPart - pProcessInfo->_lastUserCPU.QuadPart);
    percent /= (now.QuadPart - pProcessInfo->_lastCPU.QuadPart);
    percent /= theSystem->GetCPUCoreCount();

    memcpy(&pProcessInfo->_lastCPU , &now, sizeof(FILETIME));
    memcpy(&pProcessInfo->_lastUserCPU, &user, sizeof(FILETIME));
    memcpy(&pProcessInfo->_lastSysCPU, &sys, sizeof(FILETIME));

	pProcessInfo->fCPUUsage = percent * 100.0f;
#endif
}

#ifdef __WXMSW__
ULONGLONG CProcessInfoImpl::SubtractTimes(const FILETIME& ftA, const FILETIME& ftB)
{
	LARGE_INTEGER a, b;
	a.LowPart = ftA.dwLowDateTime;
	a.HighPart = ftA.dwHighDateTime;

	b.LowPart = ftB.dwLowDateTime;
	b.HighPart = ftB.dwHighDateTime;
	return a.QuadPart - b.QuadPart;
}
#endif // __WXMSW__
SHWSYSMON_PROCESS_INFO* CProcessInfoImpl::GetProcessInfo(unsigned long ulProcessID)
{
	PINFO_ITERATOR iter = m_mapProcesses.find(ulProcessID);
	if(iter == m_mapProcesses.end())
		return nullptr;

	return iter->second;
}

void CProcessInfoImpl::AllClearAndReload()
{
	Clear();
	Initialize();
}
