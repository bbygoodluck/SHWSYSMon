#ifndef PROCESSLISTVIEWCOMMON_H_INCLUDED
#define PROCESSLISTVIEWCOMMON_H_INCLUDED

#include "../../system/process/ProcessMonitoring.h"

class CProcessListViewCommon : public wxListView
{
protected:
	enum e_ListColums
	{
		eImageName = 0,
		ePID,
	//	eUserName,
		eCPU,
		ePrivateBytes,
		eWorkingSetSize,
		eFileDescription,
		eCompanyName,
		eFullPath,
	//	eCommandLine,
	//	eSessionID,
	//	eParentProcessId,
	//	eThreads,
	//	eHandles,
	//	eCreationDate,
	//	ePagefileUsage,
	//	ePeakPagefileUsage,
	//	ePeakWorkingSetSize,
	//	eUserTime,
	//	eKernelTime,
		e_ListColums_count
	};

	enum e_MemProcessInfo {
		eMemProcessName = 0,
		eMemProcessPID,
		eMemProcessPrivateBytes,
		eMemProcessWorkingSet,
		eMemProcess_ListCount,
	};

	enum e_CPUProcessInfo {
		eProcessName = 0,
		eProcessPID,
		eProcessCPU,
		eProcess_ListCount,
	};

	typedef struct _stCONTEXT_MENU {
		int iId = -1;
		wxString strMenuName = wxT("");
		wxBitmap cmbitmap;
	} CONTEXT_MENU;

	enum eContextMenu {
		CONTEXT_MENU_KILL_PROCESS = CONTEXT_MENU_START,
		CONTEXT_MENU_VIEW_PROCESS_DETAIL,
		CONTEXT_MENU_DEL_LISTITEM
	};

public:
	CProcessListViewCommon(wxWindow* _pParent, const wxSize& sz);
	~CProcessListViewCommon ();

	virtual void update() = 0;
	virtual void AddNewProcess(unsigned long ulProcessID) = 0;
	virtual void AddNewProcess(unsigned long ulProcessID, eProcessListView _enumList);
	virtual void InitializeProcessList(eProcessListView _enumList);

public:
	static void SetProcessLists();
	static void AddListViewItem(CProcessListViewCommon* pListView);

	int GetSortingColumn() { return m_iSortingColumn; }
	bool IsAscending() { return m_bAscending; }
	void DelProcess(unsigned long ulProcess);

protected:
	static int wxCALLBACK ListCompareFunction(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
	static void DelListViewItem(unsigned long ulProcessID);

	void SetTextBackgroundColor(int iIndex, float fValue, bool IsMem = false);

protected:
	virtual void Initialize() = 0;

private:
	void CreateContextMenu();

public:
	static std::unordered_map<unsigned long, SHWSYSMON_PROCESS_INFO *> m_ProcessLists;

protected:
	int m_iSortingColumn = -1;
	bool m_bAscending = true;
	int m_iSelItemRightClick = -1;

	bool m_bSorted = false;

private:
	wxMenu* m_pContextMenu = nullptr;
	wxBitmap m_bitmapKillProcess;
	wxBitmap m_bitmapDetail;
	wxBitmap m_bitmapDelItem;

	static wxVector<CProcessListViewCommon *> m_pListViews;
private:
	void OnListContextMenu(wxCommandEvent& event);
	void OnListItemRightClick( wxListEvent& event );
	wxDECLARE_EVENT_TABLE();

};

#endif // PROCESSLISTVIEWCOMMON_H_INCLUDED
