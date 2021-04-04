#include "../ginc.h"
#include "DlgProcessDetail.h"
#include "../system/process/FileVersionInfo.h"
#include "../system/process/ProcessMonitoring.h"

DlgProcessDetail::DlgProcessDetail( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	SetIcon(wxIcon("wxwin"));
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );


	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("ProgramName : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer5->Add( m_staticText3, 0, wxLEFT|wxTOP, 5 );

	m_staticProgramName = new wxStaticText( this, wxID_ANY, wxT("ddd"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticProgramName->Wrap( -1 );
	m_staticProgramName->SetFont(wxSystemSettings::GetFont(wxSYS_SYSTEM_FIXED_FONT));

	bSizer5->Add( m_staticProgramName, 0, wxALL, 5 );


	bSizer1->Add( bSizer5, 0, wxEXPAND, 5 );

	m_lstItems = new wxListView( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	m_lstItems->SetFont(wxSystemSettings::GetFont(wxSYS_SYSTEM_FIXED_FONT));
	bSizer1->Add( m_lstItems, 1, wxALL|wxEXPAND, 5 );

	m_lstItems->SetTextColour(wxColour(60, 60, 60));
	m_lstItems->SetBackgroundColour(wxColour(240, 240, 240));

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("CommandLine : "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	m_staticText1->SetFont(wxSystemSettings::GetFont(wxSYS_SYSTEM_FIXED_FONT));
	bSizer4->Add( m_staticText1, 0, wxLEFT|wxTOP, 5 );

	m_staticCommandLine = new wxStaticText( this, wxID_ANY, wxT("ddd"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticCommandLine->Wrap( -1 );
	m_staticCommandLine->SetFont(wxSystemSettings::GetFont(wxSYS_SYSTEM_FIXED_FONT));

	bSizer4->Add( m_staticCommandLine, 1, wxEXPAND|wxTOP, 5 );
	bSizer1->Add( bSizer4, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_btnClose = new wxButton( this, wxID_ANY, wxT("닫기"), wxDefaultPosition, wxDefaultSize, 0 );

	m_btnClose->SetBitmap( wxArtProvider::GetBitmap( wxART_CLOSE, wxART_BUTTON ) );
	bSizer3->Add( m_btnClose, 0, wxALL, 5 );


	bSizer2->Add( bSizer3, 0, wxALIGN_RIGHT, 5 );


	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgProcessDetail::OnInitDialog ) );
	m_btnClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgProcessDetail::OnClose ), NULL, this );
}

DlgProcessDetail::~DlgProcessDetail()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( DlgProcessDetail::OnInitDialog ) );
	m_btnClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgProcessDetail::OnClose ), NULL, this );

}

void DlgProcessDetail::OnInitDialog( wxInitDialogEvent& event )
{
	if(m_ulProcessID == 0)
		return;

#ifdef __WXMSW__
	ListView_SetImageList(m_lstItems->GetHWND(), _gImageList, LVSIL_SMALL);
	InitProcessDetail();
#else
	//other platform
#endif
}

void DlgProcessDetail::OnClose( wxCommandEvent& event )
{
	EndDialog(wxID_CLOSE);
}

void DlgProcessDetail::SetProcessID(unsigned long ulProcessID)
{
	m_ulProcessID = ulProcessID;
}

void DlgProcessDetail::InitProcessDetail()
{
	sListColumnItem lstColumn[] = {
		{wxT("ModulePath")      , 480, wxLIST_FORMAT_LEFT},
		{wxT("Company")         , 200, wxLIST_FORMAT_RIGHT},
		{wxT("File Description"), 350, wxLIST_FORMAT_LEFT},
	};

	for (unsigned int i = 0; i < WXSIZEOF(lstColumn); i++)
	{
		wxListItem lstItemInfo;
		lstItemInfo.SetText(lstColumn[i]._strColumnName);
		lstItemInfo.SetAlign(lstColumn[i]._align);
		lstItemInfo.SetWidth(lstColumn[i]._width);

		m_lstItems->InsertColumn(i, lstItemInfo);
	}

	wxColour col(60, 60, 60);

	CProcessMonitoring* pMonitor = (CProcessMonitoring *)theSystem->GetBaseResource(S_BASE_CLASS_PROCESS);
	SHWSYSMON_PROCESS_INFO* pProcess = pMonitor->GetProcessInfo(m_ulProcessID);

	m_staticProgramName->SetLabelText(pProcess->_strProcessName);
	m_staticCommandLine->SetLabelText(pProcess->_strCommandLine);

	HMODULE hMods[1024];
	HANDLE hProcess;
    DWORD cbNeeded;
    unsigned int i;

    hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                            PROCESS_VM_READ,
                            FALSE, m_ulProcessID );
	if(!hProcess)
		return;

	wxString strModuleName(wxT(""));
	wxString strDisp(wxT(""));

	int iIconIndex = -1;
	int iOverlayIndex = -1;
	wchar_t _tBUF[10] = {0x00, };

	DWORD dwFlags = LIST_MODULES_ALL;
	if( EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, dwFlags))
    {
		wxString strCompany(wxT(""));
		wxString strDescription(wxT(""));
		int iIndex = 0;
        for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
        {
            wchar_t szModName[MAX_PATH];

			strCompany = wxT("");
			strDescription = wxT("");

            if ( GetModuleFileNameEx( hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(wchar_t)))
            {
				strModuleName = szModName;
				theUtility->GetIconIndexFromImageList(strModuleName, iIconIndex, iOverlayIndex);

				memset(_tBUF, 0x00, sizeof(_tBUF));
				swprintf(_tBUF, sizeof(_tBUF), TEXT(" (0x%08X)"), hMods[i]);
				strDisp = strModuleName + wxString::Format(wxT("%s"), _tBUF);

				CFileVersionInfo data;
				if(data.Create(strModuleName))
				{
					strCompany = data.GetCompanyName();
					strDescription = data.GetFileDescription();
				}

				m_lstItems->InsertItem(iIndex, strDisp);
				m_lstItems->SetItem(iIndex, 1, strCompany);
				m_lstItems->SetItem(iIndex, 2, strDescription);
				m_lstItems->SetItemImage(iIndex, iIconIndex);
				m_lstItems->SetItemTextColour(iIndex, col);
				m_lstItems->SetItemData(iIndex, iIndex);

				iIndex++;
            }
        }

		m_lstItems->SortItems(DlgProcessDetail::ListSortFunction, (LPARAM)this);
    }

    CloseHandle( hProcess );
}

int wxCALLBACK DlgProcessDetail::ListSortFunction(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
	DlgProcessDetail* pDlg = (DlgProcessDetail *)sortData;
	if(pDlg->m_lstItems == nullptr)
	{
		if (item1 < item2)
			return 1;

		if (item1 > item2)
			return -1;

		return 0;
	}

	int iSortingColumn = 0;
	bool bAscending = true;

	wxString str1 = pDlg->m_lstItems->GetItemText(item1, iSortingColumn);
	wxString str2 = pDlg->m_lstItems->GetItemText(item2, iSortingColumn);

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
