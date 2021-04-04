#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#define EVT_MY_CUSTOM_COMMAND(custom_cmd, id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        custom_cmd, id, wxID_ANY, \
        wxCommandEventHandler(fn), \
        (wxObject *) NULL \
    ),

#define ARRSIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#if defined(_UNICODE) || defined(UNICODE)
	#define CONVSTR(x)        (x.wchar_str())
    #define CONVSTR_TO_STD(x) (x.ToStdWstring())
#else
	#define CONVSTR(x)        (x.char_str())
    #define CONVSTR_TO_STD(x) (x.ToStdString())
#endif

#define CONVSTR_TO_C_STR(x)   (x.c_str())

#ifdef __WXMSW__
	const wxString SLASH = wxT("\\");
#else
	const wxString SLASH = wxString(wxFILE_SEP_PATH);
#endif

constexpr long VIEW_STYLE = wxBORDER_NONE | wxCLIP_CHILDREN | wxCLIP_SIBLINGS | wxNO_FULL_REPAINT_ON_RESIZE;
//컨텍스트 메뉴의 갯수는 30개
constexpr int CONTEXT_MENU_START = wxID_HIGHEST + 1001;
constexpr int CONTEXT_MENU_END   = CONTEXT_MENU_START + 30;

constexpr int KILL_PROCESS_MSG_NOT_PROCESSID          = -100;
constexpr int KILL_PROCESS_MSG_PROCESS_NULL           = -101;
constexpr int KILL_PROCESS_MSG_PROCESS_TERMINATE_FAIL = -102;
constexpr int KILL_PROCESS_MSG_PROCESS_ALIVE          = -103;
constexpr int KILL_PROCESS_MSG_SUCCESS = 0;

enum _eTIMER_ID {
	CPU_TIMER_ID = 1000,
	PROCESS_TIMER_ID,
	PROCESS_MNG_TIMER_ID,
	MEMORY_TIMER_ID,
	MEMORY_SPECTRUM_TIMER_ID,
	TIMER_IDMAX
};
#ifdef __WXMSW__
typedef struct _SHWSYSMON_PROCESS_INFO {
	HANDLE _hProcess = nullptr;
	unsigned long _ulProcessID = 0x00;
	wxString _strProcessName = wxT("");
	wxString _strProcessFullPath = wxT("");
	wxString _strFileDescription = wxT("");
	wxString _strCommandLine     = wxT("");
	wxString _strCompanyName     = wxT("");
	wxString _strUserName        = wxT("");
	wxString _strDomainName      = wxT("");
	wxString _strDomainAndUser   = wxT("");

	unsigned long long _PrivateSize = 0;
	unsigned long long _WorkingSetSize = 0;

	float fCPUUsage = 0.0f;

	int iIconIndex = -1;
	int iOvelayIndex = -1;

	FILETIME _ftSysKernel;
	FILETIME _ftSysUser;
	FILETIME _ftProcKernel;
	FILETIME _ftProcUser;

	ULARGE_INTEGER _lastCPU;
	ULARGE_INTEGER _lastSysCPU;
	ULARGE_INTEGER _lastUserCPU;
} SHWSYSMON_PROCESS_INFO, *PSHWSYSMON_PROCESS_INFO;
#endif

typedef std::unordered_map<unsigned long, SHWSYSMON_PROCESS_INFO *>::const_iterator PINFO_CONST_ITERATOR;
typedef std::unordered_map<unsigned long, SHWSYSMON_PROCESS_INFO *>::iterator PINFO_ITERATOR;

typedef struct _MEMORYINFO {
	unsigned long _ullPhysicalUsedRam = 0;
	unsigned long _ullPhysicalTotalRam = 0;

	unsigned long _ullVirtualUsedRam = 0;
	unsigned long _ullVirtualTotalRam = 0;

	unsigned long _ullPageFileUsed = 0;
	unsigned long _ullPageFileTotal = 0;

} MEMORYINFO, *PMEMORYINFO;
#endif // DEFS_H_INCLUDED
