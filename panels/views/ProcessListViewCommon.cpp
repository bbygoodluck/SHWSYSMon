#include "../../ginc.h"
#include "ProcessListViewCommon.h"
#include "../../dialog/DlgProcessDetail.h"

wxVector<CProcessListViewCommon *> CProcessListViewCommon::m_pListViews;

wxBEGIN_EVENT_TABLE(CProcessListViewCommon, wxListCtrl)
	EVT_MENU_RANGE(CONTEXT_MENU_START, CONTEXT_MENU_END, CProcessListViewCommon::OnListContextMenu)
wxEND_EVENT_TABLE()

CProcessListViewCommon::CProcessListViewCommon(wxWindow* _pParent, const wxSize& sz)
	: wxListView(_pParent, wxID_ANY, wxDefaultPosition, sz)
{
	m_bitmapKillProcess = wxArtProvider::GetBitmap(wxART_QUIT, wxART_MENU, wxSize(16, 16));
	m_bitmapDetail = wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU, wxSize(16, 16));
	m_bitmapDelItem = wxArtProvider::GetBitmap(wxART_MINUS, wxART_MENU, wxSize(16, 16));
	m_bitmapUpdate = wxArtProvider::GetBitmap(wxART_FIND_AND_REPLACE, wxART_MENU, wxSize(16, 16));

	this->SetTextColour(wxColour(60, 60, 60));
	this->SetBackgroundColour(wxColour(240, 240, 240));
	wxFont font(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Fixedsys"));
	this->SetFont(font);//wxSystemSettings::GetFont(wxSYS_SYSTEM_FIXED_FONT));
#ifdef __WXMSW__
	ListView_SetImageList(GetHWND(), _gImageList, LVSIL_SMALL);
#else
	this->SetImageList(_gImageList);
#endif

	this->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( CProcessListViewCommon::OnListItemRightClick ), NULL, this );
	CreateContextMenu();

}

CProcessListViewCommon::~CProcessListViewCommon()
{
	if(m_pContextMenu)
		delete m_pContextMenu;

	this->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( CProcessListViewCommon::OnListItemRightClick ), NULL, this );
}

int wxCALLBACK CProcessListViewCommon::ListCompareFunction(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
	CProcessListViewCommon* pListCtrl = (CProcessListViewCommon *)sortData;
	if(pListCtrl == nullptr)
	{
		if (item1 < item2)
			return 1;

		if (item1 > item2)
			return -1;

		return 0;
	}

	int iSortingColumn = pListCtrl->GetSortingColumn();
	bool bAscending = pListCtrl->IsAscending();

	wxString str1 = pListCtrl->GetItemText(item1, iSortingColumn);
	wxString str2 = pListCtrl->GetItemText(item2, iSortingColumn);

	int iRet = 0;
	if(str1.CmpNoCase(str2) > 0)
	{
		if(bAscending)
			iRet = 1;
		else
			iRet = -1;
	}

	if(str1.CmpNoCase(str2) < 0)
	{
		if(bAscending)
			iRet = -1;
		else
			iRet = 1;
	}

	return iRet;
}

void CProcessListViewCommon::InitializeProcessList(eProcessListView _enumList)
{
	PINFO_CONST_ITERATOR cStartIter = theSystem->GetProcessMapConstBeing();
	PINFO_CONST_ITERATOR cEndIter = theSystem->GetProcessMapConstEnd();

	int iIndex = 0;
	if(_enumList == eTotalList)
	{
		wxString strPrivateSize(wxT(""));
		wxString strWorkingSet(wxT(""));

		while(cStartIter != cEndIter)
		{
			SHWSYSMON_PROCESS_INFO* pProcessInfo = cStartIter->second;
			InsertItem(iIndex, pProcessInfo->_strProcessName);
			SetItem(iIndex, ePID, wxString::Format(wxT("%u"), pProcessInfo->_ulProcessID));

			SetItem(iIndex, eCPU, wxT("0.0%"));
			SetItem(iIndex, ePrivateBytes, strPrivateSize + wxT("0 KB"));
			SetItem(iIndex, eWorkingSetSize, strWorkingSet + wxT("0 KB"));

			SetItem(iIndex, eFileDescription, pProcessInfo->_strFileDescription);
			SetItem(iIndex, eCompanyName, pProcessInfo->_strCompanyName);
			SetItem(iIndex, eFullPath, pProcessInfo->_strProcessFullPath);

			SetItemImage(iIndex, pProcessInfo->iIconIndex);
			this->SetItemData(iIndex, iIndex);

			iIndex++;
			++cStartIter;
		}

		this->m_bAscending = true;
		this->m_iSortingColumn = 0;

		this->SortItems(CProcessListViewCommon::ListCompareFunction, (LPARAM)this);
		m_bSorted = true;
	}

	if(_enumList == eCPUList)
	{
		iIndex = 0;
		while(cStartIter != cEndIter)
		{
			SHWSYSMON_PROCESS_INFO* pProcessInfo = cStartIter->second;
			InsertItem(iIndex, pProcessInfo->_strProcessName);
			SetItem(iIndex, eProcessPID, wxString::Format(wxT("%u"), pProcessInfo->_ulProcessID));

			SetItem(iIndex, eProcessCPU, wxT("0.0%"));

			SetItemImage(iIndex, pProcessInfo->iIconIndex);
			this->SetItemData(iIndex, iIndex);

			iIndex++;
			++cStartIter;
		}
	}

	if(_enumList == eMemoryList)
	{
		iIndex = 0;
		while(cStartIter != cEndIter)
		{
			SHWSYSMON_PROCESS_INFO* pProcessInfo = cStartIter->second;
			InsertItem(iIndex, pProcessInfo->_strProcessName);
			SetItem(iIndex, eMemProcessPID, wxString::Format(wxT("%u"), pProcessInfo->_ulProcessID));

			SetItem(iIndex, eMemProcessPrivateBytes, wxT("0 KB"));
			SetItem(iIndex, eMemProcessWorkingSet, wxT("0 KB"));

			SetItemImage(iIndex, pProcessInfo->iIconIndex);
			this->SetItemData(iIndex, iIndex);

			iIndex++;
			++cStartIter;
		}
	}
}

void CProcessListViewCommon::SetTextBackgroundColor(int iIndex, float fValue, bool IsMem)
{
	this->SetItemTextColour(iIndex, wxColour(60, 60, 60));

	if(IsMem)
	{
		float fValueCheck = fValue / 1024.0f;

		if(fValueCheck >= 1000.0f)     this->SetItemBackgroundColour(iIndex, wxColour(255, 60, 60));
		else if(fValueCheck >= 900.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 66, 66));
		else if(fValueCheck >= 800.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 74, 74));
		else if(fValueCheck >= 700.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 104, 104));
		else if(fValueCheck >= 600.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 121, 121));
		else if(fValueCheck >= 500.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 147, 147));
		else if(fValueCheck >= 400.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 166, 166));
		else if(fValueCheck >= 300.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 189, 189));
		else if(fValueCheck >= 200.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 218, 218));
		else if(fValueCheck >= 100.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 230, 230));
		else                           this->SetItemBackgroundColour(iIndex, wxColour(240, 240, 240));
	}
	else
	{
		if(fValue >= 50.0f)      this->SetItemBackgroundColour(iIndex, wxColour(255, 60, 60));//this->SetItemTextColour(iIndex, wxColour(255, 220, 220));
		else if(fValue >= 40.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 77, 77));
		else if(fValue >= 30.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 113, 113));
		else if(fValue >= 20.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 176, 176));
		else if(fValue >= 10.0f) this->SetItemBackgroundColour(iIndex, wxColour(255, 210, 210));
		else if(fValue >= 1.0f)  this->SetItemBackgroundColour(iIndex, wxColour(192, 255, 192));
		else                     this->SetItemBackgroundColour(iIndex, wxColour(240, 240, 240));

	}
}

void CProcessListViewCommon::AddNewProcess(unsigned long ulProcessID, eProcessListView _enumList)
{
	SHWSYSMON_PROCESS_INFO* pNewProcess = theSystem->GetProcessInfo(ulProcessID);//m_ProcessLists.find(ulProcessID);
	if(pNewProcess == nullptr)
		return;

	int iItemCount = this->GetItemCount();

	if(_enumList == eTotalList)
	{
		InsertItem(iItemCount, pNewProcess->_strProcessName);
		SetItem(iItemCount, ePID, wxString::Format(wxT("%u"), pNewProcess->_ulProcessID));

		SetItem(iItemCount, eCPU, wxT("0.0%"));
		SetItem(iItemCount, ePrivateBytes, wxT("0 KB"));
		SetItem(iItemCount, eWorkingSetSize, wxT("0 KB"));

		SetItem(iItemCount, eFileDescription, pNewProcess->_strFileDescription);
		SetItem(iItemCount, eCompanyName, pNewProcess->_strCompanyName);
		SetItem(iItemCount, eFullPath, pNewProcess->_strProcessFullPath);

		SetItemImage(iItemCount, pNewProcess->iIconIndex);

		m_bSorted = false;
	}

	if(_enumList == eCPUList)
	{
		InsertItem(iItemCount, pNewProcess->_strProcessName);
		SetItem(iItemCount, eProcessPID, wxString::Format(wxT("%u"), pNewProcess->_ulProcessID));
		SetItem(iItemCount, eProcessCPU, wxT("0.0%"));
		SetItemImage(iItemCount, pNewProcess->iIconIndex);
	}

	if(_enumList == eMemoryList)
	{
		InsertItem(iItemCount, pNewProcess->_strProcessName);
		SetItem(iItemCount, eMemProcessPID, wxString::Format(wxT("%u"), pNewProcess->_ulProcessID));

		SetItem(iItemCount, eMemProcessPrivateBytes, wxT("0 KB"));
		SetItem(iItemCount, eMemProcessWorkingSet, wxT("0 KB"));

		SetItemImage(iItemCount, pNewProcess->iIconIndex);
	}
}

void CProcessListViewCommon::DelProcess(unsigned long ulProcess)
{
	int iLstCnt = this->GetItemCount();

	wxString strProcessID(wxT(""));
	int iDelItem = -1;
	for (int i = 0; i < iLstCnt; i++)
	{
		strProcessID = this->GetItemText(i, 1);
		unsigned long _ulProcessID = 0;

		strProcessID.ToCULong(&_ulProcessID);

		if(ulProcess == _ulProcessID)
		{
			iDelItem = i;
			break;
		}
	}

	this->DeleteItem(iDelItem);
}

void CProcessListViewCommon::CreateContextMenu()
{
	m_pContextMenu = new wxMenu();

	CONTEXT_MENU cMenu[] = {
		{ CONTEXT_MENU_KILL_PROCESS,        wxT("kill process"),    m_bitmapKillProcess},
		{ CONTEXT_MENU_VIEW_PROCESS_DETAIL, wxT("View Dependency"), m_bitmapDetail},
		{ CONTEXT_MENU_DEL_LISTITEM,        wxT("Delete Item"),     m_bitmapDelItem},
		{ CONTEXT_MENU_UPDATE_PROCESSINFO,  wxT("Update Info"),     m_bitmapUpdate},
	};

	int iMenuCount = WXSIZEOF(cMenu);
	for(int i = 0; i < iMenuCount; i++)
	{
		wxMenuItem* pItem = m_pContextMenu->Append(cMenu[i].iId, cMenu[i].strMenuName);
		pItem->SetBitmap(cMenu[i].cmbitmap);

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &CProcessListViewCommon::OnListContextMenu, this, cMenu[i].iId);
	}
}

void CProcessListViewCommon::OnListContextMenu(wxCommandEvent& event)
{
	int iColumnCount = GetColumnCount();

	int iID = event.GetId();

	wxString strProcessID = this->GetItemText(m_iSelItemRightClick, 1);
	unsigned long ulProcessID = 0;
	strProcessID.ToULong(&ulProcessID);

	if(iID == CONTEXT_MENU_KILL_PROCESS)
	{
		wxString strMsgProcessKill(wxT(""));

		strMsgProcessKill = wxT("??????????????? : ");
		strMsgProcessKill += this->GetItemText(m_iSelItemRightClick, 0);  //???????????????
		strMsgProcessKill += wxT("\n");
		strMsgProcessKill += wxT("ProcessID : ");

		strMsgProcessKill += strProcessID;  //PID
		strMsgProcessKill += wxT("\n");

		if(iColumnCount == 8)
		{
			strMsgProcessKill += wxT("File Description : ");
			strMsgProcessKill += this->GetItemText(m_iSelItemRightClick, 5);  //??????
			strMsgProcessKill += wxT("\n");
			strMsgProcessKill += wxT("Company Name : ");
			strMsgProcessKill += this->GetItemText(m_iSelItemRightClick, 6);  //?????????
			strMsgProcessKill += wxT("\n");
			strMsgProcessKill += wxT("FullPath : ");
			strMsgProcessKill += this->GetItemText(m_iSelItemRightClick, 7);  //??????
			strMsgProcessKill += wxT("\n");
		}
		else
		{
			int iIndex = -1;
			if(iColumnCount == 5)
				iIndex = 4;
			else
				iIndex = 3;

			strMsgProcessKill += wxT("File Description : ");
			strMsgProcessKill += this->GetItemText(m_iSelItemRightClick, iIndex);  //??????
			strMsgProcessKill += wxT("\n");
		}

		strMsgProcessKill += wxT("\n??? ??????????????? ????????? ?????????????????????????");
		int iRetID = wxMessageBox(strMsgProcessKill, PROGRAM_FULL_NAME, wxICON_QUESTION | wxYES_NO);
		if(iRetID == wxYES)
		{
			int iRet = theSystem->KillProcess(ulProcessID);
			if(iRet != KILL_PROCESS_MSG_SUCCESS)
			{
				wxString strErrMsg(wxT(""));
				switch(iRet)
				{
					case KILL_PROCESS_MSG_NOT_PROCESSID:
						strErrMsg = wxT("???????????? ID??? ???????????? ????????????.");
						break;

					case KILL_PROCESS_MSG_PROCESS_NULL:
						strErrMsg = wxT("??????????????? ?????? ????????????.");
						break;

					case KILL_PROCESS_MSG_PROCESS_TERMINATE_FAIL:
						strErrMsg = wxT("???????????? ????????? ?????????????????????.");
						break;

					case KILL_PROCESS_MSG_PROCESS_ALIVE:
						strErrMsg = wxT("??????????????? ?????? ?????? ????????????.");
						break;

					default:
						break;
				}

				wxMessageBox(strErrMsg, PROGRAM_FULL_NAME, wxICON_INFORMATION | wxOK);
				return;
			}
		}
	}

	if(iID == CONTEXT_MENU_VIEW_PROCESS_DETAIL)
	{
		DlgProcessDetail dlg(nullptr);
		dlg.SetProcessID(ulProcessID);

		dlg.ShowModal();
		dlg.Destroy();
	}

	if(iID == CONTEXT_MENU_DEL_LISTITEM)
		CProcessListViewCommon::DelListViewItem(ulProcessID);

	if(iID == CONTEXT_MENU_UPDATE_PROCESSINFO)
		UpdateProcessInfo(ulProcessID);
}

void CProcessListViewCommon::OnListItemRightClick( wxListEvent& event )
{
	m_iSelItemRightClick = event.GetIndex();
	wxPoint pt = event.GetPoint();

	this->PopupMenu(m_pContextMenu, pt.x, pt.y);
}

void CProcessListViewCommon::AddListViewItem(CProcessListViewCommon* pListView)
{
	m_pListViews.push_back(pListView);
}

void CProcessListViewCommon::DelListViewItem(unsigned long ulProcessID)
{
	for(auto pView : m_pListViews)
		pView->DelProcess(ulProcessID);
}

void CProcessListViewCommon::allclear()
{
	this->DeleteAllItems();
}

void CProcessListViewCommon::UpdateProcessInfo(unsigned long ulProcessID)
{
	wxString strMsg(wxString::Format(wxT("ProcessId : %u"), ulProcessID));
	if(!theSystem->UpdateProcessInfo(ulProcessID))
	{
		wxMessageBox(strMsg + wxT("  ?????? ????????? ??????????????????."), PROGRAM_FULL_NAME, wxICON_ERROR | wxOK);
		return;
	}

	wxMessageBox(strMsg + wxT(" ??????????????? ?????????????????????."), PROGRAM_FULL_NAME, wxICON_EXCLAMATION | wxOK);
}
