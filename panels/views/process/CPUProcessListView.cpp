#include "../../../ginc.h"
#include "CPUProcessListView.h"
#include "../../PanelProcess.h"

CCPUProcessListView::CCPUProcessListView(wxWindow* _parent, const wxSize& sz)
	: CProcessListViewCommon(_parent, sz)
	, m_pParent(_parent)
{
	Initialize();
}

CCPUProcessListView::~CCPUProcessListView()
{

}

void CCPUProcessListView::Initialize()
{
	const COLUMN_INFO arrColumnInfo[] =
	{                                                                            // Win32_Process property
		// -----------------------------------------------------------------------------------------------
		{eProcessName, _T("Process Name"), wxLIST_FORMAT_LEFT, 320},             // Caption
		{eProcessPID, _T("PID"), wxLIST_FORMAT_RIGHT, 70},                       // PID
		{eProcessCPU, _T("CPU"), wxLIST_FORMAT_RIGHT, 120},                      // CPU
	};

    for(int nColumn = 0; nColumn < eProcess_ListCount; nColumn++)
    {
        COLUMN_INFO ci = arrColumnInfo[nColumn];

        wxListItem lstItemInfo;
		lstItemInfo.SetText(ci.szCaption);
		lstItemInfo.SetAlign(ci.cfFormat);
		lstItemInfo.SetWidth(ci.nWidth);

		this->InsertColumn(nColumn, lstItemInfo);
    }
}

void CCPUProcessListView::update()
{
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
		SetItem(iIndex, eProcessCPU, strCPUUsage);

		this->SetItemData(iIndex, (LPARAM)(pProcessInfo->fCPUUsage * 100.0f));
		SetTextBackgroundColor(iIndex, pProcessInfo->fCPUUsage);
	}

	this->SortItems(CProcessListViewCommon::ListCompareFunction, 0);
}

void CCPUProcessListView::AddNewProcess(unsigned long ulProcessID)
{
	CProcessListViewCommon::AddNewProcess(ulProcessID, eCPUList);
}

