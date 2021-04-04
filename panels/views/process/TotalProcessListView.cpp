#include "../../../ginc.h"
#include "TotalProcessListView.h"
#include "../../PanelProcess.h"

CTotalProcessListView::CTotalProcessListView(wxWindow* _parent)
	: CProcessListViewCommon(_parent, wxDefaultSize)
	, m_pParent(_parent)
{
	Initialize();
}

CTotalProcessListView::~CTotalProcessListView()
{

}

void CTotalProcessListView::Initialize()
{
	const COLUMN_INFO m_arrColumnInfo[] =
	{                                                                            // Win32_Process property
		// -----------------------------------------------------------------------------------------------
		{eImageName, _T("Process Name"), wxLIST_FORMAT_LEFT, 280},                   // Caption
		{ePID, _T("PID"), wxLIST_FORMAT_RIGHT, 70},                                  // PID
	//	{eUserName, _T("UserName"), wxLIST_FORMAT_RIGHT, 230},                       // UseName
		{eCPU, _T("CPU"), wxLIST_FORMAT_RIGHT, 70},                                  // CPU
		{ePrivateBytes, _T("Private Bytes"), wxLIST_FORMAT_RIGHT, 140},              // PrivateBytes
		{eWorkingSetSize, _T("Working Set"), wxLIST_FORMAT_RIGHT, 140},              // WorkingSetSize
		{eFileDescription, _T("FileDescription"), wxLIST_FORMAT_RIGHT, 320},         // FileDescription
		{eCompanyName, _T("Company"), wxLIST_FORMAT_RIGHT, 200},                     // CompanyName
		{eFullPath, _T("Path"), wxLIST_FORMAT_LEFT, 590},                            // WorkingSetSize
	//	{eCommandLine, _T("Command Line"), wxLIST_FORMAT_LEFT, 450},                 // CommandLine
	//	{eSessionID, _T("Session ID"), wxLIST_FORMAT_RIGHT, 100},                    // SessionId
	//	{eParentProcessId, _T("ParentPId"), wxLIST_FORMAT_RIGHT, 90},                // ParentProcessId
	//	{eThreads, _T("Threads"), wxLIST_FORMAT_RIGHT, 80},                          // ThreadCount
	//	{eHandles, _T("Handles"), wxLIST_FORMAT_RIGHT, 80},                          // HandleCount
	//	{eCreationDate, _T("Creation Date"), wxLIST_FORMAT_LEFT, 100},               // CreationDate
	//	{ePagefileUsage, _T("Pagefile Usage"), wxLIST_FORMAT_RIGHT, 120},            // PageFileUsage
	//	{ePeakPagefileUsage, _T("Peak Pagefile Usage"), wxLIST_FORMAT_RIGHT, 120},   // PeakPageFileUsage
	//	{ePeakWorkingSetSize, _T("Peak Working Set Size"), wxLIST_FORMAT_RIGHT, 150},// PeakWorkingSetSize
	//	{eUserTime, _T("User Time"), wxLIST_FORMAT_RIGHT, 100},                      // UserModeTime
	//	{eKernelTime, _T("User Time"), wxLIST_FORMAT_RIGHT, 100},                    // KernelModeTime
	};

    for(int nColumn = 0; nColumn < e_ListColums_count; nColumn++)
    {
        COLUMN_INFO ci = m_arrColumnInfo[nColumn];

        wxListItem lstItemInfo;
		lstItemInfo.SetText(ci.szCaption);
		lstItemInfo.SetAlign(ci.cfFormat);
		lstItemInfo.SetWidth(ci.nWidth);

		this->InsertColumn(nColumn, lstItemInfo);
    }
}

void CTotalProcessListView::update()
{
	wxString strPrivateSize(wxT(""));
	wxString strWorkingSet(wxT(""));
	wxString strCPUUsage(wxT(""));

	int iItemCount = this->GetItemCount();
	for(int iIndex = 0; iIndex < iItemCount; iIndex++)
	{
		wxString strProcessID = this->GetItemText(iIndex, 1);

		unsigned long _ulProcessID = 0;
		strProcessID.ToCULong(&_ulProcessID);
		PINFO_CONST_ITERATOR fIter = m_ProcessLists.find(_ulProcessID);
		if(fIter == m_ProcessLists.end())
			continue;

		SHWSYSMON_PROCESS_INFO* pProcessInfo = fIter->second;//theSystem->GetProcessInfo(_ulProcessID);
		strCPUUsage = wxString::Format(wxT("%.2f%"), pProcessInfo->fCPUUsage);
		SetItem(iIndex, eCPU, strCPUUsage);

		strPrivateSize = wxT("");
		theUtility->SetComma(wxString::Format(wxT("%.0f"), (float)pProcessInfo->_PrivateSize / (1024.0f)), strPrivateSize);

		strWorkingSet = wxT("");
		theUtility->SetComma(wxString::Format(wxT("%.0f"), (float)pProcessInfo->_WorkingSetSize / (1024.0f)), strWorkingSet);

		SetItem(iIndex, ePrivateBytes, strPrivateSize + wxT(" KB"));
		SetItem(iIndex, eWorkingSetSize, strWorkingSet + wxT(" KB"));

		SetTextBackgroundColor(iIndex, pProcessInfo->fCPUUsage);
		if(!m_bSorted)
			this->SetItemData(iIndex, iIndex);
	}

	if(!m_bSorted)
	{
		this->SortItems(CProcessListViewCommon::ListCompareFunction, (LPARAM)this);
		m_bSorted = true;
	}
}

void CTotalProcessListView::SetItemSelected(const wxString& strSelProcessID)
{
	SetItemState(-1, 0, wxLIST_STATE_SELECTED);

	int iItemCount = this->GetItemCount();
	int iSelIndex = -1;
	for(int iIndex = 0; iIndex < iItemCount; iIndex++)
	{
		wxString strProcessID = this->GetItemText(iIndex, ePID);
		if(strSelProcessID.CmpNoCase(strProcessID) == 0)
		{
			iSelIndex = iIndex;
			break;
		}
	}

	SetItemState(iSelIndex, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
	SetItemState(iSelIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	EnsureVisible(iSelIndex);
}

void CTotalProcessListView::AddNewProcess(unsigned long ulProcessID)
{
	CProcessListViewCommon::AddNewProcess(ulProcessID, eTotalList);
}
