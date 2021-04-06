#include "../../../ginc.h"
#include "MemProcessListView.h"
#include "../../PanelProcess.h"

CMemProcessListView::CMemProcessListView(wxWindow* _parent, const wxSize& sz)
	: CProcessListViewCommon(_parent, sz)
	, m_pParent(_parent)
{
	Initialize();
}

CMemProcessListView::~CMemProcessListView()
{
	this->Disconnect(wxEVT_LIST_ITEM_SELECTED, wxListEventHandler(CMemProcessListView::OnItemSelected), NULL, this);
}

void CMemProcessListView::Initialize()
{
	const COLUMN_INFO arrColumnInfo[] =
	{                                                                            // Win32_Process property
		// -----------------------------------------------------------------------------------------------
		{eMemProcessName, _T("Process Name"), wxLIST_FORMAT_LEFT, 180},          // Caption
		{eMemProcessPID, _T("PID"), wxLIST_FORMAT_RIGHT, 70},                    // PID
		{eMemProcessPrivateBytes, _T("PrivateBytes"), wxLIST_FORMAT_RIGHT, 130}, // PrivateBytes
		{eMemProcessWorkingSet, _T("Working Set"), wxLIST_FORMAT_RIGHT, 130},    // WorkingSet
	};

    for(int nColumn = 0; nColumn < eMemProcess_ListCount; nColumn++)
    {
        COLUMN_INFO ci = arrColumnInfo[nColumn];

        wxListItem lstItemInfo;
		lstItemInfo.SetText(ci.szCaption);
		lstItemInfo.SetAlign(ci.cfFormat);
		lstItemInfo.SetWidth(ci.nWidth);

		this->InsertColumn(nColumn, lstItemInfo);
    }

    this->Connect(wxEVT_LIST_ITEM_SELECTED, wxListEventHandler(CMemProcessListView::OnItemSelected), NULL, this);
}

void CMemProcessListView::update()
{
	wxString strPrivateSize(wxT(""));
	wxString strWorkingSet(wxT(""));

	float fPrivateSize = 0.0f;
	float fWorkingSize = 0.0f;

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
//		if(pProcessInfo == nullptr)
//			continue;

		fPrivateSize = (float)(pProcessInfo->_PrivateSize / (1024.0f));
		strPrivateSize = wxT("");
		theUtility->SetComma(wxString::Format(wxT("%.0f"), fPrivateSize), strPrivateSize);

		fWorkingSize = (float)(pProcessInfo->_WorkingSetSize / (1024.0f));
		strWorkingSet = wxT("");
		theUtility->SetComma(wxString::Format(wxT("%.0f"), fWorkingSize), strWorkingSet);

		SetItem(iIndex, eMemProcessPrivateBytes, strPrivateSize + wxT(" KB"));
		SetItem(iIndex, eMemProcessWorkingSet, strWorkingSet + wxT(" KB"));

		SetTextBackgroundColor(iIndex, fPrivateSize, true);
		this->SetItemData(iIndex, (LPARAM)fPrivateSize);
	}

	this->SortItems(CProcessListViewCommon::ListCompareFunction, 0);
}

void CMemProcessListView::OnItemSelected(wxListEvent& event)
{
	int iSel = event.GetIndex();
	wxString strProcessID = GetItemText(iSel, 1);

	((CPanelProcess *)m_pParent)->SetSelectedItem(strProcessID);
}

void CMemProcessListView::AddNewProcess(unsigned long ulProcessID)
{
	CProcessListViewCommon::AddNewProcess(ulProcessID, eMemoryList);
}

void CMemProcessListView::reload()
{
	CProcessListViewCommon::InitializeProcessList(eMemoryList);
}
